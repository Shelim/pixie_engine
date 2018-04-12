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
					deleted = true;
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
				return deleted;
			}

			provider_actual_t(provider_actual_t const&) = default;
			provider_actual_t(provider_actual_t &&) = default;
			provider_actual_t& operator=(provider_actual_t const&) = default;
			provider_actual_t& operator=(provider_actual_t &&) = default;

		protected:

			provider_actual_t(const virtual_path_t & virtual_path, std::filesystem::file_time_type time_last_mod) : virtual_path(virtual_path), time_last_mod(time_last_mod), deleted(false)
			{

			}

		private:

			virtual bool delete_target_local() = 0;
			virtual std::unique_ptr<input_t> construct_input_local()  = 0;
			virtual std::unique_ptr<output_t> construct_output_local() = 0;

			virtual_path_t virtual_path;
			std::filesystem::file_time_type time_last_mod;
			bool deleted;
		};
	}

}

#endif