#ifndef GAME_LOGGER_OUTPUT_STD
#define GAME_LOGGER_OUTPUT_STD(...)
#endif

#ifndef EDITOR_LOGGER_OUTPUT_STD
#define EDITOR_LOGGER_OUTPUT_STD(...)
#endif

#ifndef LAUNCHER_LOGGER_OUTPUT_STD
#define LAUNCHER_LOGGER_OUTPUT_STD(...)
#endif

GAME_LOGGER_OUTPUT_STD(console, true, true)
GAME_LOGGER_OUTPUT_STD(data, true, true)
GAME_LOGGER_OUTPUT_STD(raw, true, true)

EDITOR_LOGGER_OUTPUT_STD(console, false, true)
EDITOR_LOGGER_OUTPUT_STD(data, true, true)
EDITOR_LOGGER_OUTPUT_STD(raw, true, true)

LAUNCHER_LOGGER_OUTPUT_STD(console, false, true)
LAUNCHER_LOGGER_OUTPUT_STD(data, true, true)
LAUNCHER_LOGGER_OUTPUT_STD(raw, true, true)


#undef LAUNCHER_LOGGER_OUTPUT_STD
#undef EDITOR_LOGGER_OUTPUT_STD
#undef GAME_LOGGER_OUTPUT_STD