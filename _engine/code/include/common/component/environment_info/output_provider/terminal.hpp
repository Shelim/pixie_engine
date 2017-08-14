#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_OUTPUT_PROVIDER_TERMINAL_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_OUTPUT_PROVIDER_TERMINAL_HPP
#pragma once

#include "component/environment_info/real.hpp"
#include "component/terminal_writer.hpp"
#include "component/log_file_writer.hpp"
#include "provider/save_location.hpp"
#include "provider/common_filenames.hpp"
#include "utility/text/parser.hpp"
#include <memory>
#include <cstdio>

namespace engine
{

	class environment_info_output_provider_terminal_t : public environment_info_output_provider_base_t
	{

	public:

		environment_info_output_provider_terminal_t(std::shared_ptr<terminal_writer_t> terminal_writer, std::unique_ptr<settings_t<environment_info_output_t>> settings);

		void output_start() const final;
		void output(environment_info_t::key_t key, environment_info_t::status_t status, const ustring_t & value) const final;
		void output_end() const final;

	private:

		terminal_writer_string_t start_text;
		terminal_writer_string_t end_text;
		std::array<formattable_string_t, value_of(environment_info_t::key_t::count)> formattable_string;


		std::shared_ptr<terminal_writer_t> terminal_writer;

		std::unique_ptr<settings_t<environment_info_output_t>> settings;

	};

}

#endif