#ifndef GAME_MODULE_PROBLEM_STD
#define GAME_MODULE_PROBLEM_STD(...)
#endif

#ifndef GAME_ASSET_PROBLEM_STD
#define GAME_ASSET_PROBLEM_STD(...)
#endif


GAME_MODULE_PROBLEM_STD(target_manifest_corrupted)
GAME_MODULE_PROBLEM_STD(target_does_not_exists)
GAME_MODULE_PROBLEM_STD(target_required_here)
GAME_MODULE_PROBLEM_STD(target_forbidden_here)

GAME_ASSET_PROBLEM_STD(double_definitions)
GAME_ASSET_PROBLEM_STD(defined_and_deleted)
GAME_ASSET_PROBLEM_STD(no_base_item)

#undef GAME_ASSET_PROBLEM_STD
#undef GAME_MODULE_PROBLEM_STD