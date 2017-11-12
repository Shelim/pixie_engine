#ifndef ENGINE_COMMON_DEFAULT_LOGGER_HPP
#define ENGINE_COMMON_DEFAULT_LOGGER_HPP
#pragma once

#include <SDL.h>
#include <cstdio>
#include <cstdlib>
#include "bootstrapper.hpp"
#include "component/logger.hpp"
#include "component/renderer_status.hpp"
#include "core/vfs/filesystem.hpp"
#include "utility/text/ustring.hpp"
#include "core/vfs/virtual_path.hpp"
#include "component/terminal_writer.hpp"

namespace engine
{

	SETTINGS_START(logger_output_t, normal)

		SETTINGS_SET(ustring_t, file_start, "#####################\n### LOGGER OUTPUT ###\n#####################\n"_u)
		SETTINGS_SET(ustring_t, file_end, "\n##################\n### CLEAN EXIT ###\n##################\n"_u)
		SETTINGS_SET(ustring_t, terminal_start, "$section_header$#####################\n### $section_header_text$LOGGER OUTPUT$section_header$ ###\n#####################\n"_u)
		SETTINGS_SET(ustring_t, terminal_end, "\n$section_header$##################\n### $section_header_text$CLEAN EXIT$section_header$ ###\n##################\n"_u)

		SETTINGS_SET(ustring_t, format_file_task_started, " msg ###1:id##>11# <#2:module#> #3:message#...#>125#   frame: #7:frame#, time: #8:time#, thread: #9:thread#, func: #4:function#()#>253#   at: #5:file#, on line: #6:line#"_u)
		SETTINGS_SET(ustring_t, format_terminal_task_started, "$pending_meta$->$pending$#3:message#...#>29#   $pending_meta$func: $pending$#4:function#(), $pending_meta$line: $pending$#6:line#"_u)

		SETTINGS_SET(ustring_t, format_file_task_failed, "ERR  ###1:id##>11# <#2:module#> FAILED ###10:link#: '#3:message#'#>125#   frame: #7:frame#, time: #8:time#, thread: #9:thread#, func: #4:function#()#>253#   at: #5:file#, on line: #6:line#"_u)
		SETTINGS_SET(ustring_t, format_terminal_task_failed, "$error_meta$!!>$error$#3:message#... [FAILED!]#>29#   $error_meta$func: $error$#4:function#(), $error_meta$line: $error$#6:line#"_u)

		SETTINGS_SET(ustring_t, format_file_task_done, " msg ###1:id##>11# <#2:module#> Completed ###10:link#: '#3:message#'#>125#   frame: #7:frame#, time: #8:time#, thread: #9:thread#, func: #4:function#()#>253#   at: #5:file#, on line: #6:line#"_u)
		SETTINGS_SET(ustring_t, format_terminal_task_done, "$success_meta$->$success$#3:message#... [Ok]#>29#   $success_meta$func: $success$#4:function#(), $success_meta$line: $success$#6:line#"_u)

		SETTINGS_SET(ustring_t, format_file_message, " msg ###1:id##>11# <#2:module#> #3:message##>125#   frame: #7:frame#, time: #8:time#, thread: #9:thread#, func: #4:function#()#>253#   at: #5:file#, on line: #6:line#"_u)
		SETTINGS_SET(ustring_t, format_terminal_message, "$note_meta$->$note$#3:message##>29#   $note_meta$func: $note$#4:function#(), $note_meta$line: $note$#6:line#"_u)

		SETTINGS_SET(ustring_t, format_file_warning, "WRN  ###1:id##>11# <#2:module#> #3:message##>125#   frame: #7:frame#, time: #8:time#, thread: #9:thread#, func: #4:function#()#>253#   at: #5:file#, on line: #6:line#"_u)
		SETTINGS_SET(ustring_t, format_terminal_warning, "$warning_meta$!>$warning$#3:message##>29#   $warning_meta$func: $warning$#4:function#(), $warning_meta$line: $warning$#6:line#"_u)

		SETTINGS_SET(ustring_t, format_file_error, "ERR  ###1:id##>11# <#2:module#> #3:message##>125#   frame: #7:frame#, time: #8:time#, thread: #9:thread#, func: #4:function#()#>253#   at: #5:file#, on line: #6:line#"_u)
		SETTINGS_SET(ustring_t, format_terminal_error, "$error_meta$!!>$error$#3:message##>29#   $error_meta$func: $error$#4:function#(), $error_meta$line: $error$#6:line#"_u)
	SETTINGS_END()

}

#endif