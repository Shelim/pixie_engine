#ifndef ENGINE_COMPONENT_LOG_FILE_OUTPUT_REAL_HPP
#define ENGINE_COMPONENT_LOG_FILE_OUTPUT_REAL_HPP
#pragma once

#include "platform/terminal.hpp""
#include "component/log_file_writer.hpp"
#include "provider/save_location.hpp"
#include "provider/common_filenames.hpp"

namespace engine
{

	class log_file_writer_real_t : public log_file_writer_t
	{

	public:

		log_file_writer_real_t(std::shared_ptr<save_location_provider_t> save_location_provider, std::shared_ptr<common_filenames_provider_t> common_filenames_provider);

		~log_file_writer_real_t();

		void write(const ustring_t & line) final;


	private:

		std::shared_ptr<save_location_provider_t> save_location_provider;
		std::shared_ptr<common_filenames_provider_t> common_filenames_provider; 
		mutable std::recursive_mutex fp_mutex;

		std::FILE * fp;

	};
}

#endif