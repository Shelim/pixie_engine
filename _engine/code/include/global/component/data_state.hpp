#ifndef ENGINE_COMPONENT_DATA_STATE_HPP
#define ENGINE_COMPONENT_DATA_STATE_HPP
#pragma once

#include "global/core/data/input.hpp"
#include "global/core/data/output.hpp"
#include "global/core/data/provider_actual.hpp"

namespace engine
{

	class data_state_t
	{

	public:

		virtual ~data_state_t()
		{

		} 

		typedef std::unordered_set<virtual_path_t::path_t> items_t;

		virtual std::unique_ptr<data::input_t> get_input(const virtual_path_t & filename) const = 0;
		virtual std::unique_ptr<data::output_t> get_output(const virtual_path_t & filename) const = 0;
		virtual bool is_read_only(const virtual_path_t & filename) const = 0;
		virtual std::filesystem::file_time_type get_time_last_mod(const virtual_path_t & filename) const = 0;

		virtual const items_t & get_files(const virtual_path_t & path) const = 0;
		virtual const items_t & get_directories(const virtual_path_t & path) const = 0;

		virtual void update_provider(std::unique_ptr<data::provider_actual_t> provider) = 0;

	};

}

#include "global/component/data_state/dummy.hpp"
#include "global/component/data_state/real.hpp"

#endif