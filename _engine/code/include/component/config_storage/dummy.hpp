#ifndef ENGINE_COMPONENT_CONFIG_PROVIDER_DUMMY_HPP
#define ENGINE_COMPONENT_CONFIG_PROVIDER_DUMMY_HPP
#pragma once

#include "component/config_storage.hpp"
#include <map>

namespace engine
{

	class config_storage_dummy_t : public config_storage_t
	{

	public:

		config_storage_dummy_t();

		void store(const ustring_t & key, const ustring_t & val) final;
		ustring_t retrieve(const ustring_t & key, const ustring_t & def_val) final;

	private:

		std::map<ustring_t, ustring_t> values;

	};
}

#endif