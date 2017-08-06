#ifndef ENGINE_COMPONENT_TERMINAL_OUTPUT_STRING_HPP
#define ENGINE_COMPONENT_TERMINAL_OUTPUT_STRING_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/text/parser.hpp"

namespace engine
{

	enum terminal_writer_tag_t
	{
#define ENGINE_TERMINAL_OUTPUT_TAG_STD(tag) tag,
#include "std/terminal_writer_std.hpp"
		def,
		count = def
	};

	class terminal_writer_t;
	
	namespace parser
	{

		class token_terminal_escape_t : public token_base_t
		{

		public:

			static const id_t id;

			static std::unique_ptr<token_base_t> create(stream_t & stream)
			{
				if (stream.peek() == '$')
				{
					stream.advance();
					if (stream.peek() == '$')
					{
						stream.advance();
						return std::make_unique<token_terminal_escape_t>();
					}
				}

				return nullptr;
			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_terminal_escape_t>(*this);
			}

			token_terminal_escape_t() : token_base_t(id)
			{

			}

		private:


		};

		class token_terminal_eof_t : public token_base_t
		{

		public:

			static const id_t id;

			token_terminal_eof_t() : token_base_t(id)
			{

			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_terminal_eof_t>(*this);
			}

			static std::unique_ptr<token_base_t> create(stream_t & stream)
			{
				if (stream.is_eof())
				{
					return std::make_unique<token_terminal_eof_t>();
				}

				return nullptr;
			}
		};

		class token_terminal_format_t : public token_base_t
		{

		public:

			static const id_t id;

			terminal_writer_tag_t get_terminal_writer_tag() const
			{
				return terminal_writer_tag;
			}

			static std::unique_ptr<token_base_t> create(stream_t & stream)
			{
				if (stream.peek() == '$')
				{
					ustring_t key;

					stream.advance();

					while (stream.peek() != '$' && !stream.is_eof())
					{
						key.append(stream.peek());
						stream.advance();
					}

					if (!stream.is_eof())
					{
						stream.advance();

						if (key == "0"_u) return std::make_unique<token_terminal_format_t>(terminal_writer_tag_t::def);
#define ENGINE_TERMINAL_OUTPUT_TAG_STD(tag) if (key == #tag##_u) return std::make_unique<token_terminal_format_t>(terminal_writer_tag_t::tag);
#include "std/terminal_writer_std.hpp"

					}
				}

				return nullptr;
			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_terminal_format_t>(*this);
			}

			token_terminal_format_t(terminal_writer_tag_t terminal_writer_tag) : token_base_t(id), terminal_writer_tag(terminal_writer_tag)
			{

			}

		private:

			terminal_writer_tag_t terminal_writer_tag;
		};

		class resolver_terminal_t
		{

		public:

			resolver_terminal_t(std::function<void(const ustring_t &, terminal_writer_tag_t)> output_text) : output_text(output_text)
			{
				output_tag = engine::terminal_writer_tag_t::def;
			}

			void resolve(const token_base_t * token, resolver_output_t * output)
			{
				if (token->get_id() == token_terminal_escape_t::id)
				{
					output->append('$');
				}
				else if (token->get_id() == token_terminal_format_t::id)
				{
					engine::terminal_writer_tag_t new_tag = static_cast<const token_terminal_format_t*>(token)->get_terminal_writer_tag();

					if(new_tag != output_tag && output->non_empty_since_last_truncate())
						output_text(output->get_result(), output_tag);

					output->truncate_result();

					output_tag = new_tag;
				}
				else if (token->get_id() == token_terminal_eof_t::id)
				{
					if(output->non_empty_since_last_truncate())
						output_text(output->get_result(), output_tag);
				}
			}

		private:

			std::function<void(const ustring_t &, terminal_writer_tag_t)> output_text;
			engine::terminal_writer_tag_t output_tag;
		};
	}

	typedef tokenized_string_t<parser::token_terminal_eof_t, parser::token_terminal_escape_t, parser::token_terminal_format_t> terminal_writer_string_t;

}

#endif