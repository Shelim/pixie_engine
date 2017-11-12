#ifndef ENGINE_COMMON_UTILITY_VFS_VIRTUAL_PATH_HPP
#define ENGINE_COMMON_UTILITY_VFS_VIRTUAL_PATH_HPP
#pragma once

#include "core/vfs/filesystem.hpp"
#include <vector>
#include "utility/text/ustring.hpp"


namespace engine
{

	class virtual_path_t final
	{

	public:

		typedef ustring_t path_t;

		enum class type_t : uint8_t
		{
			unknown,
#define ENGINE_VIRTUAL_PATH_STD(name) name,
#include "std/virtual_path_std.hpp"
			count
		};

		virtual_path_t() : type(type_t::unknown)
		{

		}

		virtual_path_t(const path_t & path, type_t type = type_t::unknown) : type(type)
		{
			set_path(path);
		}

		virtual_path_t(const std::filesystem::path & path, virtual_path_t::type_t type = type_t::unknown) : type(type)
		{
			set_path(path);
		}

		const ustring_t & get_ext_lower() const
		{
			return ext_lower;
		}

		const path_t & get_path() const
		{
			return path;
		}

		const path_t & get_path_lower() const
		{
			return path_lower;
		}

		const type_t & get_type() const
		{
			return type;
		}

		static path_t canonize_path(const std::filesystem::path & value)
		{
			std::vector<std::filesystem::path> ret;
			for (auto iter = value.begin(); iter != value.end(); iter++)
			{
				if (*iter == ".") continue;
				else if (*iter == "..")
				{
					if (!ret.empty()) ret.erase(ret.begin() + (ret.size() - 1));
				}
				else if (*iter == "\\")
				{
					if (ret.empty() || (ret.back() != "/" && ret.back() != "\\"))
						ret.push_back("/");
				}
				else ret.push_back(*iter);
			}

			path_t path;

			for (auto & iter : ret)
			{
				if (!path.is_empty())
				{
					if (!is_path_separator_ascii(path.last_ascii()))
						path.append('/');
				}
				std::string iter_str = iter.u8string();
				if (iter_str != "\\" && iter_str != "/")
				{
					path.append_utf8(iter_str.c_str());
				}
			}

			return path;
		}

		static path_t canonize_path_lower(const std::filesystem::path & value)
		{
			return canonize_path(value).to_lower();
		}

		void set_path(const std::filesystem::path & value)
		{
			this->path = canonize_path(value);
			int index = this->path.last_index_of_ascii(".");

			if (index == -1)
				this->ext_lower = ""_u;
			else
				this->ext_lower = this->path.substr(index);

			path_calculate_lower();
		}

		void set_path(const path_t & value)
		{
			set_path(std::filesystem::path(value.get_cstring()));
		}

		void set(const path_t & path, const type_t & type)
		{
			set_path(path);
			set_type(type);
		}

		void set(const std::filesystem::path & path, const type_t & type)
		{
			set_path(path);
			set_type(type);
		}

		void set_type(const type_t & value)
		{
			type = value;
		}

		virtual_path_t subitem(const path_t & item)
		{
			path_t p = path;
			if (p.last_ascii() != '/')
				p.append('/');
			p.append(canonize_path(item.get_cstring()));

			return virtual_path_t(p, type);
		}

	private:

		void path_calculate_lower()
		{
			path_lower = path.to_lower();
		}

		path_t path;
		path_t path_lower;
		ustring_t ext_lower;
		type_t type;
	};

	inline bool operator< (const virtual_path_t & left, const virtual_path_t & right)
	{
		if (left.get_type() == right.get_type()) return left.get_path_lower() < right.get_path_lower();
		return left.get_type() < right.get_type();
	}

	inline bool operator== (const virtual_path_t & left, const virtual_path_t & right)
	{
		if (left.get_type() != right.get_type()) return false;
		return left.get_path_lower() == right.get_path_lower();
	}

	/**
	* @brief Converts `virtual_path_t` into ustring_t
	*
	* @param[in] item Variable to convert
	* @return Converted `virtual_path_t`
	* @see from_string
	*/
	template<> inline ustring_t to_string<virtual_path_t>(const virtual_path_t & item)
	{
		std::stringstream ss;

		virtual_path_t::path_t path = item.get_path();

		ss << path.get_cstring() << " [";
#define ENGINE_VIRTUAL_PATH_STD(name) if(item.get_type() == virtual_path_t::type_t::name) { ss << #name; }
#include "std/virtual_path_std.hpp"

		ss << "]";

		return ustring_t(ss.str());
	}
}

#endif