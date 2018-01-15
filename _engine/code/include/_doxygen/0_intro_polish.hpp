/**
* \~Polish @mainpage The Pixie Engine Introduction
*
* @verbatim
*  #############################################################\
*  #  _____ _      _        ______             _              # |
*  # |  __ (_)    (_)      |  ____|           (_)             # |
*  # | |__) |__  ___  ___  | |__   _ __   __ _ _ _ __   ___   # |
*  # |  ___/ \ \/ / |/ _ \ |  __| | '_ \ / _` | | '_ \ / _ \  # |
*  # | |   | |>  <| |  __/ | |____| | | | (_| | | | | |  __/  # |
*  # |_|   |_/_/\_\_|\___| |______|_| |_|\__, |_|_| |_|\___|  # |
*  #                                      __/ |               # |
*  #                                     |___/   Version 4.0  # |
*  ############################################################ |
*  \___________________________________________________________\|
*
*
*  "Do not go where the path may lead;
*  Go instead where there is no path
*  and leave a trail"
*
*                       Ralph Waldo Emerson
* @endverbatim
*
* @section pixie_intro Introduction
*     The fourth release of awesome 2D engine for video games.
*     Written in modern C++. Powerful, flexible and extendable. And completely stand-alone!
*
* @section pixie_features Features
* @subsection pixie_features_component Components
*     - @ref components "Build based on components". Each can be disabled, replacing core functionality with dummy class
*     - Major components
*         - @ref engine::config_t "Configuration component" with real-time config update notfiications
*         - @ref engine::data_source_t "Data provider" monitoring VFS for real-time data files changes
*         - @ref engine::logger_t "Logger" allowing multiple logging outputs, formatting and raporting to user
*         - @ref engine::terminal_writer_t "Terminal writer" to standard output (usually the system console)
*     - Minor components
*         - @ref engine::environment_info_t "Environmnet info" provider for logging purpouses
*         - @ref engine::renderer_status_t "Frame Notifier" allowing querying data for renderer status
*         - @ref engine::logger_file_writer_t "Log file writer" for quick and robust log output, available even when full @ref engine::data_provieder_t "data provider" is unavailable
* @subsection pixie_features_provider Information providers
*     - @ref engine::common_filenames_provider_t "Common filenames" on given platform (like logger output filename)
*     - @ref engine::save_location_provider_t "Save location" for given platform (full path)
*     - @ref engine::scanners_provider_t "Scanner provider" for @ref virtual_path VFS on given platform
* @subsection pixie_features_utilities Utilities
*     - Concurrention
*         - @ref engine::thread_pool_t "Thread pool"
*     - Containers
*         - @ref engine::compile_time_map_t "Compile-time key-value map"
*         - @ref engine::concurrent_queue_t "Concurrent queue"
*         - @ref engine::sync_queue_t "Sync queue"
*         - @ref engine::sync_vector_t "Sync vector"
*     - @ref data "Data input/output utilities"
*     - Debugging tools
*         - @ref engine::platform::dump_callstack "Callstack dumps"
*     - @ref messenger "Multithreaded messenger" for internal components communication
*     - Design patterns
*         - @ref settings "Compile-time engine-wide settings"
*         - @ref engine::factory_t "Abstract Factory"
*         - @ref engine::flags_t "Flags" (based on enum classes)
*         - @ref engine::id_t "FourCC IDs"
*         - @ref engine::instance_lifetime_guard "Lifetime guards" (for platforms that cannot handle stack unwinding during exceptions)
*         - @ref components_provider "Providers subsystem for components"
*     - Platform
*         - @ref engine::compilation_t "Compilation-time switches"
*     - Renderer helpers
*         - @ref engine::color_t "sRGB color class"
*     - Security helpers
*         - @ref engine::crc32_t "Fast CRC32 calculator"
*     - Text utilities
*         - @ref engine::ustring_t "UTF-8 strings"
*         - @ref ustring_format "Text formatters"
* @subsection pixie_features_other Other features
*     - @ref settings "Compile-time engine settings"
*     - @ref bootstrapper "Bootstrapper for initialization"
*     - ~10,000 LoC as of August, 1st, 2017
*
* @section pixie_platforms Supported Platforms
* @subsection pixie_platforms_win Windows XP/Vista/7/8/8.1/10
*    - Both x86 and x64
*    - OpenGL renderer
*    - DirectX 9.0c renderer
* @subsection pixie_platforms_lin Linux
*    (Expected, not yet working)
* @subsection pixie_platforms_osx OS X
*    (Expected, not yet working)
*
* @section pixe_depedencies Depedencies
*     - @ref dependency_angelcode "AngelCode"
*     - @ref dependency_anttweakbar "AntTweakBar"
*     - @ref dependency_apacheant "Apache Ant"
*     - @ref dependency_blowfish "Blowfish"
*     - @ref dependency_di "Boost Dependency Injection"
*     - @ref dependency_cereal "cereal"
*     - @ref dependency_cg "cg"
*     - @ref dependency_debugbreak "debugbreak"
*     - @ref dependency_directx "directx"
*     - @ref dependency_doxygen "doxygen"
*     - @ref dependency_git "git"
*     - @ref dependency_glew "glew"
*     - @ref dependency_googletest "Google Test"
*     - @ref dependency_html_help_workshop "HTML Help Workshop"
*     - @ref dependency_jdk "JDK 1.8"
*     - @ref dependency_librocket "libRocket"
*     - @ref dependency_vlc "libVLC"
*     - @ref dependency_nsis "NSIS"
*     - @ref dependency_openal_soft "OpenAL (Software)"
*     - @ref dependency_pugixml "pugixml"
*     - @ref dependency_sdl "SDL 2.0"
*     - @ref dependency_sdl_net "SDL NET 2.0"
*     - @ref dependency_stackwalker "Stackwalker"
*     - @ref dependency_stb "stb"
*     - @ref dependency_thttpd "thttpd"
*     - @ref dependency_upx "UPX"
*     - @ref dependency_wxwidgets "wxWidgets"
*
* @section pixe_copyrights Copyrights
*    This engine is written and maintained by Kosek.com
*/
