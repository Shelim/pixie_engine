#ifndef ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_FILE_HPP
#define ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_FILE_HPP
#pragma once

#include "component/logger/real.hpp"
#include "provider/save_location.hpp"
#include "provider/common_filenames.hpp"
#include "utility/text/parser.hpp"
#include <memory>
#include <cstdio>

namespace engine
{

	class logger_output_provider_file_t : public logger_output_provider_base_t
	{

	public:

		logger_output_provider_file_t(std::shared_ptr<save_location_provider_t> save_location_provider, std::shared_ptr<common_filenames_provider_t> common_filenames_provider, std::unique_ptr<settings_t<logger_output_t>> settings);
		~logger_output_provider_file_t();

		void output(const logger_item_t & item) const final;

	private:

		std::array<formattable_string_t, static_cast<std::underlying_type<logger_item_t::level_t>::type>(logger_item_t::level_t::count)> formattable_string;

		std::shared_ptr<save_location_provider_t> save_location_provider;
		std::shared_ptr<common_filenames_provider_t> common_filenames_provider;
		mutable std::recursive_mutex fp_mutex;

		std::FILE * fp;

		std::unique_ptr<settings_t<logger_output_t>> settings;
	};

}

#endif