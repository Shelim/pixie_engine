#ifndef ENGINE_COMMON_UTILITY_DATA_INPUT_HPP
#define ENGINE_COMMON_UTILITY_DATA_INPUT_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "utility/vfs/filesystem.hpp"
#include "utility/vfs/virtual_path.hpp"

#include <algorithm>

namespace engine
{
	namespace data
	{

		class input_t
		{

		public:

			virtual void jump_to_begin() = 0;
			virtual void jump_to_end() = 0;
			virtual void skip(int32_t pos) = 0;
			virtual void go_back(int32_t pos) = 0;
			virtual uint32_t read(uint8_t * buffer, uint32_t size) = 0;
			virtual bool is_eof() = 0;
			virtual uint32_t position() = 0;

			virtual ~input_t() { }
			
			typedef std::vector<uint8_t> buffer_t;

			ustring_t read_ustring()
			{
				const std::size_t PACKET_SIZE = 4096;
				std::string ret;

				while (!is_eof())
				{
					uint8_t packet[PACKET_SIZE + 1];

					uint32_t size = read(packet, PACKET_SIZE);
					packet[size] = 0;
					ret.append(reinterpret_cast<char*>(packet));
				}

				return ustring_t::from_utf8(ret.c_str());
			}

			buffer_t read_buffer()
			{
				const std::size_t PACKET_SIZE = 4096;
				buffer_t ret;

				while (!is_eof())
				{
					uint8_t packet[PACKET_SIZE + 1];

					uint32_t size = read(packet, PACKET_SIZE);
					packet[size] = 0;
					ret.insert(ret.end(), packet, packet + size);
				}

				return ret;
			}

			const virtual_path_t & virtual_path()
			{
				return path;
			}

			input_t(input_t const&) = delete;
			input_t(input_t &&) = delete;
			input_t& operator=(input_t &&) = delete;
			input_t& operator=(input_t const&) = delete;

			std::unique_ptr<input_t> spawn_partial(int32_t size);

		protected:

			input_t(virtual_path_t path) : path(path)
			{

			}

		private:

			virtual_path_t path;

		};

		class input_partial_t final : public input_t
		{

		public:

			input_partial_t(const virtual_path_t & virtual_path, const buffer_t && buffer) : input_t(virtual_path), buffer(std::move(buffer)), pos(0)
			{

			}
			void jump_to_begin() final
			{
				this->pos = 0;
			}

			void jump_to_end() final
			{
				this->pos = static_cast<int32_t>(buffer.size()) - 1;
			}

			void skip(int32_t pos) final
			{
				this->pos = std::max(0, std::min(static_cast<int32_t>(buffer.size()) - 1, this->pos + pos));
			}

			void go_back(int32_t pos) final
			{
				this->pos = std::max(0, std::min(static_cast<int32_t>(buffer.size()) - 1, this->pos - pos));
			}

			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				uint32_t len = std::min(this->buffer.size(), this->pos + size) - this->pos;
				memcpy(buffer, &this->buffer[this->pos], len);
				return len;
			}

			bool is_eof() final
			{
				return pos >= buffer.size();
			}

			uint32_t position() final
			{
				return pos;
			}

		private:

			int32_t pos;
			buffer_t buffer;
		};

		class input_streambuf_t final : public std::streambuf
		{

		public:

			input_streambuf_t(input_t * input) : input(input)
			{

			}

		private:


			int_type underflow() final
			{
				uint8_t c;
				input->read(&c, 1);

				if (input->is_eof())
				{
					input->go_back(1);
					return traits_type::eof();
				}

				input->go_back(1);

				return traits_type::to_int_type(c);
			}
			int_type uflow() final
			{
				uint8_t c;
				input->read(&c, 1);

				if (input->is_eof())
					return traits_type::eof();

				return traits_type::to_int_type(c);
			}

			int_type pbackfail(int_type ch) final
			{
				uint8_t c;
				input->go_back(1);
				input->read(&c, 1);
				input->go_back(1);

				return traits_type::to_int_type(c);
			}

			std::streamsize xsgetn(char* s, std::streamsize n) final
			{
				return input->read(reinterpret_cast<uint8_t*>(s), n);
			}

			pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in | std::ios_base::out) final
			{
				if (dir == std::ios_base::cur)
					input->skip(off);
				else if (dir == std::ios_base::beg)
				{
					input->jump_to_begin();
					input->skip(off);
				}
				else if (dir == std::ios_base::end)
				{
					input->jump_to_end();
					input->go_back(-off);
				}
				
				return input->position();
			}

			input_t * input;

		};
	}
}

#include "utility/data/input/file.hpp"
#include "utility/data/input/ustring.hpp"
#include "utility/data/input/buffer.hpp"
#include "utility/data/input/void.hpp"

#endif