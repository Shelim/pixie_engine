#ifndef ENGINE_COMPONENT_ENVIRONMENT_INFO_REAL_HPP
#define ENGINE_COMPONENT_ENVIRONMENT_INFO_REAL_HPP
#pragma once

#include "component/environment_info.hpp"
#include <array>

namespace engine
{

	class environment_info_real_t : public environment_info_t
	{

	public:

		environment_info_real_t();

		const ustring_t & get(key_t key) const final
		{
			return items[static_cast<std::underlying_type<logger_item_t::level_t>::type>(key)].get_value();
		}

		status_t get_status(key_t key) const final
		{
			return items[static_cast<std::underlying_type<logger_item_t::level_t>::type>(key)].get_status();
		}

	private:

		void set_info(key_t key, const ustring_t & value, status_t status = status_t::normal)
		{
			items[static_cast<std::underlying_type<logger_item_t::level_t>::type>(key)].set(value, status);
		}

		class item_t
		{

		public:

			item_t() : status(status_t::not_yet_populated)
			{

			}

			status_t get_status() const
			{
				return status;
			}

			const ustring_t & get_value() const
			{
				return value;
			}


		private:

			friend class environment_info_real_t;

			void set(const ustring_t & value, status_t status = status_t::normal)
			{
				this->status = status;
				this->value = value;
			}

			status_t status;
			ustring_t value;
		};

		std::array<item_t, static_cast<std::underlying_type<logger_item_t::level_t>::type>(key_t::count)> items;
	};
}

#endif