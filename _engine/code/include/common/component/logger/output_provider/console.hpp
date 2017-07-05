#ifndef ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_CONSOLE_HPP
#define ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_CONSOLE_HPP
#pragma once

#include "component/logger/real.hpp"
#include "component/platform_internal.hpp"
#include <memory>

namespace engine
{

	class logger_output_provider_console_t : public logger_output_provider_base_t
	{

	public:

		logger_output_provider_console_t(std::shared_ptr<platform_internal_t> platform_internal);

		void output(const logger_item_t & item) final;

	private:

		std::shared_ptr<platform_internal_t> platform_internal;

	};

}

#endif