#ifndef ENGINE_COMPONENT_LOG_FILE_OUTPUT_DUMMY_HPP
#define ENGINE_COMPONENT_LOG_FILE_OUTPUT_DUMMY_HPP
#pragma once

#include "component/log_file_writer.hpp"

namespace engine
{

	class log_file_writer_dummy_t : public log_file_writer_t
	{

	public:

		void write(const ustring_t & line) final
		{
			// Do nothing, it is dummy.
		}

	private:

	};
}

#endif