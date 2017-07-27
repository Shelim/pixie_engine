#ifndef ENGINE_COMPONENT_LOG_FILE_OUTPUT_HPP
#define ENGINE_COMPONENT_LOG_FILE_OUTPUT_HPP
#pragma once

#include "utility/vfs/filesystem.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/vfs/virtual_path.hpp"
#include <mutex>

namespace engine
{

	class log_file_writer_t
	{

	public:

		log_file_writer_t()
		{

		}

		virtual ~log_file_writer_t()
		{

		}

		virtual void write(const ustring_t & line) = 0;

	private:

	};
}
#include "component/log_file_writer/dummy.hpp"
#include "component/log_file_writer/real.hpp"


#endif