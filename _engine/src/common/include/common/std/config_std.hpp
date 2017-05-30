#ifndef GAME_CONFIG_STD
#define GAME_CONFIG_STD(...)
#endif

#ifndef GAME_CONFIG_STD_BEGIN_CATEGORY
#define GAME_CONFIG_STD_BEGIN_CATEGORY(...)
#endif

#ifndef GAME_CONFIG_STD_BEGIN_GROUP
#define GAME_CONFIG_STD_BEGIN_GROUP(...)
#endif

#ifndef GAME_CONFIG_STD_CONFIGURABLE
#define GAME_CONFIG_STD_CONFIGURABLE(...)
#endif

#ifndef GAME_CONFIG_STD_CONFIG_INFO
#define GAME_CONFIG_STD_CONFIG_INFO(...)
#endif

#ifndef GAME_CONFIG_STD_END_GROUP
#define GAME_CONFIG_STD_END_GROUP(...)
#endif

#ifndef GAME_CONFIG_STD_END_CATEGORY
#define GAME_CONFIG_STD_END_CATEGORY(...)
#endif

#ifndef GAME_CONFIG_PROVIDER_STD
#define GAME_CONFIG_PROVIDER_STD provider_data_t
#endif
// GAME_CONFIG_STD(Name, ReleaseDefValue, DebugDefValue)

#define GAME_COMPILER_OUTPUT_TYPE_STD(output) GAME_CONFIG_STD(output##_is_first_run, true, true)
#include "common/std/compiler_output_types_std.hpp"

#define GAME_LANGUAGE_TYPE_STD(LANG) GAME_CONFIG_STD(common_language_ ## LANG, _U("_default"), _U("_default"))
#include "common/std/language_type_std.hpp"

GAME_CONFIG_STD(game_window_w, 800, 800)
GAME_CONFIG_STD(game_window_h, 600, 600)
GAME_CONFIG_STD(game_window_resizable, false, true)

GAME_CONFIG_STD(game_is_fullscreen, false, true)
GAME_CONFIG_STD(game_is_fullscreen_native, true, true)
GAME_CONFIG_STD(game_is_fullscreen_advanced, true, true)
GAME_CONFIG_STD(game_force_fullscreen_w, 800, 800)
GAME_CONFIG_STD(game_force_fullscreen_h, 600, 600)
GAME_CONFIG_STD(game_vsync, true, true)

GAME_CONFIG_STD(game_movie_w, 512, 512)
GAME_CONFIG_STD(game_movie_h, 512, 512)

GAME_CONFIG_STD(game_renderer, _U("sdl2_opengl"), _U("sdl2_opengl"))

GAME_CONFIG_STD(game_module, _U("base"), _U("base"))
GAME_CONFIG_STD(game_submodules, engine::ustring_collection_t{}, engine::ustring_collection_t{})

GAME_CONFIG_STD(game_audio, _U(""), _U(""))
GAME_CONFIG_STD(game_volume_master, 1.0f, 1.0f)
#define GAME_AUDIO_TYPE_STD(audio) GAME_CONFIG_STD(game_volume_ ## audio, 1.0f, 1.0f)
#include "common/std/audio_type_std.hpp"

GAME_CONFIG_STD(game_has_console, false, true)
GAME_CONFIG_STD(game_has_anttweakbar, false, true)
GAME_CONFIG_STD(game_has_interface_debugger, false, true)

GAME_CONFIG_STD(game_skip_splashscreen, false, true)
GAME_CONFIG_STD(editor_skip_splashscreen, false, true)
GAME_CONFIG_STD(game_skip_fadein_on_startup, false, true)
GAME_CONFIG_STD(game_skip_fades_between_states, false, false)

#include "common/std/config_game_std.hpp"

