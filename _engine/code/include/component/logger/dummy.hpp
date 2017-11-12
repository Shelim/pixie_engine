#ifndef ENGINE_COMPONENT_LOGGER_DUMMY_HPP
#define ENGINE_COMPONENT_LOGGER_DUMMY_HPP
#pragma once

#include "component/logger.hpp"

namespace engine
{

	class logger_dummy_t : public logger_t
	{

	public:

	private:

		item_id_t log_local(engine::console::logger_item_t::kind_t kind, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) final
		{
			return 0;
		}

		const engine::console::logger_items_t & get_cache() const
		{
			return engine::console::logger_items_t{};
		}

	};
}

#endif