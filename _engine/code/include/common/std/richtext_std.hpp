#ifndef GAME_RICHTEXT_TAG_STD
#define GAME_RICHTEXT_TAG_STD(...)
#endif

#ifndef GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD
#define GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(...)
#endif

GAME_RICHTEXT_TAG_STD(section_header)
GAME_RICHTEXT_TAG_STD(section_header_text)

GAME_RICHTEXT_TAG_STD(info)
GAME_RICHTEXT_TAG_STD(note)
GAME_RICHTEXT_TAG_STD(success)
GAME_RICHTEXT_TAG_STD(pending)
GAME_RICHTEXT_TAG_STD(warning)
GAME_RICHTEXT_TAG_STD(error)

GAME_RICHTEXT_TAG_STD(info_meta)
GAME_RICHTEXT_TAG_STD(note_meta)
GAME_RICHTEXT_TAG_STD(success_meta)
GAME_RICHTEXT_TAG_STD(pending_meta)
GAME_RICHTEXT_TAG_STD(warning_meta)
GAME_RICHTEXT_TAG_STD(error_meta)

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

*/

GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(section_header, pink_dark, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(section_header_text, pink_light, black)

GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(info, teal_light, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(note, gray_light, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(success, green_light, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(pending, teal_light, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(warning, yellow_light, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(error, red_light, black)

GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(info_meta, teal_dark, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(note_meta, gray_dark, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(success_meta, green_dark, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(pending_meta, teal_dark, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(warning_meta, yellow_dark, black)
GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD(error_meta, red_dark, black)

#include "std/richtext_game_std.hpp"

#undef GAME_RICHTEXT_COLOR_WINDOWS_CONSOLE_STD
#undef GAME_RICHTEXT_TAG_STD