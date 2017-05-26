#ifndef ENGINE_COMMON_DATA_ITEM_CONTENT_HPP
#define ENGINE_COMMON_DATA_ITEM_CONTENT_HPP
#pragma once

#include "common/ustring.hpp"
#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/data/input.hpp"
#include "common/data/output.hpp"
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
		class database_t;
		class database_items_t;

		class item_content_base_t
		{

		public:

			enum class policy_io_t
			{
				forbidden,
				implicit_sync,
				implicit_async,
				implicit_async_copy,
				explicit_sync,
				explicit_async,
				explicit_async_copy
			};

			enum class result_t
			{
				success,
				already_started,
				error
			};

			virtual ~item_content_base_t()
			{

			}

			policy_io_t get_reload_policy() const
			{
				return reload_policy;
			}

			policy_io_t get_resave_policy() const
			{
				return resave_policy;
			}

			void destroy()
			{
				std::lock_guard<std::recursive_mutex> guard(mutex_io_operation);

				if(!is_flag(flag_t::destroyed))
					destroy_local();

				set_flag(flag_t::destroyed, true);
			}

			bool is_reloading()
			{
				return get_input();
			}

			bool is_resaving()
			{
				return get_output();
			}

			bool is_io_pending()
			{
				return is_reloading() || is_resaving();
			}

			bool is_destroyed()
			{
				return is_flag(flag_t::destroyed);
			}

			friend class item_t;

		protected:

			item_content_base_t(item_t * owner, policy_io_t reload_policy = policy_io_t::implicit_async, policy_io_t resave_policy = policy_io_t::explicit_async) :
				owner(owner), reload_policy(reload_policy), resave_policy(resave_policy)
			{

			}

			item_content_base_t(const item_content_base_t & other) : owner(other.owner), reload_policy(other.reload_policy), resave_policy(other.resave_policy)
			{

			}

			void set_dirty();

			void wait_till_io_done()
			{
				std::lock_guard<std::recursive_mutex> guard(mutex_io_operation);
			}

		private:

			virtual item_content_base_t * clone() const = 0;

			item_t * get_owner()
			{
				return owner;
			}

			result_t reload_init(std::unique_ptr<input_t> input, database_t * database, bool force = false)
			{
				if (force)
				{
					mutex_io_operation.lock();
				}
				else
				{
					if (!mutex_io_operation.try_lock())
						return result_t::already_started;
				}

				this->input = std::move(input);

				if (!reload_init_local(get_input(), database))
				{
					destroy();
					this->input.reset();
					mutex_io_operation.unlock();
					return result_t::error;
				}
				return result_t::success;
			}

			result_t reload_async(database_items_t * items)
			{
				
				if (!reload_async_local(get_input(), items))
				{
					destroy();
					this->input.reset();
					mutex_io_operation.unlock();
					return result_t::error;
				}
				return result_t::success;
			}

			result_t reload_end(database_t * database)
			{
				if (reload_end_local(get_input(), database))
					set_flag(flag_t::destroyed, false);
				else
				{
					destroy();
					mutex_io_operation.unlock();
					return result_t::error;
				}

				this->input.reset();
				mutex_io_operation.unlock();
				return result_t::success;
			}

			result_t resave_init(std::unique_ptr<output_t> output, database_t * database)
			{
				this->output = std::move(output);

				return resave_init_local(get_output(), database) ? result_t::success : result_t::error;
			}

			result_t resave_async(database_items_t * items)
			{
				return resave_async_local(get_output(), items) ? result_t::success : result_t::error;
			}

			result_t resave_end(database_t * database)
			{
				bool ret = resave_end_local(get_output(), database);

				this->output.reset();

				return ret ? result_t::success : result_t::error;
			}

			input_t * get_input()
			{
				return input.get();
			}

			output_t * get_output()
			{
				return output.get();
			}

			item_t * owner;
			policy_io_t reload_policy;
			policy_io_t resave_policy;

			std::recursive_mutex mutex_io_operation;

			enum class flag_t
			{
				destroyed,

				count
			};

			std::bitset < static_cast<std::size_t>(flag_t::count)> flags;

			void set_flag(flag_t flag, bool value)
			{
				flags.set(static_cast<std::size_t>(flag), value);
			}

			bool is_flag(flag_t flag) const
			{
				return flags.test(static_cast<std::size_t>(flag));
			}

			std::unique_ptr<input_t> input;
			std::unique_ptr<output_t> output;

			virtual bool reload_init_local(input_t *, database_t * database)
			{
				return true;
			}

			virtual bool reload_async_local(input_t *, database_items_t * items)
			{
				return true;
			}

			virtual bool reload_end_local(input_t *, database_t * database)
			{
				return true;
			}

			virtual bool resave_init_local(output_t *, database_t * database)
			{
				return true;
			}

			virtual bool resave_async_local(output_t *, database_items_t * items)
			{
				return true;
			}

			virtual bool resave_end_local(output_t *, database_t * database)
			{
				return true;
			}

			virtual void destroy_local() = 0;

		};

	}

}

#endif