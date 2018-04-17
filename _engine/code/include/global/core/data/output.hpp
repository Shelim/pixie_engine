#ifndef ENGINE_COMMON_UTILITY_DATA_OUTPUT_HPP
#define ENGINE_COMMON_UTILITY_DATA_OUTPUT_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "global/core/vfs/virtual_path.hpp"

namespace engine
{
	namespace data
	{
		class output_t
		{

		public:

			virtual uint64_t write(const uint8_t * buffer, uint64_t size) = 0;

			enum result_t
			{
				ok,
				error
			};

			result_t write_ustring(const ustring_t & str)
			{
				if (write(reinterpret_cast<const uint8_t*>(str.get_cstring()), str.len()) == str.len())
					return result_t::ok;
				return result_t::error;
			}

			typedef std::vector<uint8_t> buffer_t;

			result_t write_buffer(const buffer_t & buffer)
			{
				if (write(&buffer[0], buffer.size()) == buffer.size())
					return result_t::ok;
				return result_t::error;
			}

			virtual void force_flush()
			{

			}

			virtual ~output_t() { }


			const virtual_path_t & virtual_path()
			{
				return path;
			}

			output_t(output_t const&) = delete;
			output_t(output_t &&) = delete;
			output_t& operator=(output_t &&) = delete;
			output_t& operator=(output_t const&) = delete;

		protected:

			output_t(virtual_path_t path)
			{

			}

		private:

			virtual_path_t path;

		};

		class output_streambuf_t final : public std::streambuf
		{

		public:

			output_streambuf_t(output_t * output) : output(output)
			{

			}

		private:

			std::streamsize xsputn(const char_type* s, std::streamsize count) final
			{
				return output->write(reinterpret_cast<const uint8_t*>(s), count);
			}

			int_type overflow(int_type ch) final
			{
				if (ch != traits_type::eof())
				{
					uint8_t c = static_cast<uint8_t>(ch);
					output->write(&c, 1);
				}
				return ch;
			}

			output_t * output;

		};

	}
}

#endif