#ifndef ENGINE_COMMON_PLATFORM_HPP
#define ENGINE_COMMON_PLATFORM_HPP
#pragma once

#if defined WIN32 || defined _WINDOWS
#define PLATFORM_WINDOWS
#endif=

#include "common/ustring.hpp"
#include "common/injector.hpp"
#include <memory>

#include "common/filesystem.hpp"
#include "common/virtual_path.hpp"
#include "common/manifest_app.hpp"
#include <system_error>

#define XSTR(a) STR(a)
#define STR(a) #a
#define PIXIE_OUTPUT_TYPE_STR XSTR(PIXIE_OUTPUT_UNIX_NAME)
		
namespace filesystem = std::experimental::filesystem;

namespace engine
{
	class logger_t;

	class platform_t final
	{

	public:

		BOOST_DI_INJECT(platform_t, (named = arg0) std::string arg0, std::shared_ptr<manifest_app_t> manifest_app);
		~platform_t();

		platform_t(platform_t const&) = delete;
		platform_t(platform_t &&) = delete;
		platform_t& operator=(platform_t const&) = delete;
		platform_t& operator=(platform_t &&) = delete;
		
		ustring_t get_platform_version() const;
		void show_splashscreen(const virtual_path_t & virtual_path);
		void reset_working_directory();

		void copy_to_clipboard(const ustring_t & message);

		engine::logger_t * get_logger() const
		{
			return logger;
		}

		void set_logger(engine::logger_t * logger);

		class implementation_t;
		implementation_t * get_pimpl() const
		{
			return pimpl.get();
		}

		std::shared_ptr<manifest_app_t> get_manifest_app()
		{
			return manifest_app;
		}
		
	private:

		std::unique_ptr<implementation_t> pimpl;
		engine::logger_t * logger;
		std::shared_ptr<manifest_app_t> manifest_app;

	};
}

#endif