#ifndef ENGINE_COMPONENT_APP_OVERSEER_REAL_HPP
#define ENGINE_COMPONENT_APP_OVERSEER_REAL_HPP
#pragma once

#include "global/component/app_overseer.hpp"
#include "global/core/messenger.hpp"
#include "global/component/logger.hpp"
#include "global/core/process/service.hpp"
#include "utility/container/concurrent_queue.hpp"
#include "utility/concurrention/signal.hpp"

namespace engine
{

	class app_overseer_actual_t
	{

	public:

		app_overseer_actual_t(std::shared_ptr<logger_t> logger) : logger(logger)
		{
			
		}

		void run_app(app_t::kind_t kind, std::unique_ptr<app_context_t> context, app_overseer_t::run_app_instance_t instance = app_overseer_t::run_app_instance_t::allow_multiple, app_overseer_t::run_app_other_t other = app_overseer_t::run_app_other_t::keep, app_overseer_t::callback_app_t app_running = [](std::shared_ptr<app_t>){}, app_overseer_t::callback_void_t run_failed = [](){})
		{
			events.push(std::make_unique<run_app_event_t>(kind, std::move(context), instance, other, std::move(app_running), std::move(run_failed)));
		}

        void terminate_app(app_t::instance_id_t instance, app_overseer_t::callback_void_t on_completed = [](){})
		{
			events.push(std::make_unique<terminate_app_event_t>(instance, std::move(on_completed)));
		}

        void close_app(app_t::instance_id_t instance, app_overseer_t::callback_void_t on_accepted = [](){}, app_overseer_t::callback_void_t on_rejected = [](){})
		{
			events.push(std::make_unique<close_app_event_t>(instance, std::move(on_accepted), std::move(on_rejected)));
		}

        void wait_for_completion()
		{
			check_for_completion();
			signal_completion.wait();
		}

		void app_created(std::shared_ptr<app_meta_t> app_meta)
		{
			events.push(std::make_unique<app_created_event_t>(app_meta));
		}

		void app_destroyed(std::shared_ptr<app_meta_t> app_meta)
		{
			events.push(std::make_unique<app_destroyed_event_t>(app_meta));
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
				if(!event) return task_base_t::result_t::completed;
				handle_event(std::move(event));
			}
		}

	private:

		signal_t signal_completion;

		class event_t
		{
			public:

				virtual ~event_t()
				{

				}

				enum class type_t
				{
					run_app,
					terminate_app,
					close_app,
					app_created,
					app_destroyed
				};

				type_t get_type() const
				{
					return type;
				}

			protected:

				event_t(type_t type) : type(type)
				{

				}

			private:

				type_t type;
		};

		concurrent_queue_t<std::unique_ptr<event_t> > events;

		class run_app_event_t : public event_t
		{

			public:

				run_app_event_t(app_t::kind_t kind, std::unique_ptr<app_context_t> context, app_overseer_t::run_app_instance_t instance = app_overseer_t::run_app_instance_t::allow_multiple, app_overseer_t::run_app_other_t other = app_overseer_t::run_app_other_t::keep, app_overseer_t::callback_app_t app_running = [](std::shared_ptr<app_t>){}, app_overseer_t::callback_void_t run_failed = [](){}) :
					event_t(type_t::run_app), kind(kind), context(std::move(context)), instance(instance), other(other), app_running(std::move(app_running)), run_failed(std::move(run_failed))
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
				
				void execute_callback_app_running(std::shared_ptr<app_t> app)
				{
					app_running(app);
				}

				void execute_callback_run_failed()
				{
					run_failed();
				}

			private:

				app_t::kind_t kind;
				std::unique_ptr<app_context_t> context;
				app_overseer_t::run_app_instance_t instance;
				app_overseer_t::run_app_other_t other;
				app_overseer_t::callback_app_t app_running;
				app_overseer_t::callback_void_t run_failed;

		};

		class terminate_app_event_t : public event_t
		{

			public:

				terminate_app_event_t(app_t::instance_id_t instance, app_overseer_t::callback_void_t on_completed = [](){}) : event_t(type_t::terminate_app), instance(instance), on_completed(std::move(on_completed))
				{

				}

