#ifndef ENGINE_COMPONENT_FILESYSTEM_PROVIDER_GENERIC_HPP
#define ENGINE_COMPONENT_FILESYSTEM_PROVIDER_GENERIC_HPP
#pragma once

#include "component/filesystem/real.hpp"
#include "utility/text/parser.hpp"
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

			file_output_generic_t(std::filesystem::path physical_path) : physical_path(physical_path)
			{
				std::error_code ec;

				std::filesystem::path dir = physical_path;
				dir.remove_filename();
				if (!std::filesystem::is_directory(dir, ec))
					create_directories(dir, ec);

				file.open(physical_path, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
			}

			~file_output_generic_t()
			{
				file.close();
			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				file.write(reinterpret_cast<const char*>(buffer), size);
				return size;
			}

			void flush() final
			{
				file.flush();
			}

		private:

			std::ofstream file;

			std::filesystem::path physical_path;
		};

		class file_input_generic_t : public file_input_t
		{

		public:

			file_input_generic_t(std::filesystem::path path)
			{
				file.open(path, std::ios_base::in | std::ios_base::binary);
			}

			~file_input_generic_t()
			{
				file.close();
			}

			void seek(int32_t position, file_seek_origin_t origin) final
			{
				switch (origin)
				{
				case file_seek_origin_t::begin: file.seekg(position, std::ios_base::beg); break;
				case file_seek_origin_t::current: file.seekg(position, std::ios_base::cur); break;
				case file_seek_origin_t::end: file.seekg(position, std::ios_base::end); break;
				}
			}
			bool is_eof() const final
			{
				return file.eof();
			}
			uint32_t tell() const final
			{
				return file.tellg();
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				file.read(reinterpret_cast<char *>(buffer), size);

				return size;
			}

		private:

			mutable std::ifstream file;
		};

	public:

		filesystem_provider_generic_t() : tmp_item(0)
		{

		}

		std::filesystem::file_time_type get_mod_time(std::filesystem::path path) final
		{
			std::error_code ec;
			return std::filesystem::last_write_time(path, ec);
		}

		paths_t iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final
		{
			paths_t ret;
			std::error_code ec;

			for (auto & item : std::filesystem::directory_iterator(path, ec))
			{
				if (std::filesystem::is_regular_file(item.status()))
				{
					std::string str = item.path().filename().string();
					if (match(pattern.get_cstring(), str.c_str()))
					{
						ret.push_back(item.path().filename());
					}
				}
			}

			return ret;
		}
		paths_t iterate_directories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final
		{
			paths_t ret;
			std::error_code ec;

			for (auto & item : std::filesystem::directory_iterator(path, ec))
			{
				if (std::filesystem::is_directory(item.status()))
				{
					std::string str = item.path().filename().string();
					if (match(pattern.get_cstring(), str.c_str()))
					{
						ret.push_back(item.path().filename());
					}
				}
			}

			return ret;
		}
		paths_t iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final
		{
			paths_t ret;
			std::error_code ec;

			for (auto & item : std::filesystem::recursive_directory_iterator(path, ec))
			{
				if (std::filesystem::is_regular_file(item.status()))
				{
					std::string str = item.path().filename().string();
					if (match(pattern.get_cstring(), str.c_str()))
					{
						ret.push_back(naive_uncomplete(item.path(), path));
					}
				}
			}

			return ret;
		}
		paths_t iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final
		{
			paths_t ret;
			std::error_code ec;

			for (auto & item : std::filesystem::recursive_directory_iterator(path, ec))
			{
				if (std::filesystem::is_directory(item.status()))
				{
					std::string str = item.path().filename().string();
					if (match(pattern.get_cstring(), str.c_str()))
					{
						ret.push_back(naive_uncomplete(item.path(), path));
					}
				}
			}

			return ret;
		}

		std::shared_ptr<file_output_t> output(std::filesystem::path path) final
		{
			return std::make_shared<file_output_generic_t>(path);
		}

		std::shared_ptr<file_input_t> input(std::filesystem::path path) final
		{
			return std::make_shared<file_input_generic_t>(path);
		}

		std::filesystem::path construct_temp_file() final
		{
			std::lock_guard<std::mutex> guard(tmp_mutex);

			std::error_code ec;
			std::filesystem::path path = std::filesystem::temp_directory_path(ec);
			std::filesystem::path tmp_file;

			do
			{

				tmp_file = path;

				ustring_t filename = format_string("#1#_#2#.tmp"_u, ustring_t::from_utf8(PIXIE_PROJECT_NAME), ++tmp_item);

				tmp_file /= filename.get_cstring();

			} while (std::filesystem::exists(tmp_file));

			std::ofstream file;
			file.open(tmp_file, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
			file.close();
			
			return tmp_file;
		}

		bool exists(std::filesystem::path path) final
		{
			std::error_code ec;
			return std::filesystem::exists(path, ec);
		}

		bool is_read_only(std::filesystem::path path) final
		{
			return false;
		}

		bool is_directory(std::filesystem::path path) final
		{
			std::error_code ec;
			return std::filesystem::is_directory(path, ec);
		}

		void copy_file(std::filesystem::path src, std::filesystem::path trg) final
		{
			std::error_code ec;
			std::filesystem::copy_file(src, trg, std::filesystem::copy_options::overwrite_existing, ec);
		}

		void copy_directory(std::filesystem::path src, std::filesystem::path trg) final
		{
			std::error_code ec;
			std::filesystem::copy(src, trg, std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive, ec);
		}

		void delete_file(std::filesystem::path path) final
		{
			std::error_code ec;
			std::filesystem::remove(path, ec);
		}

		void delete_directory(std::filesystem::path path) final
		{
			std::error_code ec;
			std::filesystem::remove_all(path, ec);
		}


	private:

		int tmp_item;
		std::mutex tmp_mutex;

		std::filesystem::path naive_uncomplete(std::filesystem::path const p, std::filesystem::path const base)
		{

			using std::filesystem::path;

			if (p == base)
				return "./";
			/*!! this breaks stuff if path is a filename rather than a directory,
			which it most likely is... but then base shouldn't be a filename so... */

			std::filesystem::path from_path, from_base, output;

			std::filesystem::path::iterator path_it = p.begin(), path_end = p.end();
			std::filesystem::path::iterator base_it = base.begin(), base_end = base.end();

			// check for emptiness
			if ((path_it == path_end) || (base_it == base_end))
				throw std::runtime_error("path or base was empty; couldn't generate relative path");

#ifdef WIN32
			// drive letters are different; don't generate a relative path
			if (*path_it != *base_it)
				return p;

			// now advance past drive letters; relative paths should only go up
			// to the root of the drive and not past it
			++path_it, ++base_it;
#endif

			// Cache system-dependent dot, double-dot and slash strings
			const std::string _dot = ".";
			const std::string _dots = "..";
			const std::string _sep = "/";

			// iterate over path and base
			while (true) {

				// compare all elements so far of path and base to find greatest common root;
				// when elements of path and base differ, or run out:
				if ((path_it == path_end) || (base_it == base_end) || (*path_it != *base_it)) {

					// write to output, ../ times the number of remaining elements in base;
					// this is how far we've had to come down the tree from base to get to the common root
					for (; base_it != base_end; ++base_it) {
						if (*base_it == _dot)
							continue;
						else if (*base_it == _sep)
							continue;

						output /= "../";
					}

					// write to output, the remaining elements in path;
					// this is the path relative from the common root
					std::filesystem::path::iterator path_it_start = path_it;
					for (; path_it != path_end; ++path_it) {

						if (path_it != path_it_start)
							output /= "/";

						if (*path_it == _dot)
							continue;
						if (*path_it == _sep)
							continue;

						output /= *path_it;
					}

					break;
				}

				// add directory level to both paths and continue iteration
				from_path /= path(*path_it);
				from_base /= path(*base_it);

				++path_it, ++base_it;
			}

			return output;
		}
	};
}

#endif