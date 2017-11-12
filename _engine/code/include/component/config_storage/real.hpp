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

		config_storage_real_t(std::unique_ptr<holder_t<config_storage_t> > config_storage_provider) : config_storage_provider(std::move(config_storage_provider))
		{

		}

		void store(const ustring_t & key, bool val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, uint8_t val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, uint16_t val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, uint32_t val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, uint64_t val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, int8_t val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, int16_t val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, int32_t val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, int64_t val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}
		void store(const ustring_t & key, const ustring_t & val) final
		{
			config_storage_provider->get_provider()->store(key, val);
		}

		bool retrieve(const ustring_t & key, bool def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		uint8_t retrieve(const ustring_t & key, uint8_t def_val) final
		{ 
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		uint16_t retrieve(const ustring_t & key, uint16_t def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		uint32_t retrieve(const ustring_t & key, uint32_t def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		uint64_t retrieve(const ustring_t & key, uint64_t def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		int8_t retrieve(const ustring_t & key, int8_t def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		int16_t retrieve(const ustring_t & key, int16_t def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		int32_t retrieve(const ustring_t & key, int32_t def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		int64_t retrieve(const ustring_t & key, int64_t def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}
		ustring_t retrieve(const ustring_t & key, const ustring_t & def_val) final
		{
			return config_storage_provider->get_provider()->retrieve(key, def_val);
		}

		auto get_temp()
		{
			return config_storage_provider->get_provider();
		}

	private:

		std::unique_ptr<holder_t<config_storage_t> > config_storage_provider;

	};
	
}

#include "component/config_storage/provider/windows_registry.hpp"

#endif