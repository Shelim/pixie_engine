#ifndef ENGINE_COMPONENT_APP_ACCOUNTER_PROVIDER_CONSOLE_HPP
#define ENGINE_COMPONENT_APP_ACCOUNTER_PROVIDER_CONSOLE_HPP
#pragma once

#include "global/component/app_accounter.hpp"
#include "global/core/messenger.hpp"
#include "global/component/logger.hpp"
#include "global/core/process/service.hpp"
#include "global/core/console/msg/accountable.hpp"

namespace engine
{
	
	class app_accounter_provider_console_t : public app_accounter_provider_base_t
	{

	public:

		app_accounter_provider_console_t(std::shared_ptr<console_writer_t> console_writer) : console_writer(console_writer)
		{

		}
		
		void app_created(std::shared_ptr<app_meta_t> app_meta) final
		{
			console_writer->write(std::make_unique<engine::console::app_accountable_item_t>(app_meta, console::accountable_type_t::created));
		}

		void app_destroyed(std::shared_ptr<app_meta_t> app_meta) final
		{
			console_writer->write(std::make_unique<engine::console::app_accountable_item_t>(app_meta, console::accountable_type_t::destroyed));
		}

	private:

		std::shared_ptr<console_writer_t> console_writer;

	};

}

#endif