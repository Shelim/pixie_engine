#ifndef ENGINE_COMPONENT_LOGGER_FILE_OUTPUT_REAL_HPP
#define ENGINE_COMPONENT_LOGGER_FILE_OUTPUT_REAL_HPP
#pragma once

#include "component/logger_file_writer.hpp"
#include "core/data/output.hpp"
#include "utility/platform/filesystem.hpp"

namespace engine
{

	class logger_file_writer_real_t : public logger_file_writer_t
	{

	public:

		logger_file_writer_real_t(std::shared_ptr<platform::filesystem_t> platform_filesystem);

		~logger_file_writer_real_t();

		void write(const ustring_t & line) final;


	private:

		mutable std::recursive_mutex fp_mutex;

		std::shared_ptr<platform::filesystem_t::file_output_t> output;

	};
}

#endif