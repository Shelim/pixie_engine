#ifndef ENGINE_COMMON_UTILITY_VFS_VIRTUAL_PATH_HPP
#define ENGINE_COMMON_UTILITY_VFS_VIRTUAL_PATH_HPP
#pragma once

#include "filesystem.hpp"
#include <vector>
#include "utility/text/ustring.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/pattern/compilation.hpp"
#include "manifest_app.hpp"
#include "platform/path.hpp"

/**
* @page virtual_path Virtual File System (VFS)
* @tableofcontents
* @section virtual_path_intro Introduction
*		Pixie (as many other game engines) uses VFS (Virtual File System).
*		VFS allows layer of abstraction of asset path over physical file system.
*		This feature is composed out of a few features described below.
*
*		The Pixie's VFS serve not only input, but allows modyfing of any asset
*		effectivly serving as game-consumer and editor-producer at the same time.
*		The assets are reloaded dynamically during execution and can be updated
*		by any of Pixie application as well as physically by user.
*
* @section virtual_path_vpath Virtual path
*		Virtual path (vpath) represents asset inside VFS. Vpath is composed
*		out of two elements: @ref engine::virtual_path_t::type_t "type" and address.
*		Type represent search target for physical file, while address represents
*		simplified name of the file.
*
*		In general Vpath may represent more than one physical file, or even
*		no file at all (when it comes outside of @ref engine::data::database_state_t "data state database").
*
*		Vpath will always resolve to the newest file satisfying requirenments. For input
*		the requirenment is to be readable. For output the requirenment is to be both readable and writable.
*
*		Because of that, input and output may point to different files on target machine.
*		After saving, the newly saved file will usually have the highest modification date,
*		and will be noted to reload for input operation.
*
* @todo Complete this page!
*
* @see engine::virtual_path_t
*/

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

		static std::filesystem::path resolve_path(const ustring_t & path)
		{
			return path.
#define ENGINE_PATH_RESOLVE_STD(src, trg) replace(src, trg).
#include "std/virtual_path_std.hpp"
				get_cstring();
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
			p.append(item);

			return virtual_path_t(p, type);
		}

	private:

		void path_calculate_lower()
		{
			path_lower = path.to_lower();
		}

		path_t path;
		path_t path_lower;
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