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

		virtual ~filesystem_provider_base_t();
		
		paths_t iterate_files_in_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) override;
		paths_t iterate_subdirectories(std::filesystem::path path, const ustring_t & pattern = "*"_u) override;

		void move_file(std::filesystem::path src, std::filesystem::path trg) override;
		void move_directory(std::filesystem::path src, std::filesystem::path trg) override;

	protected:

		bool match(char const *needle, wchar_t const *haystack);
		bool match(char const *needle, char const *haystack);
	};

	REGISTER_PROVIDER_BASE_TYPE(filesystem_t, filesystem_provider_base_t)

	class filesystem_real_t : public filesystem_t
	{

	public:
		
		filesystem_real_t(std::unique_ptr<holder_t<filesystem_t> > filesystem_provider);

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
		void move_file(std::filesystem::path src, std::filesystem::path trg) final;
		void move_directory(std::filesystem::path src, std::filesystem::path trg) final;

	private:

		std::unique_ptr<holder_t<filesystem_t> > filesystem_provider;

	};
}

#include "component/filesystem/provider/generic.hpp"
#include "component/filesystem/provider/windows.hpp"

#endif