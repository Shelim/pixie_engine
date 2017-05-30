#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_RAW_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_RAW_HPP
#pragma once

#include "common/logger.hpp"
#include "common/logger_output/provider_base.hpp"
#include "common/environment_info.hpp"
#include "common/data/database.hpp"
#include "common/data/item.hpp"
#include "common/data/output.hpp"
#include "common/queue.hpp"
#include "common/xml_pixie_archive.hpp"
#include "common/platform.hpp"
#include <cereal/types/vector.hpp>
#include <mutex>
#include <thread>
#include <future>
#include <string>

namespace engine
{
	namespace data
	{
		class database_t;
	}

	namespace logger_output
	{
		
		class provider_data_t final : public provider_base_t
		{

		public:

			provider_data_t(std::shared_ptr<engine::data::database_t> database, std::shared_ptr<logger_t> logger, std::shared_ptr<environment_info_t> environment_info);

			~provider_data_t()
			{

			}

			void process_item(const logger_t::item_t & item) final
			{
				ustring_t subscript = _U("");

				if (item.get_level() == logger_t::item_t::level_t::task_started)
					subscript = _U("...");
				else if (item.get_level() == logger_t::item_t::level_t::task_done)
					subscript = _U("... DONE!");
				else if (item.get_level() == logger_t::item_t::level_t::task_failed)
					subscript = _U("... FAILED!!!");

				ustring_t str = level_to_prompt(item.get_level());

				padd(str, 5);
				str.append(item.get_message());
				str.append(subscript);
				padd(str, 96);
				str.append(_U("frame: "));
				str.append(to_string(item.get_frame()));
				str.append(_U(", time: "));
				str.append(to_string(item.get_time()));
				str.append(_U(", thread: "));
				str.append(to_string(item.get_thread()));
				str.append(_U(", func: "));
				str.append(item.get_function());
				str.append(_U("()"));
				padd(str, 192);
				str.append(_U("at ': "));
				str.append(item.get_file());
				str.append(_U("' on line "));
				str.append(to_string(item.get_line()));
				str.append(_U("\r\n"));

				output->write_ustring(str);
				output->force_flush();
			}

		private:

			bool save_items_completed;
			
			ustring_t level_to_prompt(logger_t::item_t::level_t level)
			{
				if (level == logger_t::item_t::level_t::task_started)
					return _U("...");
				if (level == logger_t::item_t::level_t::task_failed)
					return _U("..!");
				if (level == logger_t::item_t::level_t::task_done)
					return _U("..>");
				if (level == logger_t::item_t::level_t::message)
					return _U("-->");
				if (level == logger_t::item_t::level_t::warning)
					return _U("-!- ");
				if (level == logger_t::item_t::level_t::error)
					return _U("!!!");
			}

			void padd(ustring_t & str, std::size_t pos)
			{
				if (str.len() >= pos - 3)
					str.append(_U("   "));
				else
				{
					std::string s(pos - str.len() + 3, ' ');
					str.append_utf8(s.c_str());
				}
			}

			std::unique_ptr<engine::data::output_t> output;

		};


	}

}

#endif