#ifndef ENGINE_COMMON_DATA_PROVIDER_HPP
#define ENGINE_COMMON_DATA_PROVIDER_HPP
#pragma once

#include "common/data/provider_actual.hpp"
#include "common/data/input_void.hpp"
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

			std::unique_ptr<output_t> construct_output() const
			{
				for (auto & iter : providers)
				{
					if (!iter->is_read_only())
						return std::move(iter->construct_output());
				}
				return std::move(platform_create_new(virtual_path));
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

			void delete_all()
			{
				for (auto & iter : providers)
				{
					if (!iter->is_deleted())
					{
						iter->delete_target();
					}
				}
			}

			std::filesystem::file_time_type get_time_last_mod() const
			{
				if (!is_valid()) return std::filesystem::file_time_type::min();
				return (*providers.begin())->get_time_last_mod();
			}

			bool add_provider_actual(std::unique_ptr<provider_actual_t> provider)
			{
				if(!provider) return false;
				
				if (providers.empty())
					virtual_path = provider->get_virtual_path();

				if (virtual_path == provider->get_virtual_path())
				{
					providers.insert(std::move(provider));
					return true;
				}
				return false;
			}
			
		private:

			static std::unique_ptr<output_t> platform_create_new(virtual_path_t path);

			virtual_path_t virtual_path;

			struct providers_comparator
			{
				bool operator() (const std::unique_ptr<provider_actual_t> & left, const std::unique_ptr<provider_actual_t> &right)
				{
					return left->get_time_last_mod() > right->get_time_last_mod();
				}
			};

			std::set<std::unique_ptr<provider_actual_t>, providers_comparator> providers;

		};
	}

}

#endif