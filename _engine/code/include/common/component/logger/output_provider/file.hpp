#ifndef ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_FILE_HPP
#define ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_FILE_HPP
#pragma once

#include "component/logger/real.hpp"
#include "provider/save_location.hpp"
#include "provider/common_filenames.hpp"
#include <memory>
#include <cstdio>

namespace engine
{

	class logger_output_provider_file_t : public logger_output_provider_base_t
	{

	public:

		logger_output_provider_file_t(std::shared_ptr<save_location_provider_t> save_location_provider, std::shared_ptr<common_filenames_provider_t> common_filenames_provider, PTR_TO_SETTINGS_FOR(logger_output_t) logger_output, PTR_TO_SETTINGS_FOR(richtext_colors_t) richtext_colors);
		~logger_output_provider_file_t();

	private:

		void output(const richtext_t & item) final;
		ustring_t format_provider(const logger_item_t & item) final;

		std::shared_ptr<save_location_provider_t> save_location_provider;
		std::shared_ptr<common_filenames_provider_t> common_filenames_provider;
		mutable std::recursive_mutex fp_mutex;

		std::FILE * fp;

		PTR_TO_SETTINGS_FOR(logger_output_t) logger_output;
		PTR_TO_SETTINGS_FOR(richtext_colors_t) richtext_colors;
	};

}

#endif