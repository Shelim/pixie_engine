#ifndef ENGINE_COMMON_DATA_ITEM_OPERATION_HPP
#define ENGINE_COMMON_DATA_ITEM_OPERATION_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
#include "common/logger.hpp"
#include <bitset>
#include <map>
#include <vector>
#include <string>
#include <unordered_set>
#include <thread>
#include <memory>
#include <queue>
#include <mutex>

namespace engine
{
	namespace data
	{
		class item_generic_t;

		class item_operation_t  final
		{

		public:

			class step_t
			{

			public:

				enum class caller_t
				{
					sync_force_main_thread,
					sync,
					async
				};

				step_t(uint32_t id, caller_t caller) : id(id), caller(caller)
				{
					
				}

				step_t(const step_t & other) = default;

				uint32_t get_id() const
				{
					return id;
				}

				caller_t get_caller() const
				{
					return caller;
				}

			private:

				uint32_t id;
				caller_t caller;

			};

			enum class type_t
			{
				save,
				load,
				free,

				count
			};

			enum class result_t
			{
				success,
				error,
				already_started
			};

			struct free_t
			{

			};

			enum class flag_t
			{
				has_completed_executed,

				count
			};

			item_operation_t(std::shared_ptr<item_generic_t> target, std::unique_ptr<input_t> input, std::shared_ptr<logger_t> logger = nullptr) : logger(logger), target(target), type(type_t::load), current_step(0), input(std::move(input)), result(result_t::success)
			{
				init();
			}

			item_operation_t(std::shared_ptr<item_generic_t> target, std::unique_ptr<output_t> output, std::shared_ptr<logger_t> logger = nullptr) : logger(logger), target(target), type(type_t::save), current_step(0), output(std::move(output)), result(result_t::success)
			{
				init();
			}

			item_operation_t(std::shared_ptr<item_generic_t> target, free_t free, std::shared_ptr<logger_t> logger = nullptr) : logger(logger), target(target), type(type_t::free), current_step(0), result(result_t::success)
			{
				init();
			}

			~item_operation_t();

			void add_step(uint32_t id, step_t::caller_t caller)
			{
				steps.emplace_back(id, caller);
			}

			void execute_steps(step_t::caller_t caller);

			input_t * get_input()
			{
				return input.get();
			}

			output_t * get_output()
			{
				return output.get();
			}

			type_t get_type()
			{
				return type;
			}

			result_t get_result()
			{
				return result;
			}

			bool is_completed()
			{
				return (current_step >= steps.size());
			}

		private:

			void init();

			bool execute_step(step_t::caller_t caller);

			std::shared_ptr<item_generic_t> target;
			type_t type;

			int32_t current_step;
			std::vector<step_t> steps;
			
			result_t result;

			std::unique_ptr<input_t> input;
			std::unique_ptr<output_t> output;

			std::shared_ptr<logger_t> logger;
			logger_t::item_id_t task;

			std::recursive_mutex mutex_completed;

			std::bitset<static_cast<std::size_t>(flag_t::count)> flags;

			void set_flag(flag_t flag, bool value)
			{
				flags.set(static_cast<std::size_t>(flag), value);
			}

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

		};

	}

}

#endif