#ifndef ENGINE_COMMON_MANIFEST_HPP
#define ENGINE_COMMON_MANIFEST_HPP
#pragma once

#include <cstdint>

namespace engine
{
	/**
	* @brief Class for exposing application manifest into code
	*
	* The actual data for this will be fetched from @c manifest directory inside project structure
	*/
	class manifest_app_t final
	{

	public:

		/**
		* @brief Returns major part of version
		*
		* This should represent major changes in application
		*
		* @return Major version
		* @see version_minor, version_revision, version_build
		*/
		int32_t version_major();

		/**
		* @brief Returns minor part of version
		*
		* This should represent minor changes in application
		*
		* @return Minor version
		* @see version_major, version_revision, version_build
		*/
		int32_t version_minor();

		/**
		* @brief Returns revision part of version
		*
		* This should represent very small changes in application
		*
		* @return Version revision
		* @see version_major, version_minor, version_build
		*/
		int32_t version_revision();

		/**
		* @brief Returns build number part of version
		*
		* This function's result get incremented by 1 on every build
		*
		* @return Build number
		* @see version_major, version_minor, version_revision
		*/
		int32_t version_build();

		/**
		* @brief Returns name of the game
		*
		* Returns string as set inside @c manifest directory inside given project
		*
		* @note This value is baked into binary during compilation! You do not need to include @c manifest directory in your releases!
		*
		* @return Name of the game
		* @see unix_name
		*/
		const char * game_name();

		/**
		* @brief Returns short (UNIX) name of the game
		*
		* Returns string as directory name of the project
		*
		* @note This value is baked into binary during compilation! You do not need to include @c manifest directory in your releases!
		*
		* @return Short (UNIX) name of the game
		* @see game_name
		*/
		const char * unix_name();

		/**
		* @brief Returns url of website
		*
		* Returns url of game website as set inside @c manifest directory inside given project
		*
		* @note This value is baked into binary during compilation! You do not need to include @c manifest directory in your releases!
		*
		* @return Url of website
		* @see support_page_url, updater_manifest_url
		*/
		const char * website_url();

		/**
		* @brief Returns url of support page
		*
		* Returns url of support website as set inside @c manifest directory inside given project
		*
		* @note This value is baked into binary during compilation! You do not need to include @c manifest directory in your releases!
		*
		* @return Url of support website
		* @see website_url, updater_manifest_url
		*/
		const char * support_page_url();

		/**
		* @brief Returns url of manifest update file (XML)
		*
		* Returns url of update manifest file (XML)
		*
		* @note This value is baked into binary during compilation! You do not need to include @c manifest directory in your releases!
		*
		* @note @ref manifest_update_file "Schema of manifest update file is presented here"
		*
		* @return Url of update manifest file (XML)
		* @see website_url, support_page_url
		*/
		const char * updater_manifest_url();

		/**
		* @brief Returns platform-specific command to run game module
		*
		* This would be most useful inside launcher.
		* The value is queried from @c manifest directory inside project structure during compilation
		*
		* @note This value is baked into binary during compilation! You do not need to include @c manifest directory in your releases!
		*
		* @return platform-specific command to run the game
		* @see game_name, editor_command
		*/
		const char * game_command();

		/**
		* @brief Returns platform-specific command to run editor module
		*
		* This would be most useful inside launcher.
		* The value is queried from @c manifest directory inside project structure during compilation
		*
		* @note This value is baked into binary during compilation! You do not need to include @c manifest directory in your releases!
		*
		* @return platform-specific command to run the editor
		* @see game_name, game_command
		*/
		const char * editor_command();

		/**
		* @brief Returns windows-only app data subdirectory name
		*
		* @warning This is Windows-only! Do not use it on other platforms, as result is undefined.
		*
		* The value is queried from @c manifest directory inside project structure during compilation
		*
		* @note This value is baked into binary during compilation! You do not need to include @c manifest directory in your releases!
		*
		* @return windows-only name of app data subdirectory
		* @see game_command, editor_command
		*/
		const char * windows_game_app_dir();


	private:

	};
}

#endif