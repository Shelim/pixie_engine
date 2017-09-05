#include "utility/messenger/messenger.hpp"
#include "utility/messenger/msg_config_updated.hpp"
#include "utility/messenger/msg_shutdown.hpp"
#include "utility/messenger/msg_kill_messanger.hpp"
#include "utility/messenger/msg_scanner_found_item.hpp"

const engine::id_t engine::msg_config_provider_updated_t::type = engine::make_id_t<'c', 'f', 'g', 'p'>::value;
const engine::id_t engine::msg_config_updated_t::type = engine::make_id_t<'c', 'f', 'g', 'u'>::value;
const engine::id_t engine::msg_shutdown_t::type = engine::make_id_t<'s', 'h', 'u', 't'>::value;
const engine::id_t engine::msg_kill_messanger_t::type = engine::make_id_t<'k', 'i', 'l', 'l'>::value;
const engine::id_t engine::msg_scanner_found_item_t::type = engine::make_id_t<'s', 'f', 'i', 't'>::value;


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