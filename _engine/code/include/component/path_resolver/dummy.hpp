#ifndef ENGINE_COMPONENT_LOGGER_FILE_OUTPUT_DUMMY_HPP
#define ENGINE_COMPONENT_LOGGER_FILE_OUTPUT_DUMMY_HPP
#pragma once

#include "component/logger_file_writer.hpp"

namespace engine
{

	class logger_file_writer_dummy_t : public logger_file_writer_t
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