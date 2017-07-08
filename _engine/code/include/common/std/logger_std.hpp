#ifndef ENGINE_LOGGER_MODULE_STD
#define ENGINE_LOGGER_MODULE_STD(...)
#endif

#ifndef ENGINE_LOGGER_LEVEL_STD
#define ENGINE_LOGGER_LEVEL_STD(...)
#endif

#ifndef ENGINE_LOGGER_ITEM_PART_STD
#define ENGINE_LOGGER_ITEM_PART_STD(...)
#endif

ENGINE_LOGGER_MODULE_STD(core)

ENGINE_LOGGER_LEVEL_STD(task_started,	"#[id] ... <[module]> [message]...$>61$   frame: [frame], time: [time], thread: [thread], func: [function]()$>125$   at: '[file]', on line: [line]"_u,									"$pending_meta$...$pending$[message]...$>29$   $pending_meta$func: $pending$[function](), $pending_meta$line: $pending$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(task_failed,	"#[id] !!! <[module]> [message]... [FAILED!]$>61$   frame: [frame], time: [time], thread: [thread], func: [function]()$>125$   at: '[file]', on line: [line]"_u,						"$error_meta$!!!$error$[message]... [FAILED!]$>29$   $error_meta$func: $error$[function](), $error_meta$line: $error$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(task_done,		"#[id] ..> <[module]> [message]... [Ok]$>61$   frame: [frame], time: [time], thread: [thread], func: [function]()$>125$   at: '[file]', on line: [line]"_u,								"$success_meta$..>$success$[message]... [Ok]$>29$   $success_meta$func: $success$[function](), $success_meta$line: $success$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(message,		"#[id] --> <[module]> [message]$>61$   frame: [frame], time: [time], thread: [thread], func: [function]()$>125$   at: '[file]', on line: [line]"_u,										"$note_meta$-->$note$[message]$>29$   $note_meta$func: $note$[function](), $note_meta$line: $note$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(warning,		"#[id] -!- <[module]> [message]$>61$   frame: [frame], time: [time], thread: [thread], func: [function]()$>125$   at: '[file]', on line: [line]"_u,										"$warning_meta$-!-$warning$[message]$>29$   $warning_meta$func: $warning$[function](), $warning_meta$line: $warning$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(error,			"#[id] !!! <[module]> [message]$>61$   frame: [frame], time: [time], thread: [thread], func: [function]()$>125$   at: '[file]', on line: [line]"_u,										"$error_meta$!!!$error$[message]$>29$   $error_meta$func: $error$[function](), $error_meta$line: $error$[line]"_u)

ENGINE_LOGGER_ITEM_PART_STD("[id]"_u, to_string(item.get_id(), 4))
ENGINE_LOGGER_ITEM_PART_STD("[module]"_u, item.get_module_name())
ENGINE_LOGGER_ITEM_PART_STD("[message]"_u, item.get_message())
ENGINE_LOGGER_ITEM_PART_STD("[function]"_u, item.get_function())
ENGINE_LOGGER_ITEM_PART_STD("[file]"_u, item.get_file())
ENGINE_LOGGER_ITEM_PART_STD("[line]"_u, to_string(item.get_line()))
ENGINE_LOGGER_ITEM_PART_STD("[frame]"_u, to_string(item.get_frame()))
ENGINE_LOGGER_ITEM_PART_STD("[time]"_u, to_string(item.get_time()))
ENGINE_LOGGER_ITEM_PART_STD("[thread]"_u, to_string(item.get_thread()))

#undef ENGINE_LOGGER_ITEM_PART_STD
#undef ENGINE_LOGGER_LEVEL_STD
#undef ENGINE_LOGGER_MODULE_STD