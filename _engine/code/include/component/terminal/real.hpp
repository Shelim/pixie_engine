#ifndef ENGINE_COMPONENT_TERMINAL_REAL_HPP
#define ENGINE_COMPONENT_TERMINAL_REAL_HPP
#pragma once

#include "component/terminal.hpp"
#include "component/logger.hpp"
#include "utility/pattern/provider.hpp"
#include "utility/pattern/enum.hpp"
#include "utility/pattern/class_settings.hpp"
#include <array>

namespace engine
{
	class terminal_provider_base_t
	{

	public:

		virtual ~terminal_provider_base_t();

		virtual std::shared_ptr<terminal_t::instance_t> open(const ustring_t & name, terminal_t::color_t background, terminal_t::closing_callback_t on_closing = [](terminal_t::instance_t*){}) = 0;

	private:
	};

	REGISTER_PROVIDER_BASE_TYPE(terminal_t, terminal_provider_base_t)

	class terminal_real_t : public terminal_t
	{

	public:

		terminal_real_t(std::shared_ptr<logger_t> logger, std::unique_ptr<holder_t<terminal_t> > terminal_provider);
		~terminal_real_t();
		std::shared_ptr<instance_t> open(const ustring_t & name, terminal_t::color_t background, closing_callback_t on_closing = [](instance_t*){}) final;

	private:

		std::shared_ptr<logger_t> logger;
		std::unique_ptr<holder_t<terminal_t> > terminal_provider;

	};
}

#include "component/terminal/provider/mockup.hpp"
#include "component/terminal/provider/windows.hpp"

#endif