#ifndef ENGINE_COMPONENT_APP_OVERSEER_REAL_HPP
#define ENGINE_COMPONENT_APP_OVERSEER_REAL_HPP
#pragma once

#include "global/component/app_overseer.hpp"
#include "global/core/messenger.hpp"
#include "global/component/logger.hpp"
#include "global/component/app_interrupter.hpp"
#include "global/component/program_instancer.hpp"
#include "global/core/process/service.hpp"
#include "utility/container/concurrent_queue.hpp"
#include "utility/concurrention/signal.hpp"
#include "global/core/process/runner.hpp"
#include "global/core/policy/instances_application.hpp"
#include <memory>

namespace engine
{

	class app_overseer_actual_t : public std::enable_shared_from_this<app_overseer_actual_t>
	{

	public:

		app_overseer_actual_t(std::shared_ptr<logger_t> logger, std::shared_ptr<app_interrupter_t> app_interrupter, std::shared_ptr<thread_factory_t> thread_factory, std::shared_ptr<app_factory_t> app_factory, std::shared_ptr<instances_application_t> policy_instances_application, std::shared_ptr<program_instancer_t> program_instancer) : logger(logger), app_interrupter(app_interrupter), app_factory(app_factory), policy_instances_application(policy_instances_application), program_instancer(program_instancer), app_runner_id(0)
		{

		}

		void run_app(app_t::kind_t kind, std::unique_ptr<app_context_t> context, app_overseer_t::run_app_instance_t instance = app_overseer_t::run_app_instance_t::allow_multiple, app_overseer_t::run_app_other_t other = app_overseer_t::run_app_other_t::keep, app_overseer_t::run_app_program_t program = app_overseer_t::run_app_program_t::if_possible_same_instance_if_not_new_instance, app_overseer_t::callback_app_t app_running = [](std::shared_ptr<app_t>){}, app_overseer_t::callback_void_t run_failed = [](){}, app_overseer_t::callback_void_t run_succeed_in_new_program_instance = [](){})
		{
			events.push(std::make_unique<run_app_event_t>(shared_from_this(), kind, std::move(context), instance, other, program, std::move(app_running), std::move(run_failed), std::move(run_succeed_in_new_program_instance)));
		}

        void terminate_app(app_t::instance_id_t instance, app_overseer_t::callback_void_t on_completed = [](){})
		{
			events.push(std::make_unique<terminate_app_event_t>(shared_from_this(), instance, std::move(on_completed)));
		}

        void close_app(app_t::instance_id_t instance, app_overseer_t::callback_void_t on_allowed = [](){}, app_overseer_t::callback_void_t on_refused = [](){})
		{
			events.push(std::make_unique<close_app_event_t>(shared_from_this(), instance, std::move(on_allowed), std::move(on_refused)));
		}

        void terminate_all_of(app_t::kind_t app, app_overseer_t::callback_void_t on_completed = [](){})
		{
			events.push(std::make_unique<terminate_all_of_event_t>(shared_from_this(), app, std::move(on_completed)));
		}

        void close_all_of(app_t::kind_t app, app_overseer_t::callback_void_t on_all_accepted = [](){}, app_overseer_t::callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, app_overseer_t::callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){})
		{
			events.push(std::make_unique<close_all_of_event_t>(shared_from_this(), app, std::move(on_all_accepted), std::move(on_given_accepted), std::move(on_given_rejected)));

		}

        void terminate_all(app_overseer_t::callback_void_t on_completed = [](){})
		{
			events.push(std::make_unique<terminate_all_event_t>(shared_from_this(), std::move(on_completed)));
		}

