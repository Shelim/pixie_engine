#ifndef ENGINE_COMPONENT_CONFIG_PROVIDER_WINDOWS_REGISTRY_HPP
#define ENGINE_COMPONENT_CONFIG_PROVIDER_WINDOWS_REGISTRY_HPP
#pragma once

#if PIXIE_WINDOWS

#include "component/config_storage/real.hpp"
#include "core/manifest/manifest.hpp"

namespace engine
{

	class config_storage_provider_windows_registry_t : public config_storage_provider_base_t
	{

	public:

		config_storage_provider_windows_registry_t(std::shared_ptr<manifest_windows_t> manifest_windows);

		void store(const ustring_t & key, bool val) final;
		void store(const ustring_t & key, uint8_t val) final;
		void store(const ustring_t & key, uint16_t val) final;
		void store(const ustring_t & key, uint32_t val) final;
		void store(const ustring_t & key, uint64_t val) final;
		void store(const ustring_t & key, int8_t val) final;
		void store(const ustring_t & key, int16_t val) final;
		void store(const ustring_t & key, int32_t val) final;
		void store(const ustring_t & key, int64_t val) final;
		void store(const ustring_t & key, const ustring_t & val) final;

		bool retrieve(const ustring_t & key, bool def_val) final;
		uint8_t retrieve(const ustring_t & key, uint8_t def_val) final;
		uint16_t retrieve(const ustring_t & key, uint16_t def_val) final;
		uint32_t retrieve(const ustring_t & key, uint32_t def_val) final;
		uint64_t retrieve(const ustring_t & key, uint64_t def_val) final;
		int8_t retrieve(const ustring_t & key, int8_t def_val) final;
		int16_t retrieve(const ustring_t & key, int16_t def_val) final;
		int32_t retrieve(const ustring_t & key, int32_t def_val) final;
		int64_t retrieve(const ustring_t & key, int64_t def_val) final;
		ustring_t retrieve(const ustring_t & key, const ustring_t & def_val) final;

	private:

		std::shared_ptr<manifest_windows_t> manifest_windows;

	};

}

#endif
#endif