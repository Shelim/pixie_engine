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

		logger_output_provider_terminal_t(std::shared_ptr<terminal_output_t> terminal_output, std::unique_ptr<settings_t<logger_output_t>> settings);

		void output(const logger_item_t & item) const final;

	private:

		std::array<formattable_string_t, static_cast<std::underlying_type<logger_item_t::level_t>::type>(logger_item_t::level_t::count)> formattable_string;


		std::shared_ptr<terminal_output_t> terminal_output;

		std::unique_ptr<settings_t<logger_output_t>> settings;

	};

}

#endif