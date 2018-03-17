#ifndef ENGINE_COMPONENT_APP_INTERRUPTER_REAL_HPP
#define ENGINE_COMPONENT_APP_INTERRUPTER_REAL_HPP
#pragma once

#include "global/component/logger.hpp"
#include "global/component/app_interrupter.hpp"
#include "utility/container/concurrent_queue.hpp"
#include <map>
#include <memory>
#include <vector>
#include <array>

namespace engine
{

	class app_interrupter_real_t : public app_interrupter_t, public std::enable_shared_from_this<app_interrupter_real_t>
	{

	public:

		app_interrupter_real_t(std::shared_ptr<logger_t> logger) : logger(logger)
		{
			auto task_id = logger->log_global_task_start(apps, "Initializing app interrupter"_u);
			logger->log_global_task_done(task_id);
		}

		~app_interrupter_real_t()
		{
			auto task_id = logger->log_global_task_start(apps, "App interrupter is being disposed"_u);
			logger->log_global_task_done(task_id);
		}

		std::unique_ptr<instance_t> register_handler(app_t::kind_t app, app_context_t* context, priority_t priority, handler_t handler) final
		{
			return std::make_unique<instance_real_t>(shared_from_this(), app, context->get_instance_id(), priority, std::move(handler));
		}

        void send_interruption(std::unique_ptr<interruption_t> interruption) final
		{
			interruptions.push(std::move(interruption));	
		}

	private:

		std::shared_ptr<logger_t> logger;
		concurrent_queue_t<std::unique_ptr<interruption_t> > interruptions;

		class instance_real_t;
		std::mutex mutex;

		void register_instance(app_t::kind_t app, app_t::instance_id_t target, priority_t priority, instance_real_t * instance)
		{
			std::lock_guard<std::mutex> guard(mutex);
			auto task_id = logger->log_task_start(app, target, apps, "New handler for interruption is being registered with priority '#1#'"_u, priority);
			auto ret = instances[target].register_instance(priority, instance);
			switch(ret)
			{
				case app_info_t::register_result_t::success: logger->log_task_done(task_id); break;
				case app_info_t::register_result_t::already_registered: logger->log_warn(app, target, apps, "The handler attempted to register itself second time (priority '#1#')"_u, priority); logger->log_task_failed(task_id); break;
				default: logger->log_err(app, target, apps, "The handler returned unexpected message, most likely due to stack corruption"_u); logger->log_task_failed(task_id);
			}
		}

		void unregister_instance(app_t::kind_t app, app_t::instance_id_t target, priority_t priority, instance_real_t * instance)
		{
			std::lock_guard<std::mutex> guard(mutex);
			auto task_id = logger->log_task_start(app, target, apps, "Handler for interruption with priority '#1#' is being destroyed"_u, priority);
			auto ret = instances[target].unregister_instance(priority, instance);
			switch(ret)
			{
				case app_info_t::unregister_result_t::success_no_more_instances: instances.erase(target); // No break, it is intended
				case app_info_t::unregister_result_t::success_have_more_instances: logger->log_task_done(task_id); break;
				case app_info_t::unregister_result_t::wrong_priority: logger->log_err(app, target, apps, "The handler attempted to unregister itself with wrong priority, most likely due to stack corruption"_u); logger->log_task_failed(task_id); break;
				case app_info_t::unregister_result_t::not_registered: logger->log_warn(app, target, apps, "The handler attempted to free itself, while it was not registered in the first place"_u); logger->log_task_failed(task_id); break;
				default: logger->log_err(app, target, apps, "The handler returned unexpected message, most likely due to stack corruption"_u); logger->log_task_failed(task_id);
			}
		}

		void handle_interruption(std::unique_ptr<interruption_t> interruption)
		{
			std::lock_guard<std::mutex> guard(mutex);
			auto item = instances.find(interruption->get_target());
			if(item == instances.end())
				interruption->execute_unhandled_handler();
			else
			{
				for(std::size_t i = 0; i < value_of(priority_t::count); i++)
				{
					instances_t * instances = item->second.get(static_cast<priority_t>(i));
					for(std::size_t p = 0; p < instances->size(); p++)
					{
						(*instances)[p]->execute(interruption.get());
						if(interruption->was_handled()) return;
					}
				}
				interruption->execute_unhandled_handler();
			}
		}

		class instance_real_t : public instance_t
		{

			public:

				instance_real_t(std::shared_ptr<app_interrupter_real_t> owner, app_t::kind_t app, app_t::instance_id_t target, priority_t priority, handler_t handler) : owner(owner), app(app), target(target), priority(priority), handler(std::move(handler))
				{
					owner->register_instance(app, target, priority, this);
				}

				~instance_real_t()
				{
					owner->unregister_instance(app, target, priority, this);
				}

				void execute(interruption_t * interruption)
				{
					handler(interruption);
				}

			private:

				std::shared_ptr<app_interrupter_real_t> owner;
				app_t::kind_t app;
				app_t::instance_id_t target;
				priority_t priority;
				handler_t handler;

		};
		typedef std::vector<instance_real_t*> instances_t;

		class app_info_t
		{
			public:

				enum class register_result_t
				{
					success,
					already_registered,
					count
				};

				enum class unregister_result_t
				{
					success_have_more_instances,
					success_no_more_instances,
					wrong_priority,
					not_registered,
					count
				};

				register_result_t register_instance(priority_t priority, instance_real_t* instance)
				{
					for(std::size_t i = 0; i < value_of(priority_t::count); i++)
					{
						if(std::find(instances_by_priority[i].begin(), instances_by_priority[i].end(), instance) != instances_by_priority[i].end())
							return register_result_t::already_registered;
					}
					instances_by_priority[value_of(priority)].push_back(instance);
					return register_result_t::success;
				}

				unregister_result_t unregister_instance(priority_t priority, instance_real_t* instance)
				{
					bool is_empty = true;
					for(std::size_t i = 0; i < value_of(priority_t::count); i++)
					{
						auto iter = std::find(instances_by_priority[i].begin(), instances_by_priority[i].end(), instance);
						if(iter != instances_by_priority[i].end())
						{
							if(i != value_of(priority)) return unregister_result_t::wrong_priority;
							instances_by_priority[i].erase(iter);

							for(std::size_t p = i; p < value_of(priority_t::count); p++)
							{
								if(!is_empty) return unregister_result_t::success_have_more_instances;
								if(!instances_by_priority[p].empty())
									is_empty = false;
							}
							return unregister_result_t::success_no_more_instances;
						}
						if(!instances_by_priority[i].empty())
							is_empty = false;
					}
					return unregister_result_t::not_registered;
				}

				instances_t * get(priority_t priority)
				{
					return &instances_by_priority[value_of(priority)];
				}

			private:
				typedef std::array<instances_t, value_of(priority_t::count)> instances_by_priority_t;
				instances_by_priority_t instances_by_priority;

		};


		std::map<app_t::instance_id_t, app_info_t> instances;

	};
}

#endif