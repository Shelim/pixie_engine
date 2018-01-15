#ifndef ENGINE_COMMON_UTILITY_TEXT_PARSER_COMMON_HPP
#define ENGINE_COMMON_UTILITY_TEXT_PARSER_COMMON_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/text/expand.hpp"
#include <cstddef>
#include <deque>
#include <vector>
#include <limits>
#include <string>
#include <iostream>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace engine
{
	
	namespace parser
	{

		typedef std::string token_text_t;

		struct token_console_formatter_t
		{
			token_text_t text;
		};

	}
}

BOOST_FUSION_ADAPT_STRUCT
(
	engine::parser::token_console_formatter_t,
	(engine::parser::token_text_t, text)
)

namespace engine
{
	
	namespace parser
	{

		class resolver_output_t
		{

		public:

			resolver_output_t() : length(0)
			{

			}

			void append(const ustring_t & text)
			{
				result.append(text);
				length += text.len();
			}

			void append(usymbol_t symbol)
			{
				result.append(symbol);
				length++;
			}

			void append_ghostly(const ustring_t & text)
			{
				result.append(text);
			}

			void append_ghostly(usymbol_t symbol)
			{
				result.append(symbol);
			}

			std::size_t len()
			{
				return length;
			}

			const ustring_t & get_result()
			{
				return result;
			}

			void truncate_result()
			{
				result = ustring_t();
			}

			bool non_empty_since_last_truncate()
			{
				return !result.is_empty();
			}

		private:

			ustring_t result;
			std::size_t length;

		};

		class visitor_base_t : public boost::static_visitor<void>
		{

		public:

			void operator()(const token_text_t &s) const { output.append(ustring_t::from_utf8(s.c_str())); }

		protected:

			visitor_base_t(resolver_output_t & output) : output(output)
			{

			}

			resolver_output_t & out() const
			{
				return output;
			}

		private:

			resolver_output_t & output;
		};
	}

}



#endif