				app_t::instance_id_t get_instance() const
				{
					return instance;
				}

				void execute_callback_on_completed()
				{
					on_completed();
				}

			private:

				app_t::instance_id_t instance;
				app_overseer_t::callback_void_t on_completed;
		};
		
			

		class close_app_event_t : public event_t
		{

			public:

				close_app_event_t(app_t::instance_id_t instance, app_overseer_t::callback_void_t on_accepted = [](){}, app_overseer_t::callback_void_t on_rejected = [](){}) : event_t(type_t::close_app), instance(instance), on_accepted(std::move(on_accepted)), on_rejected(std::move(on_rejected))
				{

				}

				app_t::instance_id_t get_instance() const
				{
					return instance;
				}

				void execute_callback_on_accepted()
				{
					on_accepted();
				}

				void execute_callback_on_rejected()
				{
					on_rejected();
				}

			private:

				app_t::instance_id_t instance;
				app_overseer_t::callback_void_t on_accepted;
				app_overseer_t::callback_void_t on_rejected;
		};

		
		class app_created_event_t : public event_t
		{

			public:

				app_created_event_t(std::shared_ptr<app_meta_t> meta) : event_t(type_t::app_created), meta(meta)
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

				app_destroyed_event_t(std::shared_ptr<app_meta_t> meta) : event_t(type_t::app_destroyed), meta(meta)
				{

				}

				std::shared_ptr<app_meta_t> get_meta()
				{
					return meta;
				}

			private:

				std::shared_ptr<app_meta_t> meta;
		};

		std::shared_ptr<logger_t> logger;
		std::vector<std::shared_ptr<app_meta_t> > apps_meta;

		enum class flag_t
		{
			is_done,
			count
		};

		flags_t<flag_t> flags;

		void check_for_completion()
		{
			if(apps_meta.empty() && events.is_empty())
			{
				flags.set_flag(flag_t::is_done, true);
				signal_completion.signal();
			}
		}

		void handle_app_created_event(app_created_event_t * event)
		{
			auto iter = std::find(apps_meta.begin(), apps_meta.end(), event->get_meta());
			if(iter == apps_meta.end())
				apps_meta.push_back(event->get_meta());
		}

		void handle_app_destroyed_event(app_destroyed_event_t * event)
		{
			auto iter = std::find(apps_meta.begin(), apps_meta.end(), event->get_meta());
			if(iter != apps_meta.end())
				apps_meta.erase(iter);
		}

		void handle_event(std::unique_ptr<event_t> event)
		{
			if(!flags.is_flag(flag_t::is_done))
			{
				switch(event->get_type())
				{
					case event_t::type_t::app_created:
						handle_app_created_event(static_cast<app_created_event_t*>(event.get()));
					break;
					case event_t::type_t::app_destroyed:
						handle_app_destroyed_event(static_cast<app_destroyed_event_t*>(event.get()));
					break;
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

		app_overseer_real_t(std::shared_ptr<logger_t> logger, std::unique_ptr<service_t<app_overseer_service_t>> service, std::shared_ptr<messenger_accountable_app_t> messenger_accountable_app, std::shared_ptr<app_overseer_actual_t> actual) : logger(logger), service(std::move(service)), messenger_accountable_app(messenger_accountable_app)
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

		void run_app(app_t::kind_t kind, std::unique_ptr<app_context_t> context, run_app_instance_t instance = run_app_instance_t::allow_multiple, run_app_other_t other = run_app_other_t::keep, callback_app_t app_running = [](std::shared_ptr<app_t>){}, callback_void_t run_failed = [](){}) final
		{
			actual->run_app(kind, std::move(context), instance, other, std::move(app_running), std::move(run_failed));
		}
        void terminate_app(app_t::instance_id_t instance, callback_void_t on_completed = [](){}) final
		{
			actual->terminate_app(instance, std::move(on_completed));
		}
        void close_app(app_t::instance_id_t instance, callback_void_t on_accepted = [](){}, callback_void_t on_rejected = [](){}) final
		{
			actual->close_app(instance, std::move(on_accepted), std::move(on_rejected));
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