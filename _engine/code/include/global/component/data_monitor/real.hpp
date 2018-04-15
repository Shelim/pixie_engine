#ifndef ENGINE_COMPONENT_DATA_MONITOR_REAL_HPP
#define ENGINE_COMPONENT_DATA_MONITOR_REAL_HPP
#pragma once

#include "global/component/data_monitor.hpp"
#include "utility/concurrention/signal.hpp"

namespace engine
{
	class data_monitor_actual_t
	{

	public:

			
		void rescan(data::scanners_t & scanners)
		{
			for(auto & item : scanners)
			{
//				item->scan();
			}
		}

	};

	class data_monitor_executor_t
	{

	public:

		data_monitor_executor_t(std::shared_ptr<data_monitor_actual_t> actual) : actual(actual)
		{

		}

		void request_rescan(data::scanners_t && scanners)
		{
			{
				std::lock_guard<std::mutex> guard(mutex);
				if(flags.is_flag(flag_t::next_rescanning_requested))
				{
					for(std::size_t i = 0; i < scanners.size(); i++)
					{
						auto & scanner = scanners[i];
						if(std::find_if(this->scanners.begin(), this->scanners.end(), [&scanner](std::unique_ptr<data::scanner_t> & prev){ return *scanner==*prev; }) == this->scanners.end())
							this->scanners.push_back(std::move(scanner));
					}
					return;
				}
				else
				{
					flags.set_flag(flag_t::next_rescanning_requested, true);
					std::swap(this->scanners, scanners);
				}
			}
			messages.push(message_t::rescan_requested);
		}

		void terminate_pool()
		{
			messages.push(message_t::terminate_monitor);
		}

		task_base_t::result_t run()
		{
			for(;;)
			{
				data::scanners_t scanners;
				message_t msg = messages.pop();
				switch(msg)
				{
					case message_t::rescan_requested:
					{
						{
							std::lock_guard<std::mutex> guard(mutex);
							flags.set_flag(flag_t::next_rescanning_requested, false);
							std::swap(scanners, this->scanners);
						}
						actual->rescan(scanners);
						scanners.clear();
					}
					break;
					case message_t::terminate_monitor:
						return task_base_t::result_t::completed;
				}
			}
		}

	private:
		
		enum class message_t
		{
			rescan_requested,
			terminate_monitor
		};

		enum class flag_t
		{
			next_rescanning_requested,
			count
		};

		flags_t<flag_t> flags;
		std::mutex mutex;

		concurrent_queue_t<message_t> messages;
		
		data::scanners_t scanners;
		

		std::shared_ptr<data_monitor_actual_t> actual;

	};

	class data_monitor_real_t : public data_monitor_t
	{

	public:

		data_monitor_real_t()
		{

		}

	private:


	};
}


#endif