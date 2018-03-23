#ifndef ENGINE_COMPONENT_APP_OVERSEER_DUMMY_HPP
#define ENGINE_COMPONENT_APP_OVERSEER_DUMMY_HPP
#pragma once

#include "global/component/app_overseer.hpp"

namespace engine
{

	class app_overseer_dummy_t : public app_overseer_t
	{

	public:

		app_overseer_dummy_t()
		{

		}

		void run_app(app_t::kind_t kind, std::unique_ptr<app_context_t> context, run_app_instance_t instance = run_app_instance_t::allow_multiple, run_app_other_t other = run_app_other_t::keep, run_app_program_t program = run_app_program_t::if_possible_same_instance_if_not_new_instance, callback_app_t app_running = [](std::shared_ptr<app_t>){}, callback_void_t run_failed = [](){}, callback_void_t run_succeed_in_new_program_instance = [](){}) final
		{
			
		}
        void terminate_app(app_t::instance_id_t instance, callback_void_t on_completed = [](){}) final
		{
			
		}
        void close_app(app_t::instance_id_t instance, callback_void_t on_accepted = [](){}, callback_void_t on_rejected = [](){}) final
		{
			
		}
        void terminate_all_of(app_t::kind_t app, callback_void_t on_completed = [](){}) final
		{
			
		}
        void close_all_of(app_t::kind_t app, callback_void_t on_all_accepted = [](){}, callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){}) final
		{
			
		}
        void terminate_all(callback_void_t on_completed = [](){}) final
		{
			
		}
        void close_all(callback_void_t on_all_accepted = [](){}, callback_app_instance_t on_given_accepted = [](app_t::instance_id_t){}, callback_app_instance_t on_given_rejected = [](app_t::instance_id_t){}) final
		{
			
		}
        void wait_for_completion() final
		{

		}

	private:

	};
}

#endif