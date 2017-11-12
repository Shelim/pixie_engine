/*

#include "core/messenger/messenger.hpp"
#include "core/messenger/msg_shutdown.hpp"
#include "core/messenger/msg_kill_messanger.hpp"


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