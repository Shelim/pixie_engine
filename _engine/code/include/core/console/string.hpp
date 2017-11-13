#ifndef ENGINE_CORE_CONSOLE_STRING_HPP
#define ENGINE_CORE_CONSOLE_STRING_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/pattern/flags.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/text/parser.hpp"

namespace engine
{

	enum class console_tag_t
	{
#define ENGINE_CONSOLE_OUTPUT_TAG_STD(tag) tag,
#include "def/console.def"
		def,
		count = def
	};

	class console_t;

	namespace parser
	{

		class token_console_escape_t : public token_base_t
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
						return std::make_unique<token_console_escape_t>();
					}
				}

				return nullptr;
			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_console_escape_t>(*this);
			}

			token_console_escape_t() : token_base_t(id)
			{

			}

		private:


		};

		class token_console_eof_t : public token_base_t
		{

		public:

			static const id_t id;

			token_console_eof_t() : token_base_t(id)
			{

			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_console_eof_t>(*this);
			}

			static std::unique_ptr<token_base_t> create(stream_t & stream)
			{
				if (stream.is_eof())
				{
					return std::make_unique<token_console_eof_t>();
				}

				return nullptr;
			}
		};

		class token_console_format_t : public token_base_t
		{

		public:

			static const id_t id;

			console_tag_t get_console_tag() const
			{
				return console_tag;
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

						if (key == "0"_u) return std::make_unique<token_console_format_t>(console_tag_t::def);
#define ENGINE_CONSOLE_OUTPUT_TAG_STD(tag) if (key == #tag##_u) return std::make_unique<token_console_format_t>(console_tag_t::tag);
#include "def/console.def"

					}
				}

				return nullptr;
			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_console_format_t>(*this);
			}

			token_console_format_t(console_tag_t console_tag) : token_base_t(id), console_tag(console_tag)
			{

			}

		private:

			console_tag_t console_tag;
		};

		class resolver_console_t
		{

		public:

			resolver_console_t(std::function<void(const ustring_t &, console_tag_t)> output_text) : output_text(output_text)
			{
				output_tag = engine::console_tag_t::def;
			}

			void resolve(const token_base_t * token, resolver_output_t * output)
			{
				if (token->get_id() == token_console_escape_t::id)
				{
					output->append('$');
				}
				else if (token->get_id() == token_console_format_t::id)
				{
					engine::console_tag_t new_tag = static_cast<const token_console_format_t*>(token)->get_console_tag();

					if (new_tag != output_tag && output->non_empty_since_last_truncate())
						output_text(output->get_result(), output_tag);

					output->truncate_result();

					output_tag = new_tag;
				}
				else if (token->get_id() == token_console_eof_t::id)
				{
					if (output->non_empty_since_last_truncate())
						output_text(output->get_result(), output_tag);
				}
			}

		private:

			std::function<void(const ustring_t &, console_tag_t)> output_text;
			engine::console_tag_t output_tag;
		};
	}

	typedef tokenized_string_t<parser::token_console_eof_t, parser::token_console_escape_t, parser::token_console_format_t> console_string_t;

}

#endif