#ifndef ENGINE_COMPONENT_FILESYSTEM_REAL_HPP
#define ENGINE_COMPONENT_FILESYSTEM_REAL_HPP
#pragma once

#include "component/filesystem.hpp"
#include "utility/pattern/provider.hpp"

namespace engine
{
	
	class filesystem_provider_base_t : public filesystem_t
	{

	public:

		virtual ~filesystem_provider_base_t()
		{

		}
		
		paths_t iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) override
		{
			paths_t ret = iterate_files_in_directory(path, pattern);

			for (auto dir : iterate_directories(path))
			{
				paths_t inside = iterate_files_in_subdirectories(path / dir, pattern);

				for (uint32_t i = 0; i < inside.size(); i++)
				{
					inside[i] = dir / inside[i];
				}

				ret.insert(ret.end(), inside.begin(), inside.end());
			}

			return ret;
		}

		paths_t iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*"_u) override
		{
			paths_t ret = iterate_directories(path, pattern);
			paths_t dirs = ret;

			for (auto dir : dirs)
			{
				paths_t inside = iterate_subdirectories(path / dir, pattern);

				for (uint32_t i = 0; i < inside.size(); i++)
				{
					inside[i] = dir / inside[i];
				}

				ret.insert(ret.end(), inside.begin(), inside.end());
			}

			return ret;
		}

		void move_file(std::filesystem::path src, std::filesystem::path trg) override
		{
			copy_file(src, trg);
			delete_file(src);
		}

		void move_directory(std::filesystem::path src, std::filesystem::path trg) override
		{
			copy_directory(src, trg);
			delete_directory(src);
		}

	protected:

		bool match(char const *needle, wchar_t const *haystack)
		{
			for (; *needle != '\0'; ++needle)
			{
				switch (*needle)
				{
				case '?':
					if (*haystack == '\0')
						return false;
					++haystack;
					break;
				case '*':
				{
					if (needle[1] == '\0')
						return true;
					size_t max = wcslen(haystack);
					for (size_t i = 0; i < max; i++)
						if (match(needle + 1, haystack + i))
							return true;
					return false;
				}
				default:
					if (*haystack != *needle)
						return false;
					++haystack;
				}
			}
			return *haystack == '\0';
		}
		bool match(char const *needle, char const *haystack)
		{
			for (; *needle != '\0'; ++needle)
			{
				switch (*needle)
				{
				case '?':
					if (*haystack == '\0')
						return false;
					++haystack;
					break;
				case '*':
				{
					if (needle[1] == '\0')
						return true;
					size_t max = strlen(haystack);
					for (size_t i = 0; i < max; i++)
						if (match(needle + 1, haystack + i))
							return true;
					return false;
				}
				default:
					if (*haystack != *needle)
						return false;
					++haystack;
				}
			}
			return *haystack == '\0';
		}
	};

	REGISTER_PROVIDER_BASE_TYPE(filesystem_t, filesystem_provider_base_t)

	class filesystem_real_t : public filesystem_t
	{

	public:
		
		filesystem_real_t(std::unique_ptr<holder_t<filesystem_t> > filesystem_provider) : filesystem_provider(std::move(filesystem_provider))
		{

		}

		std::filesystem::file_time_type get_mod_time(std::filesystem::path path) final
		{
			return filesystem_provider->get_provider()->get_mod_time(path);
		}

		paths_t iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final
		{
			return filesystem_provider->get_provider()->iterate_files_in_directory(path, pattern);
		}
		paths_t iterate_directories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final
		{
			return filesystem_provider->get_provider()->iterate_directories(path, pattern);
		}
		paths_t iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final
		{
			return filesystem_provider->get_provider()->iterate_files_in_subdirectories(path, pattern);
		}
		paths_t iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final
		{
			return filesystem_provider->get_provider()->iterate_subdirectories(path, pattern);
		}

		std::shared_ptr<file_output_t> output(std::filesystem::path path) final
		{
			return filesystem_provider->get_provider()->output(path);
		}
		std::shared_ptr<file_input_t> input(std::filesystem::path path) final
		{
			return filesystem_provider->get_provider()->input(path);
		}

		std::filesystem::path construct_temp_file() final
		{
			return filesystem_provider->get_provider()->construct_temp_file();
		}
		bool exists(std::filesystem::path path) final
		{
			return filesystem_provider->get_provider()->exists(path);
		}
		bool is_read_only(std::filesystem::path path) final
		{
			return filesystem_provider->get_provider()->is_read_only(path);
		}
		bool is_directory(std::filesystem::path path) final
		{
			return filesystem_provider->get_provider()->is_directory(path);
		}

		void copy_file(std::filesystem::path src, std::filesystem::path trg) final
		{
			filesystem_provider->get_provider()->copy_file(src, trg);
		}
		void copy_directory(std::filesystem::path src, std::filesystem::path trg) final
		{
			filesystem_provider->get_provider()->copy_directory(src, trg);
		}

		void delete_file(std::filesystem::path path) final
		{
			filesystem_provider->get_provider()->delete_file(path);
		}
		void delete_directory(std::filesystem::path path) final
		{
			filesystem_provider->get_provider()->delete_directory(path);
		}
		void move_file(std::filesystem::path src, std::filesystem::path trg) final
		{
			filesystem_provider->get_provider()->move_file(src, trg);
		}
		void move_directory(std::filesystem::path src, std::filesystem::path trg) final
		{
			filesystem_provider->get_provider()->move_directory(src, trg);
		}

	private:

		std::unique_ptr<holder_t<filesystem_t> > filesystem_provider;

	};
}

#include "component/filesystem/provider/generic.hpp"
#include "component/filesystem/provider/windows.hpp"

#endif