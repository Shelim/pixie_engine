#ifndef ENGINE_COMMON_UTILITY_TEXT_PARSER_HPP
#define ENGINE_COMMON_UTILITY_TEXT_PARSER_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/text/expand.hpp"
#include "utility/pattern/fourcc.hpp"
#include <deque>

namespace engine
{

	namespace parser
	{

		class stream_t
		{

		public:

			stream_t(const ustring_t & str) : iter(str.begin()), iter_end(str.end())
			{

			}

			stream_t(const stream_t & other) : iter(other.iter), iter_end(other.iter_end)
			{

			}

			stream_t & operator=(const stream_t & other)
			{
				iter = other.iter;
				iter_end = other.iter_end;

				return (*this);
			}

			bool is_eof()
			{
				return iter == iter_end;
			}

			bool is_eof(std::size_t ahead)
			{
				ustring_t::const_iterator_t iter_tmp = iter;

				while (!is_eof() && ahead > 0)
				{
					++iter_tmp;
					--ahead;
				}

				return is_eof();
			}

			void advance(std::size_t ahead = 1)
			{
				for (std::size_t i = 0; i < ahead && !is_eof(); i++)
				{
					++iter;
				}
			}

			usymbol_t peek()
			{
				return *iter;
			}

			int32_t advance_number()
			{
				int32_t ret = 0;

				while (!is_eof())
				{
					usymbol_t num = peek();
					if (num >= '0' && num <= '9')
					{
						ret = (ret * 10) + num - '0';
						advance();
					}
					else
						break;
				}
				return ret;
			}

			bool peek_is_number()
			{
				usymbol_t num = peek();
				if (num >= '0' && num <= '9')
					return true;

				return false;
			}

			usymbol_t peek(std::size_t ahead)
			{
				if (is_eof()) return 0;

				ustring_t::const_iterator_t iter_tmp = iter;
				for (std::size_t i = 0; i < ahead; i++)
				{
					++iter_tmp;
					if (is_eof()) return 0;
				}

				return *iter_tmp;
			}

		private:

			ustring_t::const_iterator_t iter;
			ustring_t::const_iterator_t iter_end;
		};


		class token_base_t
		{

		public:

			virtual ~token_base_t()
			{

			}

			id_t get_id() const
			{
				return id;
			}

			virtual std::unique_ptr<token_base_t> clone() const = 0;

		protected:

			token_base_t(id_t id) : id(id)
			{

			}

		private:

			id_t id;

		};

		class token_text_t : public token_base_t
		{

		public:

			static const id_t id ;

			const ustring_t & get_buffer() const
			{
				return buffer;
			}

			token_text_t(const ustring_t & buffer) : token_base_t(id), buffer(buffer)
			{

			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_text_t>(*this);
			}

		private:

			const ustring_t buffer;
		};

		class token_terminal_writer_helper_t : public token_base_t
		{

		public:

			static const id_t id;

			static std::unique_ptr<token_base_t> create(stream_t & stream)
			{
				if (stream.peek() == '$')
				{
					ustring_t buffer;
					buffer.append('$');

					stream.advance();

					while (stream.peek() != '$' && !stream.is_eof())
					{
						buffer.append(stream.peek());
						stream.advance();
					}

					if(stream.peek() == '$')
					{
						buffer.append('$');
						stream.advance();

						return std::make_unique<token_terminal_writer_helper_t>(buffer);
					}
				}

				return nullptr;
			}

			token_terminal_writer_helper_t(const ustring_t & buffer) : token_base_t(id), buffer(buffer)
			{

			}

			const ustring_t & get_buffer() const
			{
				return buffer;
			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_terminal_writer_helper_t>(*this);
			}

		private:

			const ustring_t buffer;
		};

		class token_escape_t : public token_base_t
		{

		public:

			static const id_t id;

