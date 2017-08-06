#ifndef ENGINE_COMPONENT_TASKS_HPP
#define ENGINE_COMPONENT_TASKS_HPP
#pragma once

#include <pugixml.hpp>
#include <string>
#include <vector>
#include <bitset>
#include <array>
#include <mutex>
#include <map>
#include <thread>
#include <chrono>
#include <sstream>
#include <SDL.h>
#include <pugixml.hpp>
#include "utility/text/ustring.hpp"
#include "component/frame_notifier.hpp"
#include "utility/pattern/flags.hpp"
#include "component/tasks/task.hpp"
#include "utility/text/parser.hpp"
#include "utility/container/sync_queue.hpp"
#include <cereal/cereal.hpp>
#include <cereal/access.hpp>
#include <vlc/vlc.h>
#include <bitset>

namespace engine
{

	class tasks_t
	{

	public:


	private:
		
		class runner_t
		{

		public:

			void execute_one()
			{
				if (tasks.is_empty()) return;
				std::unique_ptr<task_base_t> task = std::move(tasks.pop());
			}
			
		private:

			sync_queue_t<std::unique_ptr<task_base_t>> tasks;

		};


	};

}

#include "component/tasks/dummy.hpp"
#include "component/tasks/real.hpp"

#endif