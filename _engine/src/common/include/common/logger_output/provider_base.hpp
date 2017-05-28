#ifndef ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_BASE_HPP
#define ENGINE_COMMON_LOGGER_OUTPUT_PROVIDER_BASE_HPP
#pragma once

#include <cstddef>
#include "common/logger.hpp"

namespace engine
{

	namespace logger_output
	{

		class provider_base_t
		{

		public:

			virtual ~provider_base_t()
			{

			}

			virtual void on_item_changed(const std::size_t item_changed)
			{
				if (is_enabled())
					on_item_changed_local(item_changed);
			}

			virtual void on_item_append(const logger_t::item_t & item)
			{
				if (is_enabled())
					on_item_append_local(item);
			}

			virtual void force_resave()
			{

			}

			void enable()
			{
				if (!is_enabled())
				{
					enable_local();
					enabled = true;
				}
			}
			void disable()
			{
				if (is_enabled())
				{
					disable_local();
					enabled = false;
				}
			}

			bool is_enabled()
			{
				return enabled;
			}

		protected:

			provider_base_t(std::shared_ptr<logger_t> logger) : logger(logger), enabled(false)
			{

			}

			std::shared_ptr<logger_t> get_logger()
			{
				return logger;
			}

		private:

			std::shared_ptr<logger_t> logger;
			bool enabled;

			virtual void on_item_changed_local(const std::size_t item_changed)
			{

			}

			virtual void on_item_append_local(const logger_t::item_t & item)
			{

			}

			virtual void enable_local()
			{

			}

			virtual void disable_local()
			{

			}

		};

	}

}

#endif