/*
GAME_CONFIG_STD_BEGIN_CATEGORY(system)
	GAME_CONFIG_STD_BEGIN_GROUP(system_language)
#define GAME_LANGUAGE_TYPE_STD(LANG) GAME_CONFIG_STD_CONFIGURABLE(language, system_language_language_ ## LANG, engine_language_ ## LANG, _U("lang_available_" #LANG))
#include "common/std/language_type_std.hpp"
	GAME_CONFIG_STD_END_GROUP()
GAME_CONFIG_STD_END_CATEGORY()

GAME_CONFIG_STD_BEGIN_CATEGORY(game)
// ToDo
GAME_CONFIG_STD_END_CATEGORY()

GAME_CONFIG_STD_BEGIN_CATEGORY(mods)
	GAME_CONFIG_STD_BEGIN_GROUP(mods_main)
		GAME_CONFIG_STD_CONFIGURABLE(module, mods_main_module, engine_module)
		GAME_CONFIG_STD_CONFIGURABLE(submodules, mods_main_submodules, engine_submodules)
		GAME_CONFIG_STD_CONFIG_INFO(asset_problems, mods_main_problems)
	GAME_CONFIG_STD_END_GROUP()
GAME_CONFIG_STD_END_CATEGORY()

GAME_CONFIG_STD_BEGIN_CATEGORY(graphics)
	GAME_CONFIG_STD_BEGIN_GROUP(graphics_renderer)
		GAME_CONFIG_STD_CONFIGURABLE(renderer, graphics_renderer_renderer, engine_renderer)
		GAME_CONFIG_STD_CONFIGURABLE(resolution_pot, graphics_renderer_movie_size, engine_movie_w, engine_movie_h)
	GAME_CONFIG_STD_END_GROUP()
	GAME_CONFIG_STD_BEGIN_GROUP(graphics_fullscreen)
		GAME_CONFIG_STD_CONFIGURABLE(boolean, graphics_fullscreen_is_fullscreen, engine_is_fullscreen)
		GAME_CONFIG_STD_CONFIGURABLE(boolean, graphics_fullscreen_is_fullscreen_native, engine_is_fullscreen_native)
		GAME_CONFIG_STD_CONFIGURABLE(resolution, graphics_fullscreen_force_fullscreen_size, engine_force_fullscreen_w, engine_force_fullscreen_h)
		GAME_CONFIG_STD_CONFIGURABLE(boolean, graphics_fullscreen_is_fullscreen_advanced, engine_is_fullscreen_advanced)
		GAME_CONFIG_STD_CONFIGURABLE(boolean, graphics_fullscreen_vsync, engine_vsync)
	GAME_CONFIG_STD_END_GROUP()
		GAME_CONFIG_STD_BEGIN_GROUP(graphics_window)
		GAME_CONFIG_STD_CONFIGURABLE(resolution, graphics_window_window_size, engine_window_w, engine_window_h)
		GAME_CONFIG_STD_CONFIGURABLE(boolean, graphics_window_window_resizable, engine_window_resizable)
		GAME_CONFIG_STD_END_GROUP()
GAME_CONFIG_STD_END_CATEGORY()

GAME_CONFIG_STD_BEGIN_CATEGORY(input)
	// ToDo
GAME_CONFIG_STD_END_CATEGORY()

GAME_CONFIG_STD_BEGIN_CATEGORY(sounds)
	GAME_CONFIG_STD_BEGIN_GROUP(sounds_audio)
		GAME_CONFIG_STD_CONFIGURABLE(audio_device, sounds_audio_audio, engine_audio)
	GAME_CONFIG_STD_END_GROUP()
	GAME_CONFIG_STD_BEGIN_GROUP(sounds_volume)
		GAME_CONFIG_STD_CONFIGURABLE(volume, sounds_volume_volume_master, engine_volume_master)
#define GAME_AUDIO_TYPE_STD(AUDIO) GAME_CONFIG_STD_CONFIGURABLE(volume, sounds_volume_volume_ ## AUDIO, engine_volume_ ## AUDIO)
#include "common/std/audio_type_std.hpp"
	GAME_CONFIG_STD_END_GROUP()
GAME_CONFIG_STD_END_CATEGORY()

GAME_CONFIG_STD_BEGIN_CATEGORY(network)
// ToDo
GAME_CONFIG_STD_END_CATEGORY()

GAME_CONFIG_STD_BEGIN_CATEGORY(debug)
	GAME_CONFIG_STD_BEGIN_GROUP(debug_consoles)
		GAME_CONFIG_STD_CONFIGURABLE(boolean, debug_consoles_has_anttweakbar, engine_has_anttweakbar)
		GAME_CONFIG_STD_CONFIGURABLE(boolean, debug_consoles_has_interface_debugger, engine_has_interface_debugger)
	GAME_CONFIG_STD_END_GROUP()
	GAME_CONFIG_STD_BEGIN_GROUP(debug_logger_output)
#define GAME_LOGGER_OUTPUT_STD(LOGGER_OUTPUT, ReleaseDefValue, DebugDefValue) GAME_CONFIG_STD_CONFIGURABLE(boolean, debug_logger_output_ ## LOGGER_OUTPUT, engine_logger_output_ ## LOGGER_OUTPUT)
#include "common/std/logger_output_std.hpp"
	GAME_CONFIG_STD_END_GROUP()
GAME_CONFIG_STD_END_CATEGORY()

*/

#undef GAME_CONFIG_STD
#undef GAME_CONFIG_STD_BEGIN_CATEGORY
#undef GAME_CONFIG_STD_BEGIN_GROUP
#undef GAME_CONFIG_STD_CONFIGURABLE
#undef GAME_CONFIG_STD_CONFIG_INFO
#undef GAME_CONFIG_STD_END_GROUP
#undef GAME_CONFIG_STD_END_CATEGORY