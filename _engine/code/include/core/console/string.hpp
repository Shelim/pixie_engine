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
#define ENGINE_CONSOLE_OUTPUT_TAG_DEF(tag) tag,
#include "def/console.def"
		def,
		count = def
	};
	
#define STRINGIFY_ENUM_TYPE console_tag_t
#define ENGINE_CONSOLE_OUTPUT_TAG_DEF STRINGIFY_DEF_NAME
#define STRINGIFY_DEF_INCLUDE "def/console.def"
#include "core/utility/stringify_def.hpp"

	class console_t;

	namespace parser
	{

		class token_console_escape_t : public token_base_t
		{

		public:

			static const id_t id;

			token_console_escape_t();

			static std::unique_ptr<token_base_t> create(stream_t & stream);

			std::unique_ptr<token_base_t> clone() const final;

		private:


		};

		class token_console_eof_t : public token_base_t
		{

		public:

			static const id_t id;

			token_console_eof_t();

			std::unique_ptr<token_base_t> clone() const;

			static std::unique_ptr<token_base_t> create(stream_t & stream);
		};

		class token_console_format_t : public token_base_t
		{

		public:

			static const id_t id;

			token_console_format_t(console_tag_t console_tag);

			console_tag_t get_console_tag() const;

			static std::unique_ptr<token_base_t> create(stream_t & stream);

			std::unique_ptr<token_base_t> clone() const final;

		private:

			console_tag_t console_tag;
		};

		class resolver_console_t
		{

		public:

			resolver_console_t(std::function<void(const ustring_t &, console_tag_t)> output_text);

			void resolve(const token_base_t * token, resolver_output_t * output);

		private:

			std::function<void(const ustring_t &, console_tag_t)> output_text;
			engine::console_tag_t output_tag;
		};
	}

	typedef tokenized_string_t<parser::token_console_eof_t, parser::token_console_escape_t, parser::token_console_format_t> console_string_t;

}

#endif