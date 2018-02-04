#ifndef ENGINE_COMPONENT_FILESYSTEM_HPP
#define ENGINE_COMPONENT_FILESYSTEM_HPP
#pragma once

#include "utility/text/ustring.hpp"

namespace engine
{

	class filesystem_t
	{

	public:

		virtual ~filesystem_t();

		enum class file_seek_origin_t
		{
			begin,
			end,
			current
		};

		class file_output_t
		{

		public:

			virtual ~file_output_t();
			virtual uint32_t write(const uint8_t * buffer, uint32_t size) = 0;
			virtual void flush() = 0;

		protected:

			file_output_t() = default;

		};

		class file_input_t
		{

		public:

			virtual ~file_input_t();

			virtual void seek(int32_t position, file_seek_origin_t origin) = 0;
			virtual bool is_eof() const = 0;
			virtual uint32_t tell() const = 0;
			virtual uint32_t read(uint8_t * buffer, uint32_t size) = 0;

		protected:

			file_input_t() = default;

		};

		virtual std::filesystem::file_time_type get_mod_time(std::filesystem::path path) = 0;

		virtual paths_t iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) = 0;
		virtual paths_t iterate_directories(std::filesystem::path path, const ustring_t & pattern = "*"_u) = 0;
		virtual paths_t iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) = 0;
		virtual paths_t iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*"_u) = 0;

		virtual std::shared_ptr<file_output_t> output(std::filesystem::path path) = 0;
		virtual std::shared_ptr<file_input_t> input(std::filesystem::path path) = 0;

		virtual std::filesystem::path construct_temp_file() = 0;
		virtual bool exists(std::filesystem::path path) = 0;
		virtual bool is_read_only(std::filesystem::path path) = 0;
		virtual bool is_directory(std::filesystem::path path) = 0;

		virtual void copy_file(std::filesystem::path src, std::filesystem::path trg) = 0;
		virtual void copy_directory(std::filesystem::path src, std::filesystem::path trg) = 0;

		virtual void delete_file(std::filesystem::path path) = 0;
		virtual void delete_directory(std::filesystem::path path) = 0;

		virtual void move_file(std::filesystem::path src, std::filesystem::path trg);
		virtual void move_directory(std::filesystem::path src, std::filesystem::path trg);
	};

}

#include "global/component/filesystem/dummy.hpp"
#include "global/component/filesystem/real.hpp"

#endif