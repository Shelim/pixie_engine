#ifndef ENGINE_COMMON_UTILITY_STRING_RICHTEXT_HPP
#define ENGINE_COMMON_UTILITY_STRING_RICHTEXT_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/pattern/class_settings.hpp"
#include <bitset>

namespace engine
{
	class richtext_t final
	{

	public:

		enum class flag_t
		{
#define ENGINE_RICHTEXT_TAG_STD(id) id,
#include "std/richtext_std.hpp"

			count
		};

		class part_t
		{

		public:

			bool is_flag(flag_t flag) const
			{
				return flags.is_flag(flag);
			}

			const ustring_t & get_text() const
			{
				return text;
			}

			friend class richtext_t;

		private:

			part_t(const ustring_t & text, const flags_t<flag_t> & flags = flags_t<flag_t>()) : text(text), flags(flags)
			{

			}

			flags_t<flag_t> flags;

			ustring_t text;

		};

		typedef std::vector<part_t> parts_t;

		const parts_t & get() const
		{
			return parts;
		}

		ustring_t get_raw() const
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
			flags.clear_all();
		}

		bool is_flag(flag_t flag) const
		{
			return flags.is_flag(flag);
		}

		void set_flag(flag_t flag, bool value = true)
		{
			flags.set_flag(flag, value);
		}

		richtext_t() : len(0)
		{

		}

		richtext_t(const ustring_t & path) : len(0)
		{
			int_fast32_t start = 0;
			int_fast32_t tag_start = path.index_of("$"_u);
			while (tag_start != -1)
			{
				int_fast32_t tag_end = path.index_of("$"_u, tag_start + 1);
				if (tag_end != -1)
				{
					if (tag_start > start)
						append(path.substr(start, tag_start - start));

					ustring_t tag = path.substr(tag_start + 1, tag_end - tag_start - 1);

					clear_flags();
#define ENGINE_RICHTEXT_TAG_STD(tag_type) if(tag == #tag_type##_u) set_flag(flag_t::tag_type, true);
#include "std/richtext_std.hpp"

					if (tag[0] == '>')
					{
						int32_t len = from_string<int32_t>(tag.substr(1));
						if (len > this->len)
						{
							std::string get(len - this->len, ' ');
							append(ustring_t::from_utf8(get.c_str()));
						}
					}
					else if (tag[0] == '+')
					{
						int32_t len = from_string<int32_t>(tag.substr(1));
						std::string get(' ', len);
					}

					start = tag_end + 1;
					tag_start = path.index_of("$"_u, start);
				}
				else
					break;
			}
			append(path.substr(start));
		}

		std::size_t get_len() const
		{
			return len;
		}

		void clear()
		{
			parts.clear();
			flags.clear_all();
			len = 0;
		}

	private:

		parts_t parts;

		flags_t<flag_t> flags;

		std::size_t len;
	};

}

#endif