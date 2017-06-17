#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_TEXT_DATA_OUTPUT_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_TEXT_DATA_OUTPUT_HPP
#pragma once

#include "common/logger.hpp"
#include "common/logger_output/provider_text_base.hpp"
#include "common/environment_info.hpp"
#include "common/data/database_providers.hpp"
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
		class database_providers_t;
	}

	namespace logger_output
	{
		
		class provider_data_output_t final : public provider_text_base_t
		{

		public:

			provider_data_output_t(std::shared_ptr<engine::data::database_providers_t> database_providers, std::shared_ptr<logger_t> logger, std::shared_ptr<environment_info_t> environment_info);

			~provider_data_output_t()
			{

			}

			void output_line(richtext_t line) final
			{
				output->write_ustring(line.get_raw());
				output->write_ustring(_U("\r\n"));
				output->force_flush();
			}

		private:

			std::unique_ptr<engine::data::output_t> output;
		};


	}

}

#endif