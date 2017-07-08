#ifndef ENGINE_COMMON_UTILITY_STRING_RICHTEXT_HPP
#define ENGINE_COMMON_UTILITY_STRING_RICHTEXT_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/pattern/class_settings.hpp"
#include <bitset>

namespace engine
{
	struct richtext_colors_t
	{

	};

	SETTINGS_TABLE_START(richtext_colors_t)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_default, "#C0C0C0"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_default, "#000000"_u)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_section_header, "#800080"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_section_header, "#000000"_u)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_section_header_text, "#FF00FF"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_section_header_text, "#000000"_u)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_info, "#00FFFF"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_info, "#000000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, foreground_info_meta, "#008080"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_info_meta, "#000000"_u)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_note, "#C0C0C0"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_note, "#000000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, foreground_note_meta, "#808080"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_note_meta, "#000000"_u)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_success, "#00FF00"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_success, "#000000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, foreground_success_meta, "#008000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_success_meta, "#000000"_u)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_pending, "#00FFFF"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_pending, "#000000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, foreground_pending_meta, "#008080"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_pending_meta, "#000000"_u)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_warning, "#FFFF00"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_warning, "#000000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, foreground_warning_meta, "#808000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_warning_meta, "#000000"_u)

		SETTINGS_TABLE_ENTRY(ustring_t, foreground_error, "#FF0000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_error, "#000000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, foreground_error_meta, "#800000"_u)
		SETTINGS_TABLE_ENTRY(ustring_t, background_error_meta, "#000000"_u)

		SETTINGS_TABLE_END()

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

		ustring_t get_inline_html(NON_OWNING_PTR_TO_SETTINGS_FOR(richtext_colors_t) richtext_colors) const
		{
			ustring_t ret;

			ret.append("<div style=\"margin:0px; font-family:monospace; padding:10px; text-indent:-50px; padding-left:50px; white-space:pre-wrap; color:"_u).append(richtext_colors->get()->foreground_default()).append("; background-color:"_u).append(richtext_colors->get()->background_default()).append(";\">"_u);

			for (auto & part : parts)
			{
#define ENGINE_RICHTEXT_TAG_STD(richtext_type) if (part.is_flag(richtext_t::flag_t::richtext_type)) { ret.append("<span style=\"color:"_u).append(richtext_colors->get()->foreground_##richtext_type()).append(";background-color:"_u).append(richtext_colors->get()->background_##richtext_type()).append("\">"_u).append(part.get_text().replace("<"_u, "&lt;"_u).replace(">"_u, "&gt;"_u)).append("</span>"_u); continue; }
#include "std/richtext_std.hpp"
				
				ret.append("<span style=\"color:"_u).append(richtext_colors->get()->foreground_default()).append(";background-color:"_u).append(richtext_colors->get()->background_default()).append("\">"_u).append(part.get_text().replace("<"_u, "&lt;"_u).replace(">"_u, "&gt;"_u)).append("</span>"_u);

			}

			ret.append("</div>\n"_u);

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