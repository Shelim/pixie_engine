#ifndef ENGINE_COMPONENT_DATA_STATE_REAL_HPP
#define ENGINE_COMPONENT_DATA_STATE_REAL_HPP
#pragma once

#include "global/component/data_state.hpp"
#include "global/core/data/provider.hpp"
#include "global/core/data/provider_actual.hpp"

namespace engine
{
	class data_state_real_t : public data_state_t
	{

	public:

		data_state_real_t()
		{

		}

		std::unique_ptr<data::input_t> get_input(const virtual_path_t & filename) const final
		{
			auto provider = get_provider(filename);
			if(!provider) return std::make_unique<data::input_void_t>(filename);
			return std::move(provider->construct_input());
		}
		std::unique_ptr<data::output_t> get_output(const virtual_path_t & filename) const final
		{
			auto provider = get_provider(filename);
			if(!provider) return std::make_unique<data::output_void_t>(filename);
			return std::move(provider->construct_output());
		}
		bool is_read_only(const virtual_path_t & filename) const final
		{
			auto provider = get_provider(filename);
			if(!provider) return false;
			return provider->is_read_only();
		}
		std::filesystem::file_time_type get_time_last_mod(const virtual_path_t & filename) const final
		{
			auto provider = get_provider(filename);
			if(!provider) return std::filesystem::file_time_type::min();
			return provider->get_time_last_mod();
		}

		const items_t & get_files(const virtual_path_t & path) const final
		{
			auto iter = files.find(path);
			if (iter == files.end())
				return empty;
			return iter->second;
		}
		const items_t & get_directories(const virtual_path_t & path) const final
		{
			auto iter = directories.find(path);
			if (iter == directories.end())
				return empty;
			return iter->second;
		}

		void update_provider(std::unique_ptr<data::provider_actual_t> provider) final
		{
			virtual_path_t path = provider->get_virtual_path();
			auto iter = providers.find(path);
			if (iter == providers.end())
			{
				providers[path] = std::make_unique<data::provider_t>();
			}

			providers[path]->add_provider_actual(std::move(provider));
		}

	private:

		data::provider_t * get_provider(const virtual_path_t & filename) const
		{
			auto iter = providers.find(filename);
			if (iter == providers.end())
				return nullptr;
			return iter->second.get();
		}

		typedef std::map<virtual_path_t, std::unique_ptr<data::provider_t> > providers_t;
		typedef std::map<virtual_path_t, items_t > directories_t;
		typedef std::map<virtual_path_t, items_t > files_t;

		providers_t providers;
		directories_t directories;
		files_t files;

		items_t empty;

	};
}


#endif