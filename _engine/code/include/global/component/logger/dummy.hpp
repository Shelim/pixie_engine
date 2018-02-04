#ifndef ENGINE_COMPONENT_LOGGER_DUMMY_HPP
#define ENGINE_COMPONENT_LOGGER_DUMMY_HPP
#pragma once

#include "global/component/logger.hpp"

namespace engine
{

	class logger_dummy_t : public logger_t
	{

	public:

		const engine::console::logger_items_t & get_cache() const final;

	private:

		item_id_t log_local(engine::console::logger_item_t::kind_t kind, engine::app_t::kind_t app, engine::app_t::instance_id_t instance_id, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) final;

	};
}

#endif