#include "utility/messenger/messenger.hpp"


engine::messenger_t::messenger_t()
{

	messages_thread = std::thread([this] { messages_thread_func(); });

}

engine::messenger_t::~messenger_t()
{
	post_message(std::make_unique<msg_kill_messanger_t>());
	messages_thread.join();
}

void engine::messenger_t::dispatch_message(msg_base_t * msg)
{
	auto & iter = callbacks.find(msg->get_type());
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