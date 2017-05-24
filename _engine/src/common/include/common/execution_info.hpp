#ifndef ENGINE_COMMON_EXECUTION_INFO_HPP
#define ENGINE_COMMON_EXECUTION_INFO_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/platform.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace engine
{


	class execution_info_t final
	{

	public:

		enum class key_t
		{
#define GAME_EXECUTION_INFO_STD(name, lang) name,
#include "common/std/execution_info_std.hpp"
			count
		};

		class item_t
		{

		public:

			enum class status_t : uint8_t
			{
				normal,
				good,
				bad
			};

			item_t() : status(status_t::normal)
			{

			}

			void set(const ustring_t & value, status_t status)
			{
				this->val = value;
				this->status = status;
			}

			void set_value(const ustring_t & value)
			{
				val = value;
			}

			void set_status(status_t value)
			{
				status = value;
			}

		private:

			ustring_t desc;
			
			friend class cereal::access;
			friend class execution_info_t;

			template<class archive_t> void serialize(archive_t & ar)
			{
				ar(cereal::make_nvp("value", val), cereal::make_nvp("status", status), cereal::make_nvp("desc", desc));
			}

			ustring_t val;
			status_t status;
		};

		void set_info(key_t info, const ustring_t & value, item_t::status_t status)
		{
			vals[static_cast<size_t>(info)].set(value, status);
		}

		void set_info(key_t info, const ustring_t & value)
		{
			vals[static_cast<size_t>(info)].set_value(value);
		}

		void set_status(key_t info, item_t::status_t status)
		{
			vals[static_cast<size_t>(info)].set_status(status);
		}

		execution_info_t(std::shared_ptr<engine::platform_t> platform, std::shared_ptr<engine::manifest_app_t> manifest_app);
		~execution_info_t();

		typedef std::array<item_t, static_cast<size_t>(key_t::count)> vals_t;

		vals_t & get_val_for_save();

	private:

		const ustring_t & get(key_t info)
		{
			return vals[static_cast<size_t>(info)].val;
		}

		std::chrono::system_clock::time_point time_start;

		std::shared_ptr<engine::platform_t> platform;
		std::shared_ptr<engine::manifest_app_t> manifest_app;

		void std_platform();
		vals_t vals;

	};

	template<class archive_t> void save(archive_t & ar, execution_info_t::vals_t const & vals)
	{
#define GAME_EXECUTION_INFO_STD(name, lang) ar(cereal::make_nvp(#name, vals[static_cast<std::size_t>(execution_info_t::key_t::name)]));
#include "common/std/execution_info_std.hpp"
	}

	template<class archive_t> void load(archive_t & ar, execution_info_t::vals_t & vals)
	{
#define GAME_EXECUTION_INFO_STD(name, lang) ar(cereal::make_nvp(#name, vals[static_cast<std::size_t>(execution_info_t::key_t::name)]));
#include "common/std/execution_info_std.hpp"
	}

	template <class archive_t> std::string save_minimal(archive_t const &, execution_info_t::item_t::status_t const & obj)
	{
		switch (obj)
		{
		case execution_info_t::item_t::status_t::normal: return "normal";
		case execution_info_t::item_t::status_t::bad: return "bad";
		case execution_info_t::item_t::status_t::good: return "good";
		}
		return "normal";
	}

	template <class archive_t> void load_minimal(archive_t const &, execution_info_t::item_t::status_t & obj, std::string const & value)
	{
		if (value == "normal") obj = execution_info_t::item_t::status_t::normal;
		else if (value == "bad") obj = execution_info_t::item_t::status_t::bad;
		else if (value == "good") obj = execution_info_t::item_t::status_t::good;
		else obj = execution_info_t::item_t::status_t::normal;
	}
}

#endif