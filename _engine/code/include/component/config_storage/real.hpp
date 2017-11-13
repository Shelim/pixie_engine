#ifndef ENGINE_COMPONENT_CONFIG_REAL_HPP
#define ENGINE_COMPONENT_CONFIG_REAL_HPP
#pragma once

#include "component/config_storage.hpp"
#include "utility/pattern/provider.hpp"

namespace engine
{

	typedef config_storage_t config_storage_provider_base_t;

	REGISTER_PROVIDER_BASE_TYPE(config_storage_t, config_storage_provider_base_t)

	class config_storage_real_t : public config_storage_t
	{

	public:

		config_storage_real_t(std::unique_ptr<holder_t<config_storage_t> > config_storage_provider);

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

		std::unique_ptr<holder_t<config_storage_t> > config_storage_provider;

	};
	
}

#include "component/config_storage/provider/windows_registry.hpp"

#endif