#ifndef ENGINE_COMPONENT_FILESYSTEM_PROVIDER_GENERIC_HPP
#define ENGINE_COMPONENT_FILESYSTEM_PROVIDER_GENERIC_HPP
#pragma once

#include "global/component/filesystem/real.hpp"
#include "utility/text/ustring.hpp"
#include <mutex>
#include <fstream>

namespace engine
{

	class filesystem_provider_generic_t : public filesystem_provider_base_t
	{

	private:

		class file_output_generic_t : public file_output_t
		{

		public:

			file_output_generic_t(std::filesystem::path physical_path);
			~file_output_generic_t();

			uint32_t write(const uint8_t * buffer, uint32_t size) final;
			void flush() final;

		private:

			std::ofstream file;

			std::filesystem::path physical_path;
		};

		class file_input_generic_t : public file_input_t
		{

		public:

			file_input_generic_t(std::filesystem::path path);
			~file_input_generic_t();

			void seek(int32_t position, file_seek_origin_t origin) final;
			bool is_eof() const final;
			uint32_t tell() const final;
			uint32_t read(uint8_t * buffer, uint32_t size) final;

		private:

			mutable std::ifstream file;
		};

	public:

		filesystem_provider_generic_t();

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

		int tmp_item;
		std::mutex tmp_mutex;

		std::filesystem::path naive_uncomplete(std::filesystem::path const p, std::filesystem::path const base);
	};
}

#endif