#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_TEXT_BASE_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_TEXT_BASE_HPP
#pragma once

#include <cstddef>
#include "common/logger_output/provider_base.hpp"
#include "common/richtext.hpp"

namespace engine
{

	namespace logger_output
	{

		class provider_text_base_t : public provider_base_t
		{

		public:

			virtual ~provider_text_base_t()
			{

			}

			void process_item(const logger_t::item_t & item) final;
			void process_environment_info(std::shared_ptr<environment_info_t> environment_info) final;

			virtual void output_line(richtext_t line) = 0;

		protected:

			enum class item_output_element_t
			{
				output_sep_column,
				output_sep_arrow,
				output_sep_comma,

				output_id,
				output_prompt,
				output_message,
				output_frame,
				output_time,
				output_thread,
				output_func,
				output_file,
				output_line,
			};

			typedef std::vector<item_output_element_t> item_output_elements_t;

			enum class section_t
			{
				unknown,
				environment_info,
				item
			};

			provider_text_base_t(std::shared_ptr<logger_t> logger, item_output_elements_t item_output_elements, std::size_t item_output_column_width = 96) : provider_base_t(logger), section(section_t::unknown), item_output_elements(item_output_elements), line_constructor(this, item_output_column_width)
			{

			}

		private:

			class line_constructor_t
			{

			public:

				line_constructor_t(provider_text_base_t * owner, std::size_t column_width) : owner(owner), column_width(column_width)
				{

				}

				void start(const logger_t::item_t & item)
				{
					line.clear();
					this->item = &item;

					query_richtext_flags(this->item->get_level(), normal, meta);
					column = 0;
				}

				void populate(const item_output_elements_t & item_output_elements);

				const richtext_t & get_line()
				{
					return line;
				}

			private:

				provider_text_base_t * owner;
				richtext_t line;
				richtext_t::flag_t normal;
				richtext_t::flag_t meta;
				const logger_t::item_t * item;

				std::size_t column_width;
				std::size_t column;

				void query_richtext_flags(logger_t::item_t::level_t level, richtext_t::flag_t & normal, richtext_t::flag_t & meta);

				static ustring_t level_to_prompt(logger_t::item_t::level_t level)
				{
					if (level == logger_t::item_t::level_t::task_started)
						return _U("... ");
					if (level == logger_t::item_t::level_t::task_failed)
						return _U("..! ");
					if (level == logger_t::item_t::level_t::task_done)
						return _U("..> ");
					if (level == logger_t::item_t::level_t::message)
						return _U("--> ");
					if (level == logger_t::item_t::level_t::warning)
						return _U("-!- ");
					if (level == logger_t::item_t::level_t::error)
						return _U("!!! ");
				}
			};

			item_output_elements_t item_output_elements;

			line_constructor_t line_constructor;

			void change_section(section_t section)
			{
				if (section != this->section)
					output_section(section);

				this->section = section;
			}

			void output_section(section_t section)
			{
				switch (section)
				{
				case section_t::environment_info: output_section_name(_U("ENVIRONMENT INFO")); break;
				case section_t::item: output_section_name(_U("FULL LOG")); break;
				}
			}

			void output_section_name(const ustring_t & name)
			{
				output_line(section_hashes(name));
				output_line(section_hashes_with_name(name));
				output_line(section_hashes(name));
			}

			richtext_t section_hashes_with_name(const ustring_t & name)
			{
				richtext_t ret;
				ret.clear_flags();
				ret.set_flag(richtext_t::flag_t::section_header);
				ret.append(_U("### "));

				ret.clear_flags();
				ret.set_flag(richtext_t::flag_t::section_header_text);
				ret.append(name);

				ret.clear_flags();
				ret.set_flag(richtext_t::flag_t::section_header);
				ret.append(_U(" ###"));

				return ret;
			}

			richtext_t section_hashes(const ustring_t & name)
			{
				richtext_t ret;
				ret.clear_flags();
				ret.set_flag(richtext_t::flag_t::section_header);

				uint32_t len = 3 + 1 + name.len() + 1 + 3;
				std::string h(len, '#');
				ustring_t hashes = ustring_t::from_utf8(h.c_str());

				ret.append(hashes);

				return ret;
			}

			section_t section;

			static ustring_t generate_padding(const ustring_t & str, uint32_t pos, char c = ' ')
			{
				return generate_padding(str.len(), pos, c);
			}

			static ustring_t generate_padding(std::size_t len, uint32_t pos, char c = ' ')
			{
				if (len >= pos - 3)
				{
					std::string s(3, c);
					return ustring_t::from_utf8(s.c_str());
				}
				else
				{
					std::string s(pos - len + 3, c);
					return ustring_t::from_utf8(s.c_str());
				}
			}

		};

	}

}

#endif