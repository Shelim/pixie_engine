#ifndef GAME_TASK_THREAD_CALLER_STD
#define GAME_TASK_THREAD_CALLER_STD(...)
#endif

#ifndef GAME_TASK_THREAD_CALLER_CUSTOM_STD
#define GAME_TASK_THREAD_CALLER_CUSTOM_STD(...)
#endif

GAME_TASK_THREAD_CALLER_STD(main)
GAME_TASK_THREAD_CALLER_STD(renderer)
GAME_TASK_THREAD_CALLER_CUSTOM_STD(custom)


#undef GAME_TASK_THREAD_CALLER_CUSTOM_STD
#undef GAME_TASK_THREAD_CALLER_STD