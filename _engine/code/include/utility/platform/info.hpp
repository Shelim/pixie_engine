#ifndef ENGINE_COMMON_UTILITY_PLATFORM_INFO_HPP
#define ENGINE_COMMON_UTILITY_PLATFORM_INFO_HPP
#pragma once

#include<memory>
#include "utility/text/ustring.hpp"
#include "utility/platform/cpu_features.hpp"
namespace engine
{


	namespace platform
	{

		/**
		 * @brief Gets fullname of current Operating System
		 * 
		 * @note This function is intended for logging and debugging purposes
		 * 
		 * @return Platform-specific OS name
		 */
		ustring_t get_os_version();

		/**
		 * @brief Get cache line size for given platform
		 * 
		 * @return Cache-line size 
		 */
		uint32_t get_cache_line_size();

		/**
		 * @brief Get total RAM size in megabytes
		 * 
		 * @return Total RAM size in megabytes
		 */
		uint32_t get_total_ram_in_mb();

		/**
		 * @brief List CPU features
		 * 
		 * @return Platform-specifc CPU features
		 */
		cpu_features_t get_cpu_features();

	}


}

#include "utility/debug/callstack.hpp"

#endif