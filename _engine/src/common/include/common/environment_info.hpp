#ifndef ENGINE_COMMON_ENVIRONMENT_INFO_HPP
#define ENGINE_COMMON_ENVIRONMENT_INFO_HPP
#pragma once

#include "common/ustring.hpp"
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
#define GAME_ENVIRONMENT_INFO_STD(name, lang) name,
#include "common/std/environment_info_std.hpp"
			count
		};

		class item_t
		{

		public:

			item_t()
			{

			}


			void set(const ustring_t & value)
			{
				val = value;
			}
			
			const ustring_t & get() const
			{
				return val;
			}
			
			const ustring_t & get_key() const
			{
				return key;
			}


		private:

			friend class environment_info_t;

			ustring_t key;
			ustring_t val;
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

		void generate_data();
		vals_t vals;

	};
}

#endif