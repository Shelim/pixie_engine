/**
* @mainpage The Pixie Engine Introduction
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
*     Written in modern C++'14/C++'17. Powerful and extendable. And completely stand-alone!
*
* @section pixie_features Features
* @subsection pixie_features_data Data
*     - @ref engine::data::database_providers_t "Advanced data engine" for IO supporting range of different @ref engine::data::input_t "input sources"
*     - @ref engine::data::output_t "Output target" supported, allowing writing arbitrary data
*     - @ref Fully featured @ref virtual_path "Virtual File System (VFS)", completely abstract of physical implementation
*     - @ref Hot-swapping supported for almost all input sources. Data engine monitors status of data streams and updates already loaded @ref engine::data::item_base_t "data items"
*     - Ability to 'detach' object from monitoring (thus, creating hard copy in memory); Useful for assets that needs not to be tampered, for example during network play
*     - Almost all IO operations performed asynchronously
* @subsection pixie_features_assets Asset system
*     - Build atop @ref pixie_features_data "data subsystem"
*     - Allows hot-swapping of all assets (including network patches and modding tools)
*     - Support for `modules` and `submodules` with permission rules (allow/disallow certain mod/submod configurations to be used together)
*     - All assets are localizable (not only by string-tables!)
*     - All assets can have variants, for example HD textures or panoramic gui definitions
* @subsection pixie_features_lo_system System (low level)
*     - @ref engine::ustring_t "Portable UTF-8 strings"
*     - @ref engine::logger_t "Advanced logger" with @ref engine::execution_info_t "plenty of execution-time information"
*     - Crash dumper, if something goes wrong (allowing post-mortem debugging on Windows)
*     - @ref engine::crc32_t "CRC32 calculator"
*     - @ref engine::config_t "Configuration class"
*     - ~10,000 LoC as of May, 24th, 2017
* @todo Expand the @ref pixie_features "features" section
*
* @section pixie_platforms Supported Platforms
* @subsection pixie_platforms_win Windows XP/Vista/7/8/8.1/10
*    - Both x86 and x64
*    - OpenGL renderer
* @subsection pixie_platforms_lin Linux
*    (Expected, not yet working)
* @subsection pixie_platforms_osx OS X
*    (Expected, not yet working)
*
* @section pixe_libraries Libraries
*    (Click on any name to see full licence text)
*     - @ref license_angelcode "AngelCode"
*     - @ref license_anttweakbar "AntTweakBar"
*     - @ref license_apacheant "Apache Ant"
*     - @ref license_blowfish "Blowfish"
*     - @ref license_di "Boost Dependency Injection"
*     - @ref license_cereal "cereal"
*     - @ref license_enum_flags "enum_flags"
*     - @ref license_glew "glew"
*     - @ref license_gsl "gsl"
*     - @ref license_librocket "libRocket"
*     - @ref license_vlc "libVLC"
*     - @ref license_nsis "NSIS
*     - @ref license_pugixml "pugixml"
*
* @section pixe_copyrights Copyrights
*    This engine is written and maintained by Kosek.com
*/