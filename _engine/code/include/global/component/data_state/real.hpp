#ifndef ENGINE_COMPONENT_DATA_STATE_REAL_HPP
#define ENGINE_COMPONENT_DATA_STATE_REAL_HPP
#pragma once

#include "global/component/data_state.hpp"

namespace engine
{
	class data_state_real_t : public data_state_t
	{

	public:

		data_state_real_t()
		{

		}

		std::unique_ptr<data::input_t> get_input(const virtual_path_t & filename) const final
		{
			return std::make_unique<data::input_void_t>(filename);
		}
		std::unique_ptr<data::output_t> get_output(const virtual_path_t & filename) const final
		{
			return std::make_unique<data::output_void_t>(filename);
		}
		bool is_read_only(const virtual_path_t & filename) const final
		{
			return true;
		}
		std::filesystem::file_time_type get_time_last_mod(const virtual_path_t & filename) const final
		{
			return std::filesystem::file_time_type::min();
		}

		const items_t & get_files(const virtual_path_t & path) const final
		{
			return no_items;
		}
		const items_t & get_directories(const virtual_path_t & path) const final
		{
			return no_items;
		}

		void update_provider(std::unique_ptr<data::provider_actual_t> provider) final
		{
			
		}

	private:

		items_t no_items;


	};
}


#endif