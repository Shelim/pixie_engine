#ifndef ENGINE_COMPONENT_FILESYSTEM_DUMMY_HPP
#define ENGINE_COMPONENT_FILESYSTEM_DUMMY_HPP
#pragma once

#include "global/component/filesystem.hpp"

namespace engine
{

	class filesystem_dummy_t : public filesystem_t
	{

	public:

		std::filesystem::file_time_type get_mod_time(std::filesystem::path path) final;

		paths_t iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final;
		paths_t iterate_directories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final;
		paths_t iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final;
		paths_t iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final;

		std::shared_ptr<file_output_t> output(std::filesystem::path path) final;
		std::shared_ptr<file_input_t> input(std::filesystem::path path) final;

		std::filesystem::path construct_temp_file() final;
		bool exists(std::filesystem::path path) final;
		bool is_read_only(std::filesystem::path path) final;
		bool is_directory(std::filesystem::path path) final;

		void copy_file(std::filesystem::path src, std::filesystem::path trg) final;
		void copy_directory(std::filesystem::path src, std::filesystem::path trg) final;

		void delete_file(std::filesystem::path path) final;
		void delete_directory(std::filesystem::path path) final;

	private:

		class file_output_dummy_t : public file_output_t
		{

		public:

			file_output_dummy_t();
			~file_output_dummy_t();

			uint32_t write(const uint8_t * buffer, uint32_t size) final;
			void flush() final;

		};

		class file_input_dummy_t : public file_input_t
		{

		public:

			file_input_dummy_t();
			~file_input_dummy_t();

			void seek(int32_t position, file_seek_origin_t origin) final;
			bool is_eof() const final;
			uint32_t tell() const final;
			uint32_t read(uint8_t * buffer, uint32_t size) final;

		};

	};
}

#endif