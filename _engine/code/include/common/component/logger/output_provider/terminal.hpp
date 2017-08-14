#ifndef ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_TERMINAL_HPP
#define ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_TERMINAL_HPP
#pragma once

#include "component/logger/real.hpp"
#include "component/terminal_writer.hpp"
#include <memory>

namespace engine
{

	class logger_output_provider_terminal_t : public logger_output_provider_base_t
	{

	public:

		logger_output_provider_terminal_t(std::shared_ptr<terminal_writer_t> terminal_writer, std::unique_ptr<settings_t<logger_output_t>> settings);
		~logger_output_provider_terminal_t();

		void output_start() const final;
		void output(const logger_item_t & item) const final;
		void output_end() const final;

	private:

		terminal_writer_string_t start_text;
		terminal_writer_string_t end_text;
		std::array<formattable_string_t, value_of(logger_item_t::level_t::count)> formattable_string;


		std::shared_ptr<terminal_writer_t> terminal_writer;

		std::unique_ptr<settings_t<logger_output_t>> settings;

	};

}

#endif