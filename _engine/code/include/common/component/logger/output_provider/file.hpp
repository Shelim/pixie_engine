#ifndef ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_FILE_HPP
#define ENGINE_COMPONENT_LOGGER_OUTPUT_PROVIDER_FILE_HPP
#pragma once

#include "component/logger/real.hpp"
#include "component/platform_internal.hpp"
#include <memory>
#include <cstdio>

namespace engine
{

	class logger_output_provider_file_t : public logger_output_provider_base_t
	{

	public:

		logger_output_provider_file_t(std::shared_ptr<platform_internal_t> platform_internal);
		~logger_output_provider_file_t();

		void output(const logger_item_t & item) final;

	private:

		std::shared_ptr<platform_internal_t> platform_internal;
		std::FILE * fp;

	};

}

#endif