        void close_all(app_overseer_t::callback_void_t on_all_accepted = [](){}, app_overseer_t::callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, app_overseer_t::callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){})
		{
			events.push(std::make_unique<close_all_event_t>(shared_from_this(), std::move(on_all_accepted), std::move(on_given_accepted), std::move(on_given_rejected)));
		}

        void wait_for_completion()
		{
			signal_completion.wait();
		}

		void app_created(std::shared_ptr<app_meta_t> app_meta)
		{
			events.push(std::make_unique<app_created_event_t>(shared_from_this(), app_meta));
		}

		void app_destroyed(std::shared_ptr<app_meta_t> app_meta)
		{
			events.push(std::make_unique<app_destroyed_event_t>(shared_from_this(), app_meta));
		}

        void terminate()
		{
			events.push(std::unique_ptr<event_t>());
		}

		task_base_t::result_t run()
		{
			for(;;)
			{
				std::unique_ptr<event_t> event = std::move(events.pop());
				if(!event)
				{
					complete_run();
					return task_base_t::result_t::completed;
				}
				handle_event(std::move(event));
			}
		}

		std::shared_ptr<app_interrupter_t> get_app_interrupter()
		{
			return app_interrupter;
		}

	private:

		signal_t signal_completion;

		class event_t
		{
			public:

				virtual ~event_t()
				{

				}

#define ENGINE_APP_OVERSEER_EVENT_TYPE_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(type_t, __VA_ARGS__)
#include "def/app_overseer.def"

				type_t get_type() const
				{
					return type;
				}

				std::shared_ptr<app_overseer_actual_t> get_actual()
				{
					return actual;
				}

			protected:

				event_t(std::shared_ptr<app_overseer_actual_t> actual, type_t type) : type(type), actual(actual)
				{

				}

			private:

				std::shared_ptr<app_overseer_actual_t> actual;
				type_t type;
		};

		concurrent_queue_t<std::unique_ptr<event_t> > events;

		class run_app_event_t : public event_t
		{

			public:

				run_app_event_t(std::shared_ptr<app_overseer_actual_t> actual, app_t::kind_t kind, std::unique_ptr<app_context_t> context, app_overseer_t::run_app_instance_t instance = app_overseer_t::run_app_instance_t::allow_multiple, app_overseer_t::run_app_other_t other = app_overseer_t::run_app_other_t::keep, app_overseer_t::run_app_program_t program = app_overseer_t::run_app_program_t::if_possible_same_instance_if_not_new_instance, app_overseer_t::callback_app_t app_running = [](std::shared_ptr<app_t>){}, app_overseer_t::callback_void_t run_failed = [](){}, app_overseer_t::callback_void_t run_succeed_in_new_program_instance = [](){}) :
					event_t(actual, type_t::run_app), kind(kind), context(std::move(context)), instance(instance), other(other), program(program), app_running(std::move(app_running)), run_failed(std::move(run_failed)), run_succeed_in_new_program_instance(std::move(run_succeed_in_new_program_instance))
				{

				}

				app_t::kind_t get_kind() const
				{
					return  kind;
				}

				app_context_t * get_context() const
				{
					return context.get();
				}

				std::unique_ptr<app_context_t> move_context()
				{
					return std::move(context);
				}

				app_overseer_t::run_app_instance_t get_instance() const
				{
					return instance;
				}

				app_overseer_t::run_app_other_t get_other() const
				{
					return other;
				}

				app_overseer_t::run_app_program_t get_program() const
				{
					return program;
				}
				
				app_overseer_t::callback_app_t move_on_app_running()
				{
					return std::move(app_running);
				}

				app_overseer_t::callback_void_t move_on_run_failed()
				{
					return std::move(run_failed);
				}

				app_overseer_t::callback_void_t move_on_run_succeed_in_new_program_instance()
				{
					return std::move(run_succeed_in_new_program_instance);
				}

			private:

				app_t::kind_t kind;
				std::unique_ptr<app_context_t> context;
				app_overseer_t::run_app_instance_t instance;
				app_overseer_t::run_app_other_t other;
				app_overseer_t::run_app_program_t program;
				app_overseer_t::callback_app_t app_running;
				app_overseer_t::callback_void_t run_failed;
				app_overseer_t::callback_void_t run_succeed_in_new_program_instance;

		};

		class terminate_app_event_t : public event_t
		{

			public:

				terminate_app_event_t(std::shared_ptr<app_overseer_actual_t> actual, app_t::instance_id_t instance, app_overseer_t::callback_void_t on_completed = [](){}) : event_t(actual, type_t::terminate_app), instance(instance), on_completed(std::move(on_completed))
				{

				}

				app_t::instance_id_t get_instance() const
				{
					return instance;
				}

				app_overseer_t::callback_void_t move_on_completed()
				{
					return std::move(on_completed);
				}

			private:

				app_t::instance_id_t instance;
				app_overseer_t::callback_void_t on_completed;
		};
		

		class terminate_all_event_t : public event_t
		{

			public:

				terminate_all_event_t(std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_completed = [](){}) : event_t(actual, type_t::terminate_all), on_completed(std::move(on_completed))
				{

				}

				app_overseer_t::callback_void_t move_on_completed()
				{
					return std::move(on_completed);
				}

			private:

				app_overseer_t::callback_void_t on_completed;
		};
		
		

		class terminate_all_of_event_t : public event_t
		{

			public:

				terminate_all_of_event_t(std::shared_ptr<app_overseer_actual_t> actual, app_t::kind_t app, app_overseer_t::callback_void_t on_completed = [](){}) : event_t(actual, type_t::terminate_all_of), app(app), on_completed(std::move(on_completed))
				{

				}

				app_t::kind_t get_app() const
				{
					return app;
				}

				app_overseer_t::callback_void_t move_on_completed()
				{
					return std::move(on_completed);
				}

			private:

				app_t::kind_t app;
				app_overseer_t::callback_void_t on_completed;
		};
			

		class close_app_event_t : public event_t
		{

			public:

				close_app_event_t(std::shared_ptr<app_overseer_actual_t> actual, app_t::instance_id_t instance, app_overseer_t::callback_void_t on_allowed = [](){}, app_overseer_t::callback_void_t on_refused = [](){}) : event_t(actual, type_t::close_app), instance(instance), on_allowed(std::move(on_allowed)), on_refused(std::move(on_refused))
				{

				}

				app_t::instance_id_t get_instance() const
				{
					return instance;
				}

				app_overseer_t::callback_void_t move_on_allowed()
				{
					return std::move(on_allowed);
				}

				app_overseer_t::callback_void_t move_on_refused()
				{
					return std::move(on_refused);
				}

			private:

				app_t::instance_id_t instance;
				app_overseer_t::callback_void_t on_allowed;
				app_overseer_t::callback_void_t on_refused;
		};

		class close_all_event_t : public event_t
		{

			public:

				close_all_event_t(std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_all_accepted = [](){}, app_overseer_t::callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, app_overseer_t::callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){}) : event_t(actual, type_t::close_all), on_all_accepted(std::move(on_all_accepted)), on_given_accepted(std::move(on_given_accepted)), on_given_rejected(std::move(on_given_rejected))
				{

				}

				app_overseer_t::callback_void_t move_on_all_accepted()
				{
					return std::move(on_all_accepted);
				}

				app_overseer_t::callback_app_instance_t move_on_given_accepted()
				{
					return std::move(on_given_accepted);
				}

				app_overseer_t::callback_app_instance_t move_on_given_rejected()
				{
					return std::move(on_given_rejected);
				}

			private:

				app_overseer_t::callback_void_t on_all_accepted;
				app_overseer_t::callback_app_instance_t on_given_accepted;
				app_overseer_t::callback_app_instance_t on_given_rejected;
		};
		
		class close_all_of_event_t : public event_t
		{

			public:

				close_all_of_event_t(std::shared_ptr<app_overseer_actual_t> actual, app_t::kind_t app, app_overseer_t::callback_void_t on_all_accepted = [](){}, app_overseer_t::callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, app_overseer_t::callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){}) : event_t(actual, type_t::close_all_of), app(app), on_all_accepted(std::move(on_all_accepted)), on_given_accepted(std::move(on_given_accepted)), on_given_rejected(std::move(on_given_rejected))
				{

				}

				app_t::kind_t get_app() const
				{
					return app;
				}

				app_overseer_t::callback_void_t move_on_all_accepted()
				{
					return std::move(on_all_accepted);
				}

				app_overseer_t::callback_app_instance_t move_on_given_accepted()
				{
					return std::move(on_given_accepted);
				}

				app_overseer_t::callback_app_instance_t move_on_given_rejected()
				{
					return std::move(on_given_rejected);
				}

			private:

				app_t::kind_t app;
				app_overseer_t::callback_void_t on_all_accepted;
				app_overseer_t::callback_app_instance_t on_given_accepted;
				app_overseer_t::callback_app_instance_t on_given_rejected;
		};

		
		class app_created_event_t : public event_t
		{

			public:

				app_created_event_t(std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<app_meta_t> meta) : event_t(actual, type_t::app_created), meta(meta)
				{

				}

				std::shared_ptr<app_meta_t> get_meta()
				{
					return meta;
				}

			private:

				std::shared_ptr<app_meta_t> meta;
		};

		
		class app_destroyed_event_t : public event_t
		{

			public:

				app_destroyed_event_t(std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<app_meta_t> meta) : event_t(actual, type_t::app_destroyed), meta(meta)
				{

				}

				std::shared_ptr<app_meta_t> get_meta()
				{
					return meta;
				}

			private:

				std::shared_ptr<app_meta_t> meta;
		};

		class app_running_t
		{
			public:

				app_running_t(std::shared_ptr<app_meta_t> app_meta) : app_meta(app_meta)
				{

				}

				~app_running_t()
				{
					std::lock_guard<std::mutex> guard(terminate_mutex);

					if(flags.is_flag(flag_t::being_terminated))
					{
						for(auto & handler : on_completed)
						{
							handler();
						}
					}
				}

				std::shared_ptr<app_meta_t> get_app_meta()
				{
					return app_meta;
				}

				app_t::instance_id_t get_instance_id()
				{
					return app_meta->get_instance_id();
				}

				void terminate(std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_completed)
				{
					this->on_completed.push_back(std::move(on_completed));

					std::lock_guard<std::mutex> guard(terminate_mutex);

					if(!flags.is_flag(flag_t::being_terminated))
						actual->get_app_interrupter()->send_interruption(std::make_unique<interruption_terminate_t>(get_instance_id()));

					flags.set_flag(flag_t::being_terminated, true);
				}

				void close(std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_completed, app_overseer_t::callback_void_t on_refused)
				{
					actual->get_app_interrupter()->send_interruption(std::make_unique<interruption_ask_for_close_t>(get_instance_id(), std::bind([](app_overseer_t::callback_void_t on_completed, std::shared_ptr<app_overseer_actual_t> actual, app_t::instance_id_t instance){
						actual->terminate_app(instance, std::move(on_completed));
					}, std::move(on_completed), actual, get_instance_id()), std::bind([](app_overseer_t::callback_void_t on_refused, std::shared_ptr<app_overseer_actual_t> actual){
						on_refused();
					}, std::move(on_refused), actual)));
				}

			private:

				std::mutex terminate_mutex;
				std::shared_ptr<app_meta_t> app_meta;
				std::vector<app_overseer_t::callback_void_t> on_completed;

				enum class flag_t
				{
					being_terminated,
					count
				};

				flags_t<flag_t> flags;

		};

		class kind_running_t
		{
			public:

				app_running_t * get_app(app_t::instance_id_t instance_id)
				{
					auto iter = std::find_if(apps_running.begin(), apps_running.end(), [instance_id](std::shared_ptr<app_running_t> app_running){ return app_running->get_instance_id() == instance_id; });
					if(iter == apps_running.end())
						return nullptr;

					return iter->get();
				}

				void add_app(std::shared_ptr<app_running_t> app_running)
				{
					apps_running.push_back(app_running);
				}

				void remove_app(app_t::instance_id_t instance_id)
				{
					auto iter = std::find_if(apps_running.begin(), apps_running.end(), [instance_id](std::shared_ptr<app_running_t> app_running){ return app_running->get_instance_id() == instance_id; });
					if(iter != apps_running.end())
						apps_running.erase(iter);

					if(apps_running.empty())
					{
						if(flags.is_flag(flag_t::being_closed))
						{
							flags.set_flag(flag_t::being_closed, false);	
							for(auto & item : on_close_all_completed)
								item();
							on_close_all_completed.clear();
							on_close_given_completed.clear();
							on_close_given_rejected.clear();
						}
						if(flags.is_flag(flag_t::being_terminated))
						{
							flags.set_flag(flag_t::being_terminated, false);	
							for(auto & item : on_terminate_all_completed)
								item();
							on_terminate_all_completed.clear();
						}
					}
				}

				bool is_empty()
				{
					return apps_running.empty();
				}

				bool is_being_closed()
				{
					return flags.is_flag(flag_t::being_closed);
				}

				bool is_being_terminated()
				{
					return flags.is_flag(flag_t::being_terminated);
				}


				void close_all(std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_all_accepted, app_overseer_t::callback_app_instance_t on_given_accepted, app_overseer_t::callback_app_instance_t on_given_rejected)
				{
					on_close_all_completed.push_back(std::move(on_all_accepted));
					flags.set_flag(flag_t::being_closed, true);
					for(auto & app_running : apps_running)
					{
						auto instance_id = app_running->get_instance_id();
						app_running->close(actual, [instance_id, on_given_accepted](){ on_given_accepted(instance_id); }, [instance_id, on_given_rejected](){ on_given_rejected(instance_id); });
					}
				}

				void terminate_all(std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_all_accepted)
				{
					on_terminate_all_completed.push_back(std::move(on_all_accepted));
					flags.set_flag(flag_t::being_terminated, true);
					for(auto & app_running : apps_running)
					{
						app_running->terminate(actual, [](){});
					}
				}

				void given_closed(app_t::instance_id_t instance_id)
				{
					if(flags.is_flag(flag_t::being_closed))
					{
						for(auto & item : on_close_given_completed)
							item(instance_id);
					}
				}

				void given_not_closed(app_t::instance_id_t instance_id)
				{
					if(flags.is_flag(flag_t::being_closed))
					{
						for(auto & item : on_close_given_rejected)
							item(instance_id);
					}
					flags.set_flag(flag_t::being_closed, false);
				}

			private:

				enum class flag_t
				{
					being_closed,
					being_terminated,
					count
				};

				flags_t<flag_t> flags;
				std::vector<std::shared_ptr<app_running_t> > apps_running;
				std::vector<app_overseer_t::callback_app_instance_t> on_close_given_completed;
				std::vector<app_overseer_t::callback_app_instance_t> on_close_given_rejected;
				std::vector<app_overseer_t::callback_void_t> on_close_all_completed;
				std::vector<app_overseer_t::callback_void_t> on_terminate_all_completed;

		};

		class runner_t
		{
			public:

				app_running_t * get_app(app_t::instance_id_t instance_id)
				{
					return apps_running[value_of(app_t::kind_t::_engine)].get_app(instance_id);
				}

				void add_app(std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<app_running_t> app_running)
				{
					apps_running[value_of(app_t::kind_t::_engine)].add_app(app_running);
					apps_running[value_of(app_running->get_app_meta()->get_app())].add_app(app_running);

					if(apps_running[value_of(app_t::kind_t::_engine)].is_being_terminated() || apps_running[value_of(app_running->get_app_meta()->get_app())].is_being_terminated())
					{
						app_running->terminate(actual, [](){});
					}
					else if(apps_running[value_of(app_t::kind_t::_engine)].is_being_closed() || apps_running[value_of(app_running->get_app_meta()->get_app())].is_being_closed())
					{
						auto instance_id = app_running->get_instance_id();
						auto & global = apps_running[value_of(app_t::kind_t::_engine)];
						auto & local = apps_running[value_of(app_running->get_app_meta()->get_app())];
						app_running->close(actual, [instance_id, &global, &local](){ global.given_closed(instance_id); local.given_closed(instance_id); }, [instance_id, &global, &local](){ global.given_not_closed(instance_id); local.given_not_closed(instance_id); });
					}
				}

				void remove_app(app_t::instance_id_t instance_id)
				{
					for(auto & item : apps_running)
					{
						item.remove_app(instance_id);
					}
				}

				bool is_empty()
				{
					return apps_running[value_of(app_t::kind_t::_engine)].is_empty();
				}

				bool is_empty_kind(app_t::kind_t kind)
				{
					return apps_running[value_of(kind)].is_empty();
				}

				void terminate_all(std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_all_accepted)
				{
					apps_running[value_of(app_t::kind_t::_engine)].terminate_all(actual, std::move(on_all_accepted));
				}

				void terminate_all_of(app_t::kind_t kind, std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_all_accepted)
				{
					apps_running[value_of(kind)].terminate_all(actual, std::move(on_all_accepted));
				}

				void close_all(std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_all_accepted, app_overseer_t::callback_app_instance_t on_given_accepted, app_overseer_t::callback_app_instance_t on_given_rejected)
				{
					apps_running[value_of(app_t::kind_t::_engine)].close_all(actual, std::move(on_all_accepted), std::move(on_given_accepted), std::move(on_given_rejected));
				}

				void close_all_of(app_t::kind_t kind, std::shared_ptr<app_overseer_actual_t> actual, app_overseer_t::callback_void_t on_all_accepted, app_overseer_t::callback_app_instance_t on_given_accepted, app_overseer_t::callback_app_instance_t on_given_rejected)
				{
					apps_running[value_of(kind)].close_all(actual, std::move(on_all_accepted), std::move(on_given_accepted), std::move(on_given_rejected));
				}

			private:
			
				std::array<kind_running_t, value_of(app_t::kind_t::count)> apps_running;
		};

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<app_interrupter_t> app_interrupter;
		std::shared_ptr<app_factory_t> app_factory;
		std::shared_ptr<instances_application_t> policy_instances_application;
		std::shared_ptr<program_instancer_t> program_instancer;
		std::size_t app_runner_id;
		runner_t runner;

		enum class flag_t
		{
			is_done,
			count
		};

		flags_t<flag_t> flags;
		std::mutex mutex_complete_run;

		void complete_run()
		{
			std::lock_guard<std::mutex> guard(mutex_complete_run);
			if(!flags.is_flag(flag_t::is_done))
			{
				flags.set_flag(flag_t::is_done, true);
				signal_completion.signal();
			}
		}

		void check_for_completion()
		{
			if(runner.is_empty() && events.is_empty())
			{
				complete_run();
			}
		}

		class context_holder_t
		{

			public:

				context_holder_t(std::unique_ptr<app_context_t> context) : context(std::move(context))
				{

				}

				std::unique_ptr<app_context_t> move_context()
				{
					return std::move(context);
				}

			private:

				std::unique_ptr<app_context_t> context;

		};

		std::shared_ptr<app_t> run_app_from_handler(app_t::kind_t kind, std::unique_ptr<app_context_t> context)
		{
			std::shared_ptr<app_t> app = app_factory->create(kind, std::move(context));

			std::thread holder(std::bind([](std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<app_t> app){
					app->get_meta()->wait_till_completed();
				}, shared_from_this(), app));

			try
			{
				holder.detach();
			}
			catch (const std::system_error& e) {}

			return app;
		}

		void run_app_actual(std::shared_ptr<app_overseer_actual_t> actual, app_t::kind_t kind, app_overseer_t::run_app_instance_t instance_policy, app_overseer_t::run_app_other_t other_policy, app_overseer_t::run_app_program_t program_policy, std::unique_ptr<app_context_t> context, app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance)
		{
			std::shared_ptr<context_holder_t> context_holder = std::make_shared<context_holder_t>(std::move(context));

			if(program_policy == app_overseer_t::run_app_program_t::force_new_instance)
			{
				bool success = program_instancer->run_new_program(kind);
				if(success)
					on_run_succeed_in_new_program_instance();
				else
					on_run_failed();
			}
			else
			{
				if((policy_instances_application->get_type() == instances_application_t::type_t::allow_multiple || 
					(policy_instances_application->get_type() == instances_application_t::type_t::force_single_given_kind && runner.is_empty_kind(kind)) ||
					(policy_instances_application->get_type() == instances_application_t::type_t::force_single && runner.is_empty()))
					&&
					(instance_policy == app_overseer_t::run_app_instance_t::allow_multiple || 
					(instance_policy == app_overseer_t::run_app_instance_t::force_single_given_kind && runner.is_empty_kind(kind)) ||
					(instance_policy == app_overseer_t::run_app_instance_t::force_single && runner.is_empty())))
						on_app_running(run_app_from_handler(kind, std::move(context_holder->move_context())));
				else
				{
					if(other_policy == app_overseer_t::run_app_other_t::keep)
					{
						if(program_policy == app_overseer_t::run_app_program_t::force_same_instance)
							on_run_failed();
						else
						{
							bool success = program_instancer->run_new_program(kind);
							if(success)
								on_run_succeed_in_new_program_instance();
							else
								on_run_failed();
						}

					}
					else if(other_policy == app_overseer_t::run_app_other_t::close_and_if_impossible_fail)
					{
						if(policy_instances_application->get_type() == instances_application_t::type_t::allow_multiple &&
							instance_policy == app_overseer_t::run_app_instance_t::allow_multiple)
						{
							if(program_policy == app_overseer_t::run_app_program_t::force_same_instance ||
									program_policy == app_overseer_t::run_app_program_t::if_possible_same_instance_if_not_new_instance)
							{
								on_app_running(run_app_from_handler(kind, std::move(context_holder->move_context())));
							}
						}
						else if(policy_instances_application->get_type() == instances_application_t::type_t::force_single_given_kind &&
							instance_policy == app_overseer_t::run_app_instance_t::force_single_given_kind)
						{
							
							close_all_of(kind, std::bind([kind, instance_policy, other_policy, program_policy](app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								actual->run_app_actual(actual, kind, instance_policy, other_policy, program_policy, std::move(context_holder->move_context()), std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance));
							}, std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance), shared_from_this(), context_holder),
							[](app_t::instance_id_t) {},
							std::bind([kind, instance_policy, other_policy, program_policy](app_t::instance_id_t instance, app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								
								if(program_policy == app_overseer_t::run_app_program_t::force_same_instance)
									on_run_failed();
								else if(program_policy == app_overseer_t::run_app_program_t::if_possible_same_instance_if_not_new_instance)
								{
									bool success = actual->program_instancer->run_new_program(kind);
									if(success)
										on_run_succeed_in_new_program_instance();
									else
										on_run_failed();
								}

							}, std::placeholders::_1, std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance), shared_from_this(), context_holder)
							);
						}
						else
						{
							close_all(std::bind([kind, instance_policy, other_policy, program_policy](app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								actual->run_app_actual(actual, kind, instance_policy, other_policy, program_policy, std::move(context_holder->move_context()), std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance));
							}, std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance), shared_from_this(), context_holder),
							[](app_t::instance_id_t) {},
							std::bind([kind, instance_policy, other_policy, program_policy](app_t::instance_id_t instance, app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								
								if(program_policy == app_overseer_t::run_app_program_t::force_same_instance)
									on_run_failed();
								else if(program_policy == app_overseer_t::run_app_program_t::if_possible_same_instance_if_not_new_instance)
								{
									bool success = actual->program_instancer->run_new_program(kind);
									if(success)
										on_run_succeed_in_new_program_instance();
									else
										on_run_failed();
								}

							}, std::placeholders::_1, std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance), shared_from_this(), context_holder)
							);
						}
					}
					else if(other_policy == app_overseer_t::run_app_other_t::terminate)
					{
						if(policy_instances_application->get_type() == instances_application_t::type_t::allow_multiple &&
							instance_policy == app_overseer_t::run_app_instance_t::allow_multiple)
						{
							if(program_policy == app_overseer_t::run_app_program_t::force_same_instance ||
									program_policy == app_overseer_t::run_app_program_t::if_possible_same_instance_if_not_new_instance)
							{
								on_app_running(run_app_from_handler(kind, std::move(context_holder->move_context())));
							}
						}
						else if(policy_instances_application->get_type() == instances_application_t::type_t::force_single_given_kind &&
							instance_policy == app_overseer_t::run_app_instance_t::force_single_given_kind)
						{
							terminate_all_of(kind, std::bind([kind, instance_policy, other_policy, program_policy](app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								actual->run_app_actual(actual, kind, instance_policy, other_policy, program_policy, std::move(context_holder->move_context()), std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance));
							}, std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance), shared_from_this(), context_holder));
						}
						else
						{
							terminate_all(std::bind([kind, instance_policy, other_policy, program_policy](app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								actual->run_app_actual(actual, kind, instance_policy, other_policy, program_policy, std::move(context_holder->move_context()), std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance));
							}, std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance), shared_from_this(), context_holder));
						}
					}
				}
			}
		}
		
		void handle_run_app_event(run_app_event_t * event)
		{
			run_app_actual(event->get_actual(), event->get_kind(), event->get_instance(), event->get_other(), event->get_program(), std::move(event->move_context()), std::move(event->move_on_app_running()), std::move(event->move_on_run_failed()), std::move(event->move_on_run_succeed_in_new_program_instance()));
		}
		
		void handle_terminate_app_event(terminate_app_event_t * event)
		{
			auto on_completed = event->move_on_completed();
			auto app_running = runner.get_app(event->get_instance());

			if(!app_running)
				on_completed();
			else
				app_running->terminate(event->get_actual(), std::move(on_completed));
		}

		void handle_close_app_event(close_app_event_t * event)
		{
			auto on_allowed = event->move_on_allowed();
			auto on_refused = event->move_on_refused();
			auto app_running = runner.get_app(event->get_instance());

			if(!app_running)
				on_allowed();
			else
				app_running->close(event->get_actual(), std::move(on_allowed), std::move(on_refused));
		}

		void handle_terminate_all_event(terminate_all_event_t * event)
		{
			runner.terminate_all(event->get_actual(), event->move_on_completed());
		}

		void handle_terminate_all_of_event(terminate_all_of_event_t * event)
		{
			runner.terminate_all_of(event->get_app(), event->get_actual(), event->move_on_completed());
		}

		void handle_close_all_event(close_all_event_t * event)
		{
			runner.close_all(event->get_actual(), event->move_on_all_accepted(), event->move_on_given_accepted(), event->move_on_given_rejected());
		}

		void handle_close_all_of_event(close_all_of_event_t * event)
		{
			runner.close_all_of(event->get_app(), event->get_actual(), event->move_on_all_accepted(), event->move_on_given_accepted(), event->move_on_given_rejected());
		}

		void handle_app_created_event(app_created_event_t * event)
		{
			runner.add_app(event->get_actual(), std::make_shared<app_running_t>(event->get_meta()));
		}

		void handle_app_destroyed_event(app_destroyed_event_t * event)
		{
			runner.remove_app(event->get_meta()->get_instance_id());
		}

		void handle_event(std::unique_ptr<event_t> event)
		{
			if(!flags.is_flag(flag_t::is_done))
			{
				switch(event->get_type())
				{

#define ENGINE_APP_OVERSEER_EVENT_TYPE_IMPL(type) case event_t::type_t::type: handle_##type##_event(static_cast<type##_event_t*>(event.get())); break;		
#define ENGINE_APP_OVERSEER_EVENT_TYPE_DEF(...) DEFINE_TYPE_PASS(ENGINE_APP_OVERSEER_EVENT_TYPE_IMPL, __VA_ARGS__)
#include "def/app_overseer.def"	

				}
				check_for_completion();
			}
		}

	};

	class app_overseer_service_t : public service_base_t
	{

	public:

		app_overseer_service_t(std::shared_ptr<logger_t> logger, std::shared_ptr<app_overseer_actual_t> actual) : logger(logger), actual(actual)
		{
			logger->log_global_msg(apps, "App overseer service has started"_u);
		}

		~app_overseer_service_t()
		{
			logger->log_global_msg(apps, "App overseer service has concluded"_u);
		}

		ustring_t get_name() const final
		{
			return "App overseer service"_u;
		}

		task_base_t::result_t run() final
		{
			return actual->run();
		}

		void on_end_requested() final
		{
			actual->terminate();
		}

	private:

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<app_overseer_actual_t> actual;

	};


	class app_overseer_real_t : public app_overseer_t
	{

	public:

		app_overseer_real_t(std::shared_ptr<logger_t> logger, std::unique_ptr<service_t<app_overseer_service_t>> service, std::shared_ptr<messenger_accountable_app_t> messenger_accountable_app, std::shared_ptr<app_overseer_actual_t> actual) : logger(logger), service(std::move(service)), messenger_accountable_app(messenger_accountable_app), actual(actual)
		{
			auto task_id = logger->log_global_task_start(apps, "Initializing overseer [app]"_u);
			messenger_instance = messenger_accountable_app->register_callback([this](messenger::msg_accountable_app_t* msg){on_accountable_changed(msg);}, messenger::history_t::dump_if_available);
			this->service->start();
			logger->log_global_task_done(task_id);
		}

		~app_overseer_real_t()
		{
			auto task_id = logger->log_global_task_start(apps, "App overseer is being disposed"_u);
			this->service->end();
			logger->log_global_task_done(task_id);
		}

		void run_app(app_t::kind_t kind, std::unique_ptr<app_context_t> context, run_app_instance_t instance = run_app_instance_t::allow_multiple, run_app_other_t other = run_app_other_t::keep, run_app_program_t program = run_app_program_t::if_possible_same_instance_if_not_new_instance, callback_app_t app_running = [](std::shared_ptr<app_t>){}, callback_void_t run_failed = [](){}, callback_void_t run_succeed_in_new_program_instance = [](){}) final
		{
			actual->run_app(kind, std::move(context), instance, other, program, std::move(app_running), std::move(run_failed), std::move(run_succeed_in_new_program_instance));
		}
        void terminate_app(app_t::instance_id_t instance, callback_void_t on_completed = [](){}) final
		{
			actual->terminate_app(instance, std::move(on_completed));
		}
        void close_app(app_t::instance_id_t instance, callback_void_t on_allowed = [](){}, callback_void_t on_refused = [](){}) final
		{
			actual->close_app(instance, std::move(on_allowed), std::move(on_refused));
		}
        void terminate_all_of(app_t::kind_t app, callback_void_t on_completed = [](){}) final
		{
			actual->terminate_all_of(app, std::move(on_completed));
		}
        void close_all_of(app_t::kind_t app, callback_void_t on_all_accepted = [](){}, callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){}) final
		{
			actual->close_all_of(app, std::move(on_all_accepted), std::move(on_given_accepted), std::move(on_given_rejected));
		}
        void terminate_all(callback_void_t on_completed = [](){}) final
		{
			actual->terminate_all(std::move(on_completed));
		}
        void close_all(callback_void_t on_all_accepted = [](){}, callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){}) final
		{
			actual->close_all(std::move(on_all_accepted), std::move(on_given_accepted), std::move(on_given_rejected));
		}
        void wait_for_completion() final
		{
			actual->wait_for_completion();
		}

	private:

		std::shared_ptr<logger_t> logger;
		std::unique_ptr<service_t<app_overseer_service_t>> service;
		std::shared_ptr<messenger_accountable_app_t> messenger_accountable_app;
 		std::shared_ptr<app_overseer_actual_t> actual;
		

		void on_accountable_created(messenger::msg_accountable_app_t * msg)
		{
			actual->app_created(msg->get_object()->get_meta());
		}

        void on_accountable_destroyed(messenger::msg_accountable_app_t * msg)
		{
			actual->app_destroyed(msg->get_object()->get_meta());
		}

		void on_accountable_changed(messenger::msg_accountable_app_t* msg)
		{
			switch(msg->get_type())
			{
				case messenger::accountable_type_t::create: 
                    on_accountable_created(msg);
				break;
				case messenger::accountable_type_t::destroy: 
                    on_accountable_destroyed(msg);
				break;
			}
		}

		std::unique_ptr<messenger::instance_t<messenger::msg_accountable_app_t> > messenger_instance;

	};
}

#endif