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

ENGINE_LOGGER_LEVEL_STD(task_started,	"$pending_meta$#[id] ... $pending$[message]$>125$   $pending_meta$frame: $pending$[frame]$pending_meta$, time: $pending$[time]$pending_meta$, thread: $pending$[thread]$pending_meta$, func: $pending$[function]()$>253$   $pending_meta$at: $pending$'[file]'$pending_meta$, on line: $pending$[line]\n"_u,		"$pending_meta$...$pending$[message]$>29$   $pending_meta$func: $pending$[function](), $pending_meta$line: $pending$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(task_failed,	"$error_meta$#[id] !!! $error$[message]$>125$   $error_meta$frame: $error$[frame]$error_meta$, time: $error$[time]$error_meta$, thread: $error$[thread]$error_meta$, func: $error$[function]()$>253$   $error_meta$at: $error$'[file]'$error_meta$, on line: $error$[line]\n"_u,									"$error_meta$!!!$error$[message]$>29$   $error_meta$func: $error$[function](), $error_meta$line: $error$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(task_done,		"$success_meta$#[id] ..> $success$[message]$>125$   $success_meta$frame: $success$[frame]$success_meta$, time: $success$[time]$success_meta$, thread: $success$[thread]$success_meta$, func: $success$[function]()$>253$   $success_meta$at: $success$'[file]'$success_meta$, on line: $success$[line]\n"_u,		"$success_meta$..>$success$[message]$>29$   $success_meta$func: $success$[function](), $success_meta$line: $success$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(message,		"$note_meta$#[id] --> $note$[message]$>125$   $note_meta$frame: $note$[frame]$note_meta$, time: $note$[time]$note_meta$, thread: $note$[thread]$note_meta$, func: $note$[function]()$>253$   $note_meta$at: $note$'[file]'$note_meta$, on line: $note$[line]\n"_u,													"$note_meta$-->$note$[message]$>29$   $note_meta$func: $note$[function](), $note_meta$line: $note$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(warning,		"$warning_meta$#[id] -!- $warning$[message]$>125$   $warning_meta$frame: $warning$[frame]$warning_meta$, time: $warning$[time]$warning_meta$, thread: $warning$[thread]$warning_meta$, func: $warning$[function]()$>253$   $warning_meta$at: $warning$'[file]'$warning_meta$, on line: $warning$[line]\n"_u,		"$warning_meta$-!-$warning$[message]$>29$   $warning_meta$func: $warning$[function](), $warning_meta$line: $warning$[line]"_u)
ENGINE_LOGGER_LEVEL_STD(error,			"$error_meta$#[id] !!! $error$[message]$>125$   $error_meta$frame: $error$[frame]$error_meta$, time: $error$[time]$error_meta$, thread: $error$[thread]error_meta$, func: $error$[function]()$>253$   $error_meta$at: $error$'[file]'$error_meta$, on line: $error$[line]\n"_u,										"$error_meta$!!!$error$[message]$>29$   $error_meta$func: $error$[function](), $error_meta$line: $error$[line]"_u)

ENGINE_LOGGER_ITEM_PART_STD("[id]"_u, to_string(item.get_id(), 4))
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