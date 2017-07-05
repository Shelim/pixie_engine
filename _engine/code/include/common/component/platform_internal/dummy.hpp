#ifndef ENGINE_COMPONENT_PLATFORM_INTERNAL_DUMMY_HPP
#define ENGINE_COMPONENT_PLATFORM_INTERNAL_DUMMY_HPP
#pragma once

#include "model/manifest_app.hpp"
#include "component/platform_internal.hpp"

namespace engine
{

	class platform_internal_dummy_t : public platform_internal_t
	{

	public:

		platform_internal_dummy_t(std::shared_ptr<manifest_app_t> manifest_app) : manifest_app(manifest_app)
		{

		}
		
		void console_write(const richtext_t & richtext) final
		{
			// Do nothing, it is dummy.
		}

		std::filesystem::path get_self_path() final
		{
			return std::filesystem::current_path(); // For dummy purposes that is enought
		}

		std::filesystem::path get_save_path(const virtual_path_t & path) final
		{
			ustring_t test = manifest_app->get_local_app_name();
			return get_self_path() / "dummy_save_loc" / test.get_cstring(); // Save in self directory
		}

	private:

		std::shared_ptr<manifest_app_t> manifest_app;

		void open_console() final
		{
			// Do nothing, it is dummy.
		}

		void close_console() final
		{
			// Do nothing, it is dummy.
		}

	};
}

#endif