#ifndef ENGINE_COMPONENT_LOGGER_REAL_HPP
#define ENGINE_COMPONENT_LOGGER_REAL_HPP
#pragma once

#include "component/logger.hpp"
#include "utility/text/ustring.hpp"
#include "utility/pattern/class_settings.hpp"
#include "utility/pattern/provider.hpp"
#include "core/console/msg/logger.hpp"
#include "core/console/writer.hpp"
#include "component/renderer_status.hpp"
#include "component/environment_info.hpp"

namespace engine
{
	class logger_provider_base_t
	{

	public:

		virtual ~logger_provider_base_t();

		virtual void output_start() const = 0;
		virtual void output(const engine::console::logger_item_t & item) const = 0;
		virtual void output_end() const = 0;

	private:
	};

	REGISTER_PROVIDER_BASE_TYPE(logger_t, logger_provider_base_t)
	
	class logger_real_t : public logger_t
	{

	public:

		logger_real_t(std::shared_ptr<engine::renderer_status_t> renderer_status, std::shared_ptr<engine::environment_info_t> environment_info, std::unique_ptr<holder_t<logger_t> > logger_providers);
		~logger_real_t();

		const engine::console::logger_items_t & get_cache() const final;

	private:

		engine::console::logger_items_t cache;
		
		std::shared_ptr<engine::renderer_status_t> renderer_status;
		std::unique_ptr<holder_t<logger_t> > logger_providers;

		void output_start();
		void output_end();

		std::chrono::system_clock::time_point time_start;

		item_id_t log_local(engine::console::logger_item_t::kind_t kind, engine::console::logger_item_t::source_t source, const ustring_t & file, uint32_t line, const ustring_t & function, std::size_t link, const ustring_t & message) final;
	};
}

#include "component/logger/provider/console.hpp"
#include "component/logger/provider/mockup.hpp"

#endif