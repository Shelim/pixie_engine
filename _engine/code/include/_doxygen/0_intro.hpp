/**
* \~English @mainpage The Pixie Engine Introduction
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
* @subsection pixie_features_architecture Architecture
*     - @ref components "Architecture based on components". Each can be disabled, replacing core functionality with dummy class (for example for testing purposes)
*     - @ref providers "Most of the components supports different functionality providers", which are tweakable
*     - Major group of components
*         - @ref component_config "Configuration component" with real-time config update notifications
*         - @ref component_data "Data provider component" providing VFS over files across different systems
*         - @ref component_environment "Environment component" providing insight on executing environment
*         - @ref component_filesystem "Filesystem component" providing direct access to files (outside Data provider)
*         - @ref component_logger "Logger" supporting variety of output formats (even scriptable by user!)
*         - @ref component_terminal "Terminal component" allowing multiple terminal window with text coloring on each supported platform
* @subsection pixie_features_other Other features
*     - ~18,000 LoC across 225 files as of January, 1st, 2018
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
* @section pixie_depedencies Depedencies
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
* @section pixie_copyrights Copyrights
*    This engine is written and maintained by Kosek.com
*/
