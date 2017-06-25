#ifndef ENGINE_COMMON_ASSET_PATH_HPP
#define ENGINE_COMMON_ASSET_PATH_HPP
#pragma once

#include "common/filesystem.hpp"
#include "common/utility/vfs/virtual_path.hpp"
#include <vector>
#include <bitset>
#include "common/utility/text/ustring.hpp"
#include "common/language/base.hpp"

namespace engine
{
	namespace asset
	{

		class path_t final
		{

		public:

			typedef ustring_t owner_t;
			typedef ustring_t canonized_t;
			typedef ustring_t tag_t;
			typedef ustring_t name_t;

			enum class flag_t
			{
				deleted,
				count
			};

			enum class type_t
			{
				unknown,
#define GAME_ASSET_PATH_STD(name) name,
#include "common/std/virtual_path_std.hpp"
				count
			};

			path_t(const virtual_path_t & path_real);

			const virtual_path_t & get_path_real() const
			{
				return path_real;
			}

			const owner_t & get_owner() const
			{
				return owner;
			}

			type_t get_owner_type() const
			{
				return owner_type;
			}

			const language_id_t & get_lang() const
			{
				return lang;
			}

			const tag_t & get_tag() const
			{
				return tag;
			}

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

			const name_t & get_name() const
			{
				return name;
			}

			const canonized_t & get_path() const
			{
				return path_canonized;
			}

			static canonized_t clean_path(const virtual_path_t::path_t & path, bool is_common = false)
			{
				return clean_path(path.to_lower().get_cstring(), is_common);
			}

		private:

			static canonized_t clean_path(std::string src, bool is_common = false);

			static std::size_t find_next_separator(const std::string & src, std::size_t index)
			{
				std::size_t dot = src.find('.', index);
				std::size_t slash = src.find('/', index);

				if (dot != -1 && slash != -1)
					return std::min(dot, slash);
				else if (dot != -1)
					return dot;
				else if (slash != -1)
					return slash;
				else
					return src.length();

			}

			void set_flag(flag_t flag, bool value)
			{
				flags.set(static_cast<std::size_t>(flag), value);
			}

			virtual_path_t path_real;
			owner_t owner;
			type_t owner_type;
			language_id_t lang;
			tag_t tag;
			std::bitset<static_cast<std::size_t>(flag_t::count)> flags;
			name_t name;
			canonized_t path_canonized;
		};

	}
}

#endif