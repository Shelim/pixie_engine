#ifndef ENGINE_COMMON_UTILITY_MESSENGER_MSG_SCANNER_FOUND_ITEM_HPP
#define ENGINE_COMMON_UTILITY_MESSENGER_MSG_SCANNER_FOUND_ITEM_HPP
#pragma once

#include <cstdint>
#include "utility/messenger/msg_base.hpp"

namespace engine
{

	namespace data
	{
		class results_t;
		class provider_actual_t;
	}

	class msg_scanner_found_item_t : public msg_base_t
	{

	public:

		static const id_t type;

		msg_scanner_found_item_t() : msg_base_t(type), results(nullptr), provider(nullptr)
		{

		}

		void set(data::results_t * results, data::provider_actual_t * provider)
		{
			result = result_t::accepted;
			this->results = results;
			this->provider = provider;
		}

		data::results_t * get_results()
		{
			return results;
		}

		data::provider_actual_t * get_provider()
		{
			return provider;
		}

		void reject()
		{
			result = result_t::rejected;
		}

		bool is_accepted()
		{
			return result == result_t::accepted;
		}

		enum class result_t
		{
			accepted,
			rejected
		};

	private:

		result_t result;
		data::results_t * results;
		data::provider_actual_t * provider;

	};

}

#endif