#ifndef ENGINE_COMPONENT_LOGGER_DUMMY_HPP
#define ENGINE_COMPONENT_LOGGER_DUMMY_HPP
#pragma once

#include "component/logger.hpp"

namespace engine
{

	class logger_dummy_t : public logger_t
	{

	public:

		logger_items_t get_raported_items(logger_item_t::module_t module = logger_item_t::module_t::unknown) const final
		{
			return logger_items_t{};
		}

		logger_items_t get_all_items(logger_item_t::module_t module = logger_item_t::module_t::unknown) const final
		{
			return logger_items_t{};
		}

		void reset_raports(logger_item_t::module_t module = logger_item_t::module_t::unknown) final
		{
			// Do nothing, it is dummy.
		}

	private:

		item_id_t log_local(logger_item_t::level_t level, logger_item_t::module_t module, bool raport, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) final
		{
			return 0;
		}

	};
}

#endif