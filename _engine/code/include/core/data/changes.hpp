#ifndef ENGINE_COMMON_UTILITY_DATA_CHANGES_HPP
#define ENGINE_COMMON_UTILITY_DATA_CHANGES_HPP
#pragma once

#include "core/vfs/virtual_path.hpp"
#include "core/data/provider.hpp"
#include <map>
#include <vector>
#include <string>

namespace engine
{
	namespace data
	{
		class change_t final
		{

		public:

			enum type_t : uint8_t
			{
				added,
				updated,
				deleted,
				changed_directory
			};

			const virtual_path_t & get_path() const
			{
				return path;
			}

			type_t get_type() const
			{
				return type;
			}

			change_t(const virtual_path_t & path, type_t type) : type(type), path(path)
			{

			}

		private:

			virtual_path_t path;
			type_t type;
		};

		typedef std::vector<change_t> changes_t;
	}
	
}

#endif