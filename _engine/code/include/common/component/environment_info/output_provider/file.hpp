#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_OUTPUT_PROVIDER_FILE_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_OUTPUT_PROVIDER_FILE_HPP
#pragma once

#include "component/environment_info/real.hpp"
#include "component/log_file_writer.hpp"
#include "provider/save_location.hpp"
#include "provider/common_filenames.hpp"
#include "utility/text/parser.hpp"
#include <memory>
#include <cstdio>

namespace engine
{

	class environment_info_output_provider_file_t : public environment_info_output_provider_base_t
	{

	public:

		environment_info_output_provider_file_t(std::shared_ptr<log_file_writer_t> log_file_writer, std::unique_ptr<settings_t<environment_info_output_t>> settings);
		~environment_info_output_provider_file_t();

		void output_start() const final;
		void output(environment_info_t::key_t key, environment_info_t::status_t status, const ustring_t & value) const final;
		void output_end() const final;

	private:
		ustring_t start_text;
		ustring_t end_text;
		std::array<formattable_string_t, static_cast<std::underlying_type<environment_info_t::key_t>::type>(environment_info_t::key_t::count)> formattable_string;
		
		std::shared_ptr<log_file_writer_t> log_file_writer;

		std::unique_ptr<settings_t<environment_info_output_t>> settings;
	};

}

#endif