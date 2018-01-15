#ifndef ENGINE_COMMON_UTILITY_TEXT_FORMAT_STRING_HPP
#define ENGINE_COMMON_UTILITY_TEXT_FORMAT_STRING_HPP
#pragma once

#include "utility/text/parser_common.hpp"


namespace engine
{

	namespace parser
	{

		struct token_formatter_t
		{
			uint32_t num;
			token_text_t format;
		};

	}
}

BOOST_FUSION_ADAPT_STRUCT
(
	engine::parser::token_formatter_t,
	(uint32_t, num)
	(engine::parser::token_text_t, format)
)


namespace engine
{

	namespace parser
	{

		typedef boost::variant<token_text_t, token_console_formatter_t, token_formatter_t> token_t;
		typedef std::vector<token_t> tokenized_string_t;

		namespace rules
		{
			namespace
			{
				const boost::spirit::x3::rule<class ustring_formatter_format, token_text_t> ustring_formatter_format = "ustring_formatter_format";
				auto const ustring_formatter_format_def = boost::spirit::x3::lit(':') >> +(boost::spirit::x3::char_ - '#' - '@');

				const boost::spirit::x3::rule<class ustring_formatter_comment> ustring_formatter_comment = "ustring_formatter_comment";
				auto const ustring_formatter_comment_def = boost::spirit::x3::lit('@') >> +(boost::spirit::x3::char_ - '#' - ':');

				const boost::spirit::x3::rule<class ustring_formatter, token_formatter_t> ustring_formatter = "ustring_formatter";
				auto const ustring_formatter_def = boost::spirit::x3::lit('#') >> boost::spirit::x3::uint_ >> (ustring_formatter_format | boost::spirit::x3::attr("")) >> (-ustring_formatter_comment) >> boost::spirit::x3::lit('#');
			
				const boost::spirit::x3::rule<class ustring_console_formatter, token_console_formatter_t> ustring_console_formatter = "ustring_console_formatter";
				auto const ustring_console_formatter_def = boost::spirit::x3::lit('$') >> +(boost::spirit::x3::char_ - '$') >> boost::spirit::x3::lit('$');

				const boost::spirit::x3::rule<class ustring_comment, token_text_t> ustring_comment = "ustring_comment";
				auto const ustring_comment_def = boost::spirit::x3::lit('#') >> boost::spirit::x3::char_('#');

				const boost::spirit::x3::rule<class ustring_text, token_text_t> ustring_text = "ustring_text";
				auto const ustring_text_def = +(boost::spirit::x3::char_ - '#' - '$');

				const boost::spirit::x3::rule<class ustring_misformat, token_text_t> ustring_misformat = "ustring_misformat";
				auto const ustring_misformat_def = +(boost::spirit::x3::char_ - '#' - '$');

				const boost::spirit::x3::rule<class ustring_format_token, token_t> ustring_format_token = "ustring_format_token";
				auto const ustring_format_token_def = ustring_console_formatter | ustring_comment | ustring_formatter | ustring_text | ustring_misformat;

				const boost::spirit::x3::rule<class ustring_format, tokenized_string_t> ustring_format = "ustring_format";
				auto const ustring_format_def = *ustring_format_token;

				BOOST_SPIRIT_DEFINE(ustring_formatter_format)
				BOOST_SPIRIT_DEFINE(ustring_formatter_comment)
				BOOST_SPIRIT_DEFINE(ustring_formatter)
				BOOST_SPIRIT_DEFINE(ustring_console_formatter)
				BOOST_SPIRIT_DEFINE(ustring_comment)
				BOOST_SPIRIT_DEFINE(ustring_text)
				BOOST_SPIRIT_DEFINE(ustring_misformat)
				BOOST_SPIRIT_DEFINE(ustring_format_token)
				BOOST_SPIRIT_DEFINE(ustring_format)
			}
		}

        template<typename... args_t> class formattable_visitor_t : public visitor_base_t
		{
        private:

			const std::tuple<args_t...> & tuple;

        public:

			formattable_visitor_t(resolver_output_t & output, const std::tuple<args_t...> & tuple) : tuple(tuple), visitor_base_t(output)
			{

			}
            
            using visitor_base_t::operator();

			void operator()(const token_formatter_t &s) const
			{
				if (s.num == 0 || s.num > sizeof...(args_t))
					out().append(to_string(""_u, ustring_t::from_utf8(s.format.c_str())));
				else
					out().append(tuple_expand_t{}(tuple, s.num - 1, ustring_t::from_utf8(s.format.c_str())));
			}

			void operator()(const token_console_formatter_t &s) const
			{
				out().append_ghostly('$');
				out().append_ghostly(ustring_t::from_utf8(s.text.c_str()));
				out().append_ghostly('$');
			}
		};
    }

	template<class... args_t> inline ustring_t format_string(const ustring_t & str, args_t... args)
	{
		auto iter = str.begin();
		parser::tokenized_string_t result;
		boost::spirit::x3::parse(iter, str.end(), parser::rules::ustring_format, result);
		
		std::tuple<args_t...> tuple(args...);
		parser::resolver_output_t output;

		for (auto & iter : result)
		{
			boost::apply_visitor(parser::formattable_visitor_t<args_t...>(output, tuple), iter);
		}

		return output.get_result();
	}
}

#endif