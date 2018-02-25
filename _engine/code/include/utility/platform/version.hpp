#ifndef ENGINE_COMMON_UTILITY_PLATFORM_VERSION_HPP
#define ENGINE_COMMON_UTILITY_PLATFORM_VERSION_HPP

#include "utility/text/ustring.hpp"

#pragma once

namespace engine
{

	class version_t
	{

	public:

        static constexpr uint32_t any = static_cast<uint32_t>(-1);

        version_t(uint32_t major = any, uint32_t minor = any, uint32_t revision = any, uint32_t build = any) : major(major), minor(minor), revision(revision), build(build)
        {

        }

		uint32_t get_major() const
        {
            return major;
        }
		uint32_t get_minor() const
        {
            return minor;
        }
		uint32_t get_revision() const
        {
            return revision;
        }
		uint32_t get_build() const
        {
            return build;
        }

	private:

        uint32_t major;
        uint32_t minor;
        uint32_t revision;
        uint32_t build;

	};

    inline bool operator== (const version_t & version1, const version_t & version2)
    {
        if(version1.get_major() != version_t::any && version2.get_major() != version_t::any)
        {
            if(version1.get_major() != version2.get_major()) return false;
        }
        if(version1.get_minor() != version_t::any && version2.get_minor() != version_t::any)
        {
            if(version1.get_minor() != version2.get_minor()) return false;
        }
        if(version1.get_revision() != version_t::any && version2.get_revision() != version_t::any)
        {
            if(version1.get_revision() != version2.get_revision()) return false;
        }
        if(version1.get_build() != version_t::any && version2.get_build() != version_t::any)
        {
            if(version1.get_build() != version2.get_build()) return false;
        }
        return true;
    }
    inline bool operator!= (const version_t & version1, const version_t & version2)
    {
        return !(version1 == version2);
    }
    inline bool operator< (const version_t & version1, const version_t & version2)
    {
        if(version1.get_major() != version_t::any && version2.get_major() != version_t::any)
        {
            if(version1.get_major() < version2.get_major()) return true;
            if(version1.get_major() > version2.get_major()) return false;
        }
        if(version1.get_minor() != version_t::any && version2.get_minor() != version_t::any)
        {
            if(version1.get_minor() < version2.get_minor()) return true;
            if(version1.get_minor() > version2.get_minor()) return false;
        }
        if(version1.get_revision() != version_t::any && version2.get_revision() != version_t::any)
        {
            if(version1.get_revision() < version2.get_revision()) return true;
            if(version1.get_revision() > version2.get_revision()) return false;
        }
        if(version1.get_build() != version_t::any && version2.get_build() != version_t::any)
        {
            if(version1.get_build() < version2.get_build()) return true;
            if(version1.get_build() > version2.get_build()) return false;
            if(version1.get_build() == version2.get_build()) return false;
        }
        return true;
    }
    inline bool operator<= (const version_t & version1, const version_t & version2)
    {
        return version1 < version2 || version1 == version2;
    }
    inline bool operator> (const version_t & version1, const version_t & version2)
    {
        return !(version1 <= version2);
    }
    inline bool operator>= (const version_t & version1, const version_t & version2)
    {
        return !(version1 < version2);
    }

	template<> inline ustring_t to_string<version_t>(const version_t & item)
	{
		return format_string("#1#.#2#.#3#.#4#"_u, item.get_major(), item.get_minor(), item.get_revision(), item.get_build());
	}

	template<> inline version_t from_string<version_t>(const ustring_t & item)
	{

	}

}

#endif