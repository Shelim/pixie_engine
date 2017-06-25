#ifndef ENGINE_COMMON_ENVIRONMENT_INFO_HPP
#define ENGINE_COMMON_ENVIRONMENT_INFO_HPP
#pragma once

#include "common/utility/text/ustring.hpp"
#include "common/platform.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace engine
{


	class environment_info_t final
	{

	public:

		enum class key_t
		{
#define GAME_ENVIRONMENT_INFO_STD(key, name) key,
#include "common/std/environment_info_std.hpp"
			count
		};

		class item_t
		{

		public:

			item_t()
			{

			}
			
			const ustring_t & get_name() const
			{
				return name;
			}
			
			const ustring_t & get_value() const
			{
				return value;
			}


		private:

			friend class environment_info_t;

			void set(const ustring_t & value)
			{
				this->value = value;
			}

			ustring_t name;
			ustring_t value;
		};

		environment_info_t(std::shared_ptr<engine::platform_t> platform, std::shared_ptr<engine::manifest_app_t> manifest_app);
		~environment_info_t();

		typedef std::array<item_t, static_cast<size_t>(key_t::count)> vals_t;

		const item_t & get(key_t key)
		{
			return vals[static_cast<size_t>(key)];
		}

	private:

		void set_info(key_t info, const ustring_t & value)
		{
			vals[static_cast<size_t>(info)].set(value);
		}
		
		std::shared_ptr<engine::platform_t> platform;
		std::shared_ptr<engine::manifest_app_t> manifest_app;

		void gather_data();
		vals_t vals;

	};
}

#endif