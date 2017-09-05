#ifndef ENGINE_COMPONENT_DATA_PROVIDER_DUMMY_HPP
#define ENGINE_COMPONENT_DATA_PROVIDER_DUMMY_HPP
#pragma once

#include "component/data_source.hpp"

namespace engine
{
	class data_source_dummy_t : public data_source_t
	{

	public:

		data_source_dummy_t()
		{
		}

		~data_source_dummy_t()
		{
		}

		const data::changes_t & get_changes() const final
		{
			return data::changes_t();
		}

		void delete_item(const virtual_path_t & filename) final
		{

		}

		void delete_item_newest(const virtual_path_t & filename) final
		{

		}

		void copy_item(const virtual_path_t & src, const virtual_path_t & dst) final
		{

		}

		std::unique_ptr<data::input_t> get_input(const virtual_path_t & filename) final
		{
			return std::move(std::make_unique<data::input_void_t>(filename));;
		}

		std::unique_ptr<data::output_t> get_output(const virtual_path_t & filename) final
		{
			return std::move(std::make_unique<data::output_void_t>(filename));
		}
			
		types_t get_virtual_path_type_changes() const final
		{
			return types_t();
		}

	};
}

#endif