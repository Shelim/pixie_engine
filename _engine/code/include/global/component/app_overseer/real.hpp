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

		app_overseer_actual_t(std::shared_ptr<logger_t> logger, std::shared_ptr<app_interrupter_t> app_interrupter, std::shared_ptr<process::runner_spawn_factory_t> runner_spawn_factory, std::shared_ptr<app_factory_t> app_factory, std::shared_ptr<instances_application_t> policy_instances_application, std::shared_ptr<program_instancer_t> program_instancer) : logger(logger), app_interrupter(app_interrupter), runner_spawn_factory(runner_spawn_factory), app_factory(app_factory), policy_instances_application(policy_instances_application), program_instancer(program_instancer), termination_runner_id(0), app_runner_id(0)
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
			check_for_completion();
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
					std::move(on_completed);
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
					std::move(on_completed);
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
					std::move(on_completed);
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

		std::shared_ptr<logger_t> logger;
		std::shared_ptr<app_interrupter_t> app_interrupter;
		std::shared_ptr<process::runner_spawn_factory_t> runner_spawn_factory;
		std::shared_ptr<app_factory_t> app_factory;
		std::shared_ptr<instances_application_t> policy_instances_application;
		std::shared_ptr<program_instancer_t> program_instancer;
		std::size_t termination_runner_id;
		std::size_t app_runner_id;
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

		std::shared_ptr<app_t> run_app_from_handler(app_t::kind_t kind, std::unique_ptr<app_context_t> context)
		{
			// ToDo: Send signal to all instnaces of kind if we can instantiate this kind
			std::shared_ptr<app_t> app = app_factory->create(kind, std::move(context));

			std::unique_ptr<process::runner_spawn_t> runner_spawn = runner_spawn_factory->create(app->get_meta()->get_app(), app->get_meta()->get_instance_id(), format_string("App runner ###1#"_u, ++app_runner_id));
				
			runner_spawn->add_task(std::make_unique<task_func_t>(std::bind([](process::token_t* token, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<app_t> app){
					app->get_meta()->wait_till_completed();
					return task_base_t::result_t::completed;
				}, std::placeholders::_1, shared_from_this(), app), format_string("Running #1####2#"_u, app->get_meta()->get_app(), app->get_meta()->get_instance_id())));

			return app;
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

		void run_app_internal(app_t::kind_t kind, app_overseer_t::run_app_instance_t instance_policy, app_overseer_t::run_app_other_t other_policy, app_overseer_t::run_app_program_t program_policy, std::unique_ptr<app_context_t> context, app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance)
		{
			std::shared_ptr<context_holder_t> context_holder = std::make_shared<context_holder_t>(std::move(context));
			std::vector<std::shared_ptr<app_meta_t> > apps_meta_running;
			apps_meta_running.reserve(apps_meta.size());

			for(auto app_meta : apps_meta)
			{
				if(app_meta->get_app() == kind)
				{
					apps_meta_running.push_back(app_meta);
				}
			}

			if((policy_instances_application->get_type() == instances_application_t::type_t::allow_multiple || 
				(policy_instances_application->get_type() == instances_application_t::type_t::force_single_given_kind && apps_meta_running.empty()) ||
				(policy_instances_application->get_type() == instances_application_t::type_t::force_single && apps_meta.empty()))
				&&
				(instance_policy == app_overseer_t::run_app_instance_t::allow_multiple || 
				(instance_policy == app_overseer_t::run_app_instance_t::force_single_given_kind && apps_meta_running.empty()) ||
				(instance_policy == app_overseer_t::run_app_instance_t::force_single && apps_meta.empty())))
			{
				if(program_policy == app_overseer_t::run_app_program_t::force_new_instance)
				{
					bool success = program_instancer->run_new_program(kind);
					if(success)
						on_run_succeed_in_new_program_instance();
					else
						on_run_failed();
				}
				else
					on_app_running(run_app_from_handler(kind, std::move(context_holder->move_context())));
			}
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
						if(program_policy == app_overseer_t::run_app_program_t::force_new_instance)
						{
							bool success = program_instancer->run_new_program(kind);
							if(success)
								on_run_succeed_in_new_program_instance();
							else
								on_run_failed();
						}
						else if(program_policy == app_overseer_t::run_app_program_t::force_same_instance ||
						 		program_policy == app_overseer_t::run_app_program_t::if_possible_same_instance_if_not_new_instance)
						{
							on_app_running(run_app_from_handler(kind, std::move(context_holder->move_context())));
						}
					}
					else if(policy_instances_application->get_type() == instances_application_t::type_t::force_single_given_kind &&
						instance_policy == app_overseer_t::run_app_instance_t::force_single_given_kind)
					{
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
							close_all_of(kind, std::bind([kind, instance_policy, other_policy, program_policy](app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								actual->run_app_internal(kind, instance_policy, other_policy, program_policy, std::move(context_holder->move_context()), std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance));
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
					else
					{
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
							close_all(std::bind([kind, instance_policy, other_policy, program_policy](app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								actual->run_app_internal(kind, instance_policy, other_policy, program_policy, std::move(context_holder->move_context()), std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance));
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
				}
				else if(other_policy == app_overseer_t::run_app_other_t::terminate)
				{
					if(policy_instances_application->get_type() == instances_application_t::type_t::allow_multiple &&
						instance_policy == app_overseer_t::run_app_instance_t::allow_multiple)
					{
						if(program_policy == app_overseer_t::run_app_program_t::force_new_instance)
						{
							bool success = program_instancer->run_new_program(kind);
							if(success)
								on_run_succeed_in_new_program_instance();
							else
								on_run_failed();
						}
						else if(program_policy == app_overseer_t::run_app_program_t::force_same_instance ||
						 		program_policy == app_overseer_t::run_app_program_t::if_possible_same_instance_if_not_new_instance)
						{
							on_app_running(run_app_from_handler(kind, std::move(context_holder->move_context())));
						}
					}
					else if(policy_instances_application->get_type() == instances_application_t::type_t::force_single_given_kind &&
						instance_policy == app_overseer_t::run_app_instance_t::force_single_given_kind)
					{
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
							terminate_all_of(kind, std::bind([kind, instance_policy, other_policy, program_policy](app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								actual->run_app_internal(kind, instance_policy, other_policy, program_policy, std::move(context_holder->move_context()), std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance));
							}, std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance), shared_from_this(), context_holder));
						}
					}
					else
					{
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
							terminate_all(std::bind([kind, instance_policy, other_policy, program_policy](app_overseer_t::callback_app_t on_app_running, app_overseer_t::callback_void_t on_run_failed, app_overseer_t::callback_void_t on_run_succeed_in_new_program_instance, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<context_holder_t> context_holder){
								actual->run_app_internal(kind, instance_policy, other_policy, program_policy, std::move(context_holder->move_context()), std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance));
							}, std::move(on_app_running), std::move(on_run_failed), std::move(on_run_succeed_in_new_program_instance), shared_from_this(), context_holder));
						}
					}
				}
			}
		}
		
		void handle_run_app_event(run_app_event_t * event)
		{
			run_app_internal(event->get_kind(), event->get_instance(), event->get_other(), event->get_program(), std::move(event->move_context()), std::move(event->move_on_app_running()), std::move(event->move_on_run_failed()), std::move(event->move_on_run_succeed_in_new_program_instance()));
		}
		
		void handle_terminate_app_event(terminate_app_event_t * event)
		{
			auto instance_id = event->get_instance();
			auto iter = std::find_if(apps_meta.begin(), apps_meta.end(), [instance_id](std::shared_ptr<app_meta_t> app_meta){ return app_meta->get_instance_id() == instance_id; });
			if(iter == apps_meta.end())
				event->move_on_completed()();
			else
			{
				app_interrupter->send_interruption(std::make_unique<interruption_terminate_t>(event->get_instance()));
				std::unique_ptr<process::runner_spawn_t> runner_spawn = runner_spawn_factory->create((*iter)->get_app(), (*iter)->get_instance_id(), format_string("Termination runner ###1#"_u, ++termination_runner_id));
				
				runner_spawn->add_task(std::make_unique<task_func_t>(std::bind([](process::token_t* token, app_overseer_t::callback_void_t on_completed, std::shared_ptr<app_overseer_actual_t> actual, std::shared_ptr<app_meta_t> app_meta){
					app_meta->wait_till_completed();
					on_completed();
					return task_base_t::result_t::completed;
				}, std::placeholders::_1, std::move(event->move_on_completed()), event->get_actual(), *iter), format_string("Waiting for termination of #1####2#"_u, (*iter)->get_app(), (*iter)->get_instance_id())));

			}
		}

		void handle_close_app_event(close_app_event_t * event)
		{
			app_interrupter->send_interruption(std::make_unique<interruption_ask_for_close_t>(event->get_instance(), std::bind([](app_overseer_t::callback_void_t on_allowed, std::shared_ptr<app_overseer_actual_t> actual, app_t::instance_id_t instance){
				actual->terminate_app(instance, std::move(on_allowed));
			}, std::move(event->move_on_allowed()), event->get_actual(), event->get_instance()), std::bind([](app_overseer_t::callback_void_t on_refused, std::shared_ptr<app_overseer_actual_t> actual){
				on_refused();
			}, std::move(event->move_on_refused()), event->get_actual())));
		}

		void handle_terminate_all_event(terminate_all_event_t * event)
		{
			// Todo!
		}

		void handle_close_all_event(close_all_event_t * event)
		{
			// Todo!
		}

		void handle_terminate_all_of_event(terminate_all_of_event_t * event)
		{
			// Todo!
		}

		void handle_close_all_of_event(close_all_of_event_t * event)
		{
			// Todo!
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