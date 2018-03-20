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

		void run_app(app_t::kind_t kind, std::unique_ptr<app_context_t> context, run_app_instance_t instance = run_app_instance_t::allow_multiple, run_app_other_t other = run_app_other_t::keep, callback_app_t app_running = [](std::shared_ptr<app_t>){}, callback_void_t run_failed = [](){}) final
		{
			
		}
        void terminate_app(app_t::instance_id_t instance, callback_void_t on_completed = [](){}) final
		{
			
		}
        void close_app(app_t::instance_id_t instance, callback_void_t on_accepted = [](){}, callback_void_t on_rejected = [](){}) final
		{
			
		}
        void wait_for_completion() final
		{

		}

	private:

	};
}

#endif