#ifndef ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_TERMINAL_HPP
#define ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_TERMINAL_HPP
#pragma once

#include "component/logger/real.hpp"
#include "component/terminal_output.hpp"
#include <memory>

namespace engine
{

	class logger_output_provider_terminal_t : public logger_output_provider_base_t
	{

	public:

		logger_output_provider_terminal_t(std::shared_ptr<terminal_output_t> terminal_output, PTR_TO_SETTINGS_FOR(logger_output_t) logger_output);


	private:

		void output(const richtext_t & item) final;
		ustring_t format_provider(const logger_item_t & item) final;

		std::shared_ptr<terminal_output_t> terminal_output;

		PTR_TO_SETTINGS_FOR(logger_output_t) logger_output;

	};

}

#endif