#ifndef ENGINE_RICHTEXT_TAG_STD
#define ENGINE_RICHTEXT_TAG_STD(...)
#endif

#ifndef ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD
#define ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(...)
#endif

ENGINE_RICHTEXT_TAG_STD(section_header)
ENGINE_RICHTEXT_TAG_STD(section_header_text)

ENGINE_RICHTEXT_TAG_STD(info)
ENGINE_RICHTEXT_TAG_STD(note)
ENGINE_RICHTEXT_TAG_STD(success)
ENGINE_RICHTEXT_TAG_STD(pending)
ENGINE_RICHTEXT_TAG_STD(warning)
ENGINE_RICHTEXT_TAG_STD(error)

ENGINE_RICHTEXT_TAG_STD(info_meta)
ENGINE_RICHTEXT_TAG_STD(note_meta)
ENGINE_RICHTEXT_TAG_STD(success_meta)
ENGINE_RICHTEXT_TAG_STD(pending_meta)
ENGINE_RICHTEXT_TAG_STD(warning_meta)
ENGINE_RICHTEXT_TAG_STD(error_meta)

/*

red_dark,
red_light,

green_dark,
green_light,

blue_dark,
blue_light,

gray_dark,
gray_light,

white,
black,

yellow_dark,
yellow_light,

pink_dark,
pink_light,

teal_dark,
teal_light

ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(section_header, pink_dark, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(section_header_text, pink_light, black)

ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(info, teal_light, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(note, gray_light, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(success, green_light, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(pending, teal_light, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(warning, yellow_light, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(error, red_light, black)

ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(info_meta, teal_dark, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(note_meta, gray_dark, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(success_meta, green_dark, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(pending_meta, teal_dark, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(warning_meta, yellow_dark, black)
ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(error_meta, red_dark, black)

*/

#include "std/app_richtext_std.hpp"

#undef ENGINE_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD
#undef ENGINE_RICHTEXT_TAG_STD