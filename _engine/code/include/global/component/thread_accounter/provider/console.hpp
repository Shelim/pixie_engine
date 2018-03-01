#ifndef ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_CONSOLE_HPP
#define ENGINE_COMPONENT_THREAD_ACCOUNTER_PROVIDER_CONSOLE_HPP
#pragma once

#include "global/component/thread_accounter.hpp"
#include "global/core/messenger.hpp"
#include "global/component/logger.hpp"
#include "global/core/process/service.hpp"
#include "global/core/console/msg/accountable.hpp"

namespace engine
{
	
	class thread_accounter_provider_console_t : public thread_accounter_provider_base_t
	{

	public:

		thread_accounter_provider_console_t(std::shared_ptr<console_writer_t> console_writer) : console_writer(console_writer)
		{

		}
		
		void thread_created(std::shared_ptr<thread_meta_t> thread_meta) final
		{
			console_writer->write(std::make_unique<engine::console::thread_accountable_item_t>(thread_meta, console::accountable_type_t::created));
		}

		void thread_destroyed(std::shared_ptr<thread_meta_t> thread_meta) final
		{
			console_writer->write(std::make_unique<engine::console::thread_accountable_item_t>(thread_meta, console::accountable_type_t::destroyed));
		}

	private:

		std::shared_ptr<console_writer_t> console_writer;

	};

}

#endif