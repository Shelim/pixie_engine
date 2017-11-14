#ifndef ENGINE_CORE_CONSOLE_WRITER_HPP
#define ENGINE_CORE_CONSOLE_WRITER_HPP
#pragma once

#include "utility/pattern/writer.hpp"
#include "core/messenger/queue.hpp"
#include "core/messenger/msg/console.hpp"

namespace engine
{
	class console_writer_t : public writer_base_t<messenger::msg_console_t::actual_t>
	{

	public:

		console_writer_t(std::shared_ptr<messenger_console_t> console);

	private:

		std::shared_ptr<messenger_console_t> console;

		void write_local(std::unique_ptr<messenger::msg_console_t::actual_t> msg) final;

	};


}

#endif