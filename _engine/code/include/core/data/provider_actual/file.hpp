#ifndef ENGINE_COMMON_UTILITY_DATA_PROVIDER_ACTUAL_FILE_HPP
#define ENGINE_COMMON_UTILITY_DATA_PROVIDER_ACTUAL_FILE_HPP
#pragma once

#include "component/filesystem.hpp"
#include "core/data/provider_actual.hpp"
#include "core/data/input/file.hpp"
#include "core/data/output/file.hpp"
#include "core/data/output/file_safe.hpp"

namespace engine
{
	namespace data
	{
		class provider_actual_file_t final : public provider_actual_t
		{

		public:

			bool is_read_only() final
			{
				return flags.is_flag(flag_t::is_read_only);
			}

			enum class read_only_t
			{
				yes,
				no
			};

			enum class use_safe_output_t
			{
				yes,
				no
			};

			provider_actual_file_t(std::shared_ptr<filesystem_t> filesystem, const virtual_path_t & virtual_path, const std::filesystem::path & physical_path, read_only_t read_only = read_only_t::yes, use_safe_output_t use_safe_output = use_safe_output_t::yes) : provider_actual_t(virtual_path, filesystem->get_mod_time(physical_path)), filesystem(filesystem), physical_path(physical_path)
			{
				if (read_only == read_only_t::yes)
					flags.set_flag(flag_t::is_read_only, true);

				if (use_safe_output == use_safe_output_t::yes)
					flags.set_flag(flag_t::use_safe_output, true);
			}

		private:

			enum class flag_t
			{
				is_read_only,
				use_safe_output,
				count
			};

			bool delete_target_local() final
			{
				std::error_code ec;
				return remove(physical_path, ec);
			}

			std::unique_ptr<input_t> construct_input_local() final
			{
				return std::make_unique<input_file_t>(get_virtual_path(), filesystem, physical_path);
			}
			std::unique_ptr<output_t> construct_output_local() final
			{
				if (flags.is_flag(flag_t::is_read_only)) return nullptr;

				if(flags.is_flag(flag_t::use_safe_output))
					return std::make_unique<output_file_safe_t>(get_virtual_path(), filesystem, physical_path);
				else
					return std::make_unique<output_file_t>(get_virtual_path(), filesystem, physical_path);
			}

			std::shared_ptr<filesystem_t> filesystem;
			std::filesystem::path physical_path;
			flags_t<flag_t> flags;
		};
	}
}

#endif