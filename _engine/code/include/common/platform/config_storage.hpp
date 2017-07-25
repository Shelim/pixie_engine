#ifndef ENGINE_COMMON_PLATFORM_CONFIG_STORAGE_HPP
#define ENGINE_COMMON_PLATFORM_CONFIG_STORAGE_HPP

#include "utility/text/ustring.hpp"

namespace engine
{

	namespace platform
	{

		void store(const ustring_t & key, bool val);
		void store(const ustring_t & key, uint8_t val);
		void store(const ustring_t & key, uint16_t val);
		void store(const ustring_t & key, uint32_t val);
		void store(const ustring_t & key, uint64_t val);
		void store(const ustring_t & key, int8_t val);
		void store(const ustring_t & key, int16_t val);
		void store(const ustring_t & key, int32_t val);
		void store(const ustring_t & key, int64_t val);
		void store(const ustring_t & key, const ustring_t & val);

		bool retrieve(const ustring_t & key, bool def_val);
		uint8_t retrieve(const ustring_t & key, uint8_t def_val);
		uint16_t retrieve(const ustring_t & key, uint16_t def_val);
		uint32_t retrieve(const ustring_t & key, uint32_t def_val);
		uint64_t retrieve(const ustring_t & key, uint64_t def_val);
		int8_t retrieve(const ustring_t & key, int8_t def_val);
		int16_t retrieve(const ustring_t & key, int16_t def_val);
		int32_t retrieve(const ustring_t & key, int32_t def_val);
		int64_t retrieve(const ustring_t & key, int64_t def_val);
		ustring_t retrieve(const ustring_t & key, const ustring_t & def_val);

	}

}

#endif