#ifndef ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_FILE_HPP
#define ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_FILE_HPP
#pragma once

#include "component/logger/real.hpp"
#include "provider/save_location.hpp"
#include "provider/common_filenames.hpp"
#include "utility/text/parser.hpp"
#include <memory>
#include <cstdio>
#include <array>

namespace engine
{

	class logger_output_provider_file_t : public logger_output_provider_base_t
	{

	public:

		logger_output_provider_file_t(std::shared_ptr<log_file_writer_t> log_file_writer, std::unique_ptr<settings_t<logger_output_t>> settings);
		~logger_output_provider_file_t();

		void output_start() const final;
		void output(const logger_item_t & item) const final;
		void output_end() const final;

	private:

		ustring_t start_text;
		ustring_t end_text;
		std::array<formattable_string_t, to_value(logger_item_t::level_t::count)> formattable_string;

		std::shared_ptr<log_file_writer_t> log_file_writer;

		std::unique_ptr<settings_t<logger_output_t>> settings;
	};

}

#endif