			static std::unique_ptr<token_base_t> create(stream_t & stream)
			{
				if (stream.peek() == '#')
				{
					stream.advance();
					if (stream.peek() == '#')
					{
						stream.advance();
						return std::make_unique<token_escape_t>();
					}
				}

				return nullptr;
			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_escape_t>(*this);
			}

			token_escape_t() : token_base_t(id)
			{

			}

		private:


		};

		class token_format_number_t : public token_base_t
		{

		public:

			static const id_t id;

			uint32_t get_format_number() const
			{
				return format_number;
			}

			static std::unique_ptr<token_base_t> create(stream_t & stream)
			{
				if (stream.peek() == '#')
				{
					stream.advance();

					uint32_t format_number = stream.advance_number();
					if (format_number != 0)
					{
						if (stream.peek() == ':' || stream.peek() == '#')
						{
							while (stream.peek() != '#' && !stream.is_eof())
								stream.advance();

							if (stream.peek() == '#')
							{
								stream.advance();

								return std::make_unique<token_format_number_t>(format_number);
							}
						}

					}
				}

				return nullptr;
			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_format_number_t>(*this);
			}

			token_format_number_t(uint32_t format_number) : token_base_t(id), format_number(format_number)
			{

			}

		private:

			const uint32_t format_number;
		};

		class token_aligment_t : public token_base_t
		{

		public:

			static const id_t id;

			uint32_t get_column() const
			{
				return column;
			}

			usymbol_t get_symbol() const
			{
				return symbol;
			}

			static std::unique_ptr<token_base_t> create(stream_t & stream)
			{
				if (stream.peek() == '#')
				{
					stream.advance();
					if (stream.peek() == '>')
					{
						stream.advance();

						if(stream.peek_is_number())
						{
							uint32_t column = stream.advance_number();
							usymbol_t symbol = ' ';

							if (stream.peek() == ':')
							{
								stream.advance();
								symbol = stream.peek();
								stream.advance();
							}
							if (stream.peek() == '#')
							{
								stream.advance();

								return std::make_unique<token_aligment_t>(column, symbol);
							}
						}


					}
				}

				return nullptr;
			}

			std::unique_ptr<token_base_t> clone() const final
			{
				return std::make_unique<token_aligment_t>(*this);
			}

			token_aligment_t(uint32_t column, usymbol_t symbol = ' ') : token_base_t(id), column(column), symbol(symbol)
			{

			}

		private:

			const uint32_t column;
			const usymbol_t symbol;
		};

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

		template<class... args_t> class resolver_format_t
		{

		public:

			resolver_format_t(args_t... args)
			{
				expand_into_array(a, args...);
			}

			void resolve(const token_base_t * token, resolver_output_t * output)
			{
				if (token->get_id() == token_format_number_t::id)
				{
					uint32_t number = static_cast<const token_format_number_t*>(token)->get_format_number() - 1;
					if (number < a.size())
						output->append(a[number]);
				}
				else if (token->get_id() == token_escape_t::id)
				{
					output->append('#');
				}
				else if (token->get_id() == token_aligment_t::id)
				{
					usymbol_t symbol = static_cast<const token_aligment_t*>(token)->get_symbol();
					while (output->len() < static_cast<const token_aligment_t*>(token)->get_column())
						output->append(symbol);
				}
				else if (token->get_id() == token_terminal_writer_helper_t::id)
				{
					output->append_ghostly(static_cast<const token_terminal_writer_helper_t*>(token)->get_buffer());
				}
			}

		private:

			std::array<ustring_t, sizeof...(args_t)> a;

		};

	}

	typedef std::vector<std::unique_ptr<parser::token_base_t> > tokens_t;

	template<class... allowed_tokens_t> class tokenized_string_t
	{

	private:

		static std::unique_ptr<parser::token_base_t> accept(parser::stream_t & stream, std::tuple<> )
		{
			return nullptr;
		}

		template<class allowed_token_t, class... allowed_tokens_t> static std::unique_ptr<parser::token_base_t> accept(parser::stream_t & stream, std::tuple<allowed_token_t*, allowed_tokens_t*...>)
		{
			parser::stream_t stream_tmp = stream;
			std::unique_ptr<parser::token_base_t> token = allowed_token_t::create(stream_tmp);
			if (token)
			{
				stream = stream_tmp;
				return std::move(token);
			}

			return accept(stream, std::tuple<allowed_tokens_t*...>());
		}

		tokens_t tokens;

		void parse(ustring_t source)
		{
			tokens.clear();
			parser::stream_t stream = source;

			ustring_t elem;

			while (!stream.is_eof())
			{

				std::unique_ptr<parser::token_base_t> token = accept(stream, std::tuple<allowed_tokens_t*...>());
				if (!token)
				{
					elem.append(stream.peek());
					stream.advance();
				}
				else
				{
					if (!elem.is_empty())
					{
						tokens.push_back(std::make_unique<parser::token_text_t>(elem));
						elem = ustring_t();
					}

					tokens.push_back(std::move(token));

				}
			}

			std::unique_ptr<parser::token_base_t> token = accept(stream, std::tuple<allowed_tokens_t*...>());
			if (token)
			{
				if (!elem.is_empty())
				{
					tokens.push_back(std::make_unique<parser::token_text_t>(elem));
					elem = ustring_t();
				}

				tokens.push_back(std::move(token));

			}
			else if (!elem.is_empty())
			{
				tokens.push_back(std::make_unique<parser::token_text_t>(elem));
				elem = ustring_t();
			}
		}

	public:

		const tokens_t & get() const
		{
			return tokens;
		}

		tokens_t::const_iterator begin() const
		{
			return tokens.begin();
		}

		tokens_t::const_iterator end() const
		{
			return tokens.end();
		}

		tokenized_string_t()
		{

		}

		tokenized_string_t(tokenized_string_t && other)
		{
			std::swap(tokens, other.tokens);
		}

		tokenized_string_t(const tokenized_string_t & other)
		{
			for (const auto & token : other.tokens)
			{
				tokens.push_back(std::move(token->clone()));
			}
		}

		tokenized_string_t(const ustring_t & str)
		{
			parse(str);
		}

		tokenized_string_t & operator= (tokenized_string_t && other)
		{
			std::swap(tokens, other.tokens);

			return *this;
		}

		tokenized_string_t & operator= (const tokenized_string_t & other)
		{
			tokens.clear();

			for (const auto & token : other.tokens)
			{
				tokens.push_back(std::move(token->clone()));
			}

			return *this;
		}

		tokenized_string_t & operator= (const ustring_t & str)
		{
			parse(str);

			return *this;
		}
		
	};

	inline void resolve_token(const parser::token_base_t * token, parser::resolver_output_t * output)
	{
		if (token->get_id() == parser::token_text_t::id)
		{
			output->append(static_cast<const parser::token_text_t*>(token)->get_buffer());
		}
	}

	template<class resolve_t, class... resolvers_t> inline void resolve_token(const parser::token_base_t * token, parser::resolver_output_t * output, resolve_t & resolver, resolvers_t&... resolvers)
	{
		resolver.resolve(token, output);
		resolve_token(token, output, resolvers...);
	}

	template<class... resolvers_t> inline void resolve(const tokens_t & tokens, parser::resolver_output_t * output, resolvers_t&... resolvers)
	{
		for (const auto & iter : tokens)
		{
			resolve_token(iter.get(), output, resolvers...);
		}
	}

	typedef tokenized_string_t<parser::token_escape_t, parser::token_aligment_t, parser::token_format_number_t, parser::token_terminal_writer_helper_t> formattable_string_t;

	template<class... args_t> inline ustring_t format_string(const formattable_string_t & str, args_t... args)
	{
		parser::resolver_output_t resolver;
		resolve(str.get(), &resolver, parser::resolver_format_t<args_t...>(args...));
		return resolver.get_result();
	}
}



#endif