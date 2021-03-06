#ifndef ENGINE_COMPONENT_LOGGER_PROVIDER_CONSOLE_HPP
#define ENGINE_COMPONENT_LOGGER_PROVIDER_CONSOLE_HPP
#pragma once

#include "global/component/logger/real.hpp"

namespace engine
{
    class logger_provider_console_t : public logger_provider_base_t
	{

	public:

        logger_provider_console_t(std::shared_ptr<console_writer_t> console_writer);

		void output(const engine::console::logger_item_t & item) const final;

    private:

        std::shared_ptr<console_writer_t> console_writer;
    };


}

#endif