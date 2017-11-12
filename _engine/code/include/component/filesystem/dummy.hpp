#ifndef ENGINE_COMPONENT_FILESYSTEM_DUMMY_HPP
#define ENGINE_COMPONENT_FILESYSTEM_DUMMY_HPP
#pragma once

#include "component/filesystem.hpp"

namespace engine
{

	class filesystem_dummy_t : public filesystem_t
	{

	public:

		std::filesystem::file_time_type get_mod_time(std::filesystem::path path) final
		{
			return std::filesystem::file_time_type::min();
		}

		paths_t iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final
		{
			return paths_t();
		}
		paths_t iterate_directories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final
		{
			return paths_t();
		}
		paths_t iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final
		{
			return paths_t();
		}
		paths_t iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final
		{
			return paths_t();
		}

		std::shared_ptr<file_output_t> output(std::filesystem::path path) final
		{
			return std::make_shared<file_output_dummy_t>();
		}
		std::shared_ptr<file_input_t> input(std::filesystem::path path) final
		{
			return std::make_shared<file_input_dummy_t>();
		}

		std::filesystem::path construct_temp_file() final
		{
			return std::filesystem::path();
		}
		bool exists(std::filesystem::path path) final
		{
			return false;
		}
		bool is_read_only(std::filesystem::path path) final
		{
			return true;
		}
		bool is_directory(std::filesystem::path path) final
		{
			return false;
		}

		void copy_file(std::filesystem::path src, std::filesystem::path trg) final
		{

		}
		void copy_directory(std::filesystem::path src, std::filesystem::path trg) final
		{

		}

		void delete_file(std::filesystem::path path) final
		{

		}
		void delete_directory(std::filesystem::path path) final
		{

		}

	private:

		class file_output_dummy_t : public file_output_t
		{

		public:

			file_output_dummy_t()
			{

			}

			~file_output_dummy_t()
			{

			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				return size;
			}

			void flush() final
			{

			}

		};

		class file_input_dummy_t : public file_input_t
		{

		public:

			file_input_dummy_t()
			{

			}

			~file_input_dummy_t()
			{

			}

			void seek(int32_t position, file_seek_origin_t origin) final
			{

			}
			bool is_eof() const final
			{
				return true;
			}
			uint32_t tell() const final
			{
				return 0;
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				return 0;
			}

		};

	};
}

#endif