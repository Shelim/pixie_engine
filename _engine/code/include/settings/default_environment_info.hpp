#ifndef ENGINE_COMMON_DEFAULT_ENVIRONMENT_INFO_HPP
#define ENGINE_COMMON_DEFAULT_ENVIRONMENT_INFO_HPP
#pragma once

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"
#include "global/component/environment_info.hpp"
#include "global/core/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "global/core/vfs/virtual_path.hpp"
#include "global/component/terminal_writer.hpp"

namespace engine
{

	SETTINGS_START(environment_info_output_t, normal)

		SETTINGS_SET(ustring_t, file_start, "########################\n### ENVIRONMENT INFO ###\n########################\n"_u)
		SETTINGS_SET(ustring_t, file_end, "\n############################\n### END ENVIRONMENT INFO ###\n############################\n"_u)
		SETTINGS_SET(ustring_t, terminal_start, "$section_header$########################\n### $section_header_text$ENVIRONMENT INFO$section_header$ ###\n########################\n"_u)
		SETTINGS_SET(ustring_t, terminal_end, "\n$section_header$############################\n### $section_header_text$END ENVIRONMENT INFO$section_header$ ###\n############################\n"_u)

		SETTINGS_SET(ustring_t, format_file_normal, "> #1#:#>28:-#--> #2#"_u)
		SETTINGS_SET(ustring_t, format_terminal_normal, " $info_meta$#1#:#>28:-#--> $info$#2#"_u)

		SETTINGS_SET(ustring_t, format_file_good, "> #1#:#>28:-#--> #2#"_u)
		SETTINGS_SET(ustring_t, format_terminal_good, " $info_meta$#1#:#>28:-#--> $info$#2#"_u)

		SETTINGS_SET(ustring_t, format_file_bad, "! #1#:#>28:-#--> #2#"_u)
		SETTINGS_SET(ustring_t, format_terminal_bad, "!$info_meta$#1#:#>28:-#--> $info$#2#$info_meta$ (Problematic)"_u)

		SETTINGS_SET(ustring_t, format_file_not_yet_populated, "! #1#:#>28:-#--> ???"_u)
		SETTINGS_SET(ustring_t, format_terminal_not_yet_populated, "!$info_meta$#1#:#>28:-#--> $info$???$info_meta$ (Unknown)"_u)

	SETTINGS_END()

}

#endif