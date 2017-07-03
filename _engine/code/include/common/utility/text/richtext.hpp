#ifndef ENGINE_COMMON_UTILITY_STRING_RICHTEXT_HPP
#define ENGINE_COMMON_UTILITY_STRING_RICHTEXT_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include <bitset>

namespace engine
{

	class richtext_t final
	{

	public:

		enum class flag_t
		{
#define GAME_RICHTEXT_TAG_STD(id) id,
#include "std/richtext_std.hpp"

			count
		};

		class part_t
		{

		public:

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

			const ustring_t & get_text() const
			{
				return text;
			}

			friend class richtext_t;

		private:

			typedef std::bitset<static_cast<std::size_t>(flag_t::count)> flags_t;

			part_t(const ustring_t & text, const flags_t & flags = flags_t()) : text(text), flags(flags)
			{

			}

			flags_t flags;

			ustring_t text;

		};

		typedef std::vector<part_t> parts_t;

		const parts_t & get()
		{
			return parts;
		}

		ustring_t get_raw()
		{
			ustring_t ret;

			for (auto & part : parts)
			{
				ret.append(part.get_text());
			}

			return ret;
		}

		void append(const ustring_t & text)
		{
			parts.push_back(part_t(text, flags));
			len += text.len();
		}

		void clear_flags()
		{
			flags.reset();
		}

		bool is_flag(flag_t flag) const
		{
			return flags.test(static_cast<std::size_t>(flag));
		}

		void set_flag(flag_t flag, bool value = true)
		{
			flags.set(static_cast<std::size_t>(flag), value);
		}

		richtext_t() : len(0)
		{

		}

		std::size_t get_len() const
		{
			return len;
		}

		void clear()
		{
			parts.clear();
			flags.reset();
			len = 0;
		}

	private:

		parts_t parts;

		part_t::flags_t flags;

		std::size_t len;
	};

}

#endif