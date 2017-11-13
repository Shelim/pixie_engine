#ifndef ENGINE_COMPONENT_DATA_ARCHIVES_REAL_HPP
#define ENGINE_COMPONENT_DATA_ARCHIVES_REAL_HPP
#pragma once

#include "component/data_archives.hpp"
#include "utility/messenger/messenger.hpp"
#include "utility/pattern/class_settings.hpp"

namespace engine
{
	SETTINGS_TABLE_START(data_archives_t)

#define ENGINE_ARCHIVE_READABLE_STD(archive) SETTINGS_TABLE_ENTRY(ustring_collection_t, allowed_extensions_for_##archive)
#include "def/archive.def"

	SETTINGS_TABLE_END()

	class data_archives_real_t : public data_archives_t
	{

	public:

		data_archives_real_t(std::shared_ptr<messenger_t> messenger, std::unique_ptr<settings_t<data_archives_t>> settings);

		~data_archives_real_t()
		{

		}

	private:

		std::shared_ptr<messenger_t> messenger;

		callback_container_t callbacks_container;

		std::unique_ptr<settings_t<data_archives_t>> settings;

		void on_scanner_found_item(msg_base_t * msg);

	};
}

#endif