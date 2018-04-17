#ifndef ENGINE_COMMON_UTILITY_DATA_MANIPULATOR_ACTUAL_HPP
#define ENGINE_COMMON_UTILITY_DATA_MANIPULATOR_ACTUAL_HPP
#pragma once

#include "utility/text/ustring.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/core/data/input.hpp"
#include "global/core/data/output.hpp"
#include "global/core/data/input/void.hpp"
#include "global/core/data/output/void.hpp"
#include "utility/pattern/flags.hpp"

namespace engine
{
	namespace data
	{
		class provider_actual_t
		{

		public:

			std::unique_ptr<input_t> construct_input()
			{
				return std::move(construct_input_local());
			}
			std::unique_ptr<output_t> construct_output()
			{
				return std::move(construct_output_local());
			}
			virtual bool is_read_only()
			{
				return true;
			}
			bool delete_target()
			{
				if(is_read_only()) return false;
				if (delete_target_local())
				{
					flags.set_flag(flag_t::deleted, true);
					return true;
				}
				return false;
			}
			virtual ~provider_actual_t() { }

			std::filesystem::file_time_type get_time_last_mod() const
			{
				return time_last_mod;
			}

			const virtual_path_t & get_virtual_path() const
			{
				return virtual_path;
			}

			bool is_deleted()
			{
				return flags.is_flag(flag_t::deleted);
			}

#define ENGINE_DATA_PROVIDER_DEF(...) DEFINE_ENUM_ONLY_1ST_TYPE(kind_t, __VA_ARGS__)
#include "def/data_provider.def"

			kind_t get_kind() const
			{
				return kind;
			}

		protected:

			provider_actual_t(kind_t kind, const virtual_path_t & virtual_path, std::filesystem::file_time_type time_last_mod) :  kind(kind), virtual_path(virtual_path), time_last_mod(time_last_mod)
			{

			}

		private:

			virtual bool delete_target_local() = 0;
			virtual std::unique_ptr<input_t> construct_input_local()  = 0;
			virtual std::unique_ptr<output_t> construct_output_local() = 0;

			kind_t kind;
			virtual_path_t virtual_path;
			std::filesystem::file_time_type time_last_mod;
			enum class flag_t
			{
				deleted,
				count
			};
			flags_t<flag_t> flags;
		};
	}

}

#define ENGINE_DATA_PROVIDER_DEF(...) DEFINE_ENUM_ONLY_1ST_TO_STRING(engine::data::provider_actual_t::kind_t, __VA_ARGS__)
#include "def/data_provider.def"

#endif