#ifndef ENGINE_COMPONENT_DATA_PROVIDER_HPP
#define ENGINE_COMPONENT_DATA_PROVIDER_HPP
#pragma once

#include "utility/data/state.hpp"
#include "utility/data/changes.hpp"
#include "utility/data/scanners.hpp"
//#include "utility/data/item.hpp"
#include "component/logger.hpp"
#include "component/config.hpp"
#include "platform/data.hpp"
#include "utility/text/ustring.hpp"
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <thread>
#include <memory>
#include <queue>
#include <mutex>

namespace engine
{

	class data_source_t
	{

	public:

		data_source_t()
		{

		}

		virtual ~data_source_t()
		{

		}

		virtual const data::changes_t & get_changes() const = 0;

		virtual void delete_item(const virtual_path_t & filename) = 0;

		virtual void delete_item_newest(const virtual_path_t & filename) = 0;

		virtual void copy_item(const virtual_path_t & src, const virtual_path_t & dst) = 0;

		virtual std::unique_ptr<data::input_t> get_input(const virtual_path_t & filename) = 0;
		virtual std::unique_ptr<data::output_t> get_output(const virtual_path_t & filename) = 0;

		typedef std::set<virtual_path_t::type_t> types_t;

		virtual types_t get_virtual_path_type_changes() const = 0;

		bool has_virtual_path_type_changed(virtual_path_t::type_t type)
		{
			return get_virtual_path_type_changes().find(type) != get_virtual_path_type_changes().end();
		}

	private:

	};
}

#include "component/data_source/dummy.hpp"
#include "component/data_source/real.hpp"

#endif