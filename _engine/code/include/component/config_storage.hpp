#ifndef ENGINE_COMPONENT_CONFIG_STORAGE_HPP
#define ENGINE_COMPONENT_CONFIG_STORAGE_HPP
#pragma once

#include "utility/text/ustring.hpp"

namespace engine
{

	class config_storage_t
	{

	public:

		virtual ~config_storage_t();

		virtual void store(const ustring_t & key, bool val);
		virtual void store(const ustring_t & key, uint8_t val);
		virtual void store(const ustring_t & key, uint16_t val);
		virtual void store(const ustring_t & key, uint32_t val);
		virtual void store(const ustring_t & key, uint64_t val);
		virtual void store(const ustring_t & key, int8_t val);
		virtual void store(const ustring_t & key, int16_t val);
		virtual void store(const ustring_t & key, int32_t val);
		virtual void store(const ustring_t & key, int64_t val);
		virtual void store(const ustring_t & key, const ustring_t & val) = 0;

		virtual bool retrieve(const ustring_t & key, bool def_val);
		virtual uint8_t retrieve(const ustring_t & key, uint8_t def_val);
		virtual uint16_t retrieve(const ustring_t & key, uint16_t def_val);
		virtual uint32_t retrieve(const ustring_t & key, uint32_t def_val);
		virtual uint64_t retrieve(const ustring_t & key, uint64_t def_val);
		virtual int8_t retrieve(const ustring_t & key, int8_t def_val);
		virtual int16_t retrieve(const ustring_t & key, int16_t def_val);
		virtual int32_t retrieve(const ustring_t & key, int32_t def_val);
		virtual int64_t retrieve(const ustring_t & key, int64_t def_val);
		virtual ustring_t retrieve(const ustring_t & key, const ustring_t & def_val) = 0;

	};


}

#include "component/config_storage/dummy.hpp"
#include "component/config_storage/real.hpp"

#endif