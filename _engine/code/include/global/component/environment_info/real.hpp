#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_REAL_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_REAL_HPP
#pragma once

#include "global/component/environment_info.hpp"
#include "utility/pattern/enum.hpp"
#include "global/core/manifest/version.hpp"
#include "utility/platform/info.hpp"
#include <array>

namespace engine
{
	
	class environment_info_real_t : public environment_info_t
	{

	public:

		environment_info_real_t(std::shared_ptr<manifest_version_t> manifest_version, std::shared_ptr<manifest_os_t> manifest_os)
		{
			this->set_engine_version(manifest_version->get_version());
			this->set_launched_time(std::chrono::system_clock::now());
			this->set_os_name(manifest_os->get_os());
			this->set_os_ver(platform::get_os_version());
			
			uint32_t cpu_cores = std::thread::hardware_concurrency();
			if(cpu_cores > 0)
				this->set_cpu_cores(cpu_cores);
			else
				this->set_cpu_cores(cpu_cores, status_t::bad);

			this->set_cpu_cache_line(platform::get_cache_line_size());
			this->set_cpu_features(platform::get_cpu_features());
			this->set_ram(platform::get_total_ram_in_mb());
		}

		ustring_t get(type_t key) const final
		{
#define ENGINE_ENVIRONMENT_INFO_DEF(key_val, name, type) if(key == type_t::key_val) return to_string(val_##key_val);
#include "def/environment_info.def"

			return ""_u;
		}

#define ENGINE_ENVIRONMENT_INFO_DEF(key, name, type) ::type get_##key() const final { return val_##key; }
#include "def/environment_info.def"

		status_t status(type_t key) const final
		{
			return statuses[value_of(key)];
		}

	private:

		std::array<status_t, value_of(type_t::count)> statuses;
		
#define ENGINE_ENVIRONMENT_INFO_DEF(key, name, type) ::type val_##key;
#include "def/environment_info.def"

#define ENGINE_ENVIRONMENT_INFO_DEF(key, name, type) void set_##key(const ::type & val_##key, status_t status = status_t::normal) { this->val_##key = val_##key; statuses[value_of(type_t::key)] = status; }
#include "def/environment_info.def"
	};

}

#endif