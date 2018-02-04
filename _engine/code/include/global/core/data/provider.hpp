#ifndef ENGINE_COMMON_UTILITY_DATA_PROVIDER_HPP
#define ENGINE_COMMON_UTILITY_DATA_PROVIDER_HPP
#pragma once

#include "global/core/data/provider_actual.hpp"
#include "global/core/data/input/void.hpp"
#include "global/core/data/output/void.hpp"
#include "global/component/data_creator.hpp"
#include <set>

namespace engine
{
	namespace data
	{
		class provider_t final
		{

		public:
			
			provider_t(provider_t const&) = delete;
			provider_t(provider_t &&) = default;
			provider_t& operator=(provider_t const&) = delete;
			provider_t& operator=(provider_t &&) = default;

			const virtual_path_t & get_virtual_path() const
			{
				return virtual_path;
			}

			provider_t()
			{

			}

			bool is_valid() const
			{
				return !providers.empty();
			}

			std::unique_ptr<input_t> construct_input() const
			{
				if (!is_valid()) return std::make_unique<input_void_t>(virtual_path);

				return std::move((*providers.begin())->construct_input());
			}

			std::unique_ptr<output_t> construct_output(std::shared_ptr<data_creator_t> data_creator = nullptr) const
			{
				for (auto & iter : providers)
				{
					if (!iter->is_read_only())
						return std::move(iter->construct_output());
				}
				if (!data_creator)
					return std::move(std::make_unique<output_void_t>(virtual_path));

				return std::move(data_creator->create_new_file(virtual_path));
			}

			bool is_read_only() const
			{
				for (auto & iter : providers)
				{
					if (!iter->is_read_only()) return false;
				}
				return true;
			}

			bool is_deleted() const
			{
				for (auto & iter : providers)
				{
					if (!iter->is_deleted()) return false;
				}
				return true;

			}

			bool delete_newest()
			{
				for (auto & iter : providers)
				{
					if (!iter->is_deleted())
					{
						if (!iter->delete_target()) continue;
						else return true;
					}
				}
				return false;
			}

			bool delete_all()
			{
				bool ret = true;
				for (auto & iter : providers)
				{
					if (!iter->is_deleted())
					{
						if (!iter->delete_target())
							ret = false;
					}
				}
				return ret;
			}

			std::filesystem::file_time_type get_time_last_mod() const
			{
				if (!is_valid()) return std::filesystem::file_time_type::min();
				return (*providers.begin())->get_time_last_mod();
			}

			bool add_provider_actual(std::unique_ptr<provider_actual_t> provider);

		private:

			struct providers_comparator
			{
				bool operator() (const std::unique_ptr<provider_actual_t> & left, const std::unique_ptr<provider_actual_t> &right)
				{
					return left->get_time_last_mod() > right->get_time_last_mod();
				}
			};

		public:
			
			typedef std::set<std::unique_ptr<provider_actual_t>, providers_comparator> providers_t;

			const providers_t & get_all_providers() const
			{
				return providers;
			}
			
		private:

			virtual_path_t virtual_path;

			providers_t providers;

		};
	}

}

#endif