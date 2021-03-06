#include "global/core/messenger/msg/console.hpp"
#include "global/core/messenger/instance.hpp"
#include "global/core/messenger/msg.hpp"
#include "global/core/messenger.hpp"

engine::messenger::msg_console_t::actual_t::~actual_t()
{

}

engine::app_t::kind_t engine::messenger::msg_console_t::actual_t::get_app() const
{
	return app;
}

engine::app_t::instance_id_t engine::messenger::msg_console_t::actual_t::get_app_instance_id() const
{
	return app_instance_id;
}

engine::messenger::msg_console_t::actual_t::type_t engine::messenger::msg_console_t::actual_t::get_type() const
{
	return type;
}

engine::messenger::msg_console_t::actual_t::source_t engine::messenger::msg_console_t::actual_t::get_source() const
{
	return source;
}

engine::messenger::msg_console_t::actual_t::kind_t engine::messenger::msg_console_t::actual_t::get_kind() const
{
	return kind;
}

engine::messenger::msg_console_t::actual_t::actual_t(app_t::kind_t app, app_t::instance_id_t app_instance_id, type_t type, source_t source, kind_t kind) : app(app), app_instance_id(app_instance_id), type(type), source(source), kind(kind)
{

}

engine::messenger::msg_console_t::msg_console_t(std::unique_ptr<actual_t> actual) : actual(std::move(actual))
{

}

engine::messenger::msg_console_t::actual_t * engine::messenger::msg_console_t::get()
{
	return actual.get();
}

engine::messenger::msg_base_t::~msg_base_t()
{

}

engine::messenger::msg_base_t::type_t engine::messenger::msg_base_t::get_type() const
{
	return type;
}

engine::messenger::msg_base_t::msg_base_t(type_t type) : type(type)
{

}

/*

#include "global/core/messenger.hpp"
#include "global/core/messenger/msg_shutdown.hpp"
#include "global/core/messenger/msg_kill_messanger.hpp"


engine::messenger_t::messenger_t()
{

	messages_thread = std::thread([this] { messages_thread_func(); });

}

engine::messenger_t::~messenger_t()
{
	post_message(std::make_unique<msg_kill_messanger_t>());
	try
	{
		messages_thread.join();
	}
	catch (const std::system_error& e) {}
}

void engine::messenger_t::dispatch_message(msg_base_t * msg)
{
	const auto & iter = callbacks.find(msg->get_type());
	if (iter != callbacks.end())
		iter->second.call(msg);
}

void engine::messenger_t::messages_thread_func()
{
	for (;;)
	{
		std::unique_ptr<msg_base_t> item = std::move(messages.pop());

		dispatch_message(item.get());

		if (item->get_type() == msg_kill_messanger_t::type)
			return;

		}
}

*/