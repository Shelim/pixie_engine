#ifndef ENGINE_COMPONENT_APP_INTERRUPTER_DUMMY_HPP
#define ENGINE_COMPONENT_APP_INTERRUPTER_DUMMY_HPP
#pragma once

#include "global/component/app_interrupter.hpp"

namespace engine
{

	class app_interrupter_dummy_t : public app_interrupter_t
	{

	public:

		app_interrupter_dummy_t()
		{

		}

		std::unique_ptr<instance_t> register_handler(app_t::kind_t app,app_context_t* context, priority_t priority, handler_t handler) final
		{
			return std::make_unique<instance_t>();
		}

        void send_interruption(std::unique_ptr<interruption_t> interruption) final
		{

		}

	private:

	};
}

#endif