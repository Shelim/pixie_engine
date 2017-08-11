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

		virtual ~tasks_t()
		{

		}

		virtual void start_task(std::unique_ptr<task_base_t> task) = 0;

		class runner_t
		{

		public:

			virtual ~runner_t()
			{

			}

			virtual void update_top_task() = 0;
			virtual void update_tasks(std::chrono::duration<double> min_available_time) = 0;
		};

		virtual std::unique_ptr<runner_t> construct_runner(task_base_t::runner_t runner) = 0;

	private:
		
		

	};

}

#include "component/tasks/dummy.hpp"
#include "component/tasks/real.hpp"

#endif