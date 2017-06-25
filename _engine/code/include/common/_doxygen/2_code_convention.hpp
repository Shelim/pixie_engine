 /**
 * @page code_convention Code Convention
 * @tableofcontents
 * @section code_convention_naming Naming conventions
 *
 *  1. All names are lowercase with underscores as word separator (ie. `hello_world`)
 *  2. Basic name pattern:
 *      - `object_property`` (ie. `game_name`)
 *      - `action_object_property` (ie. 'new_window_handle')
 *      -  Each section of the above may span more then one word (ie. `new_game_window_handle`)
 *  3. Naming list:
 *      - `asset` -> Any non-hardcoded piece of data that can be modded. See `content`
 *      - `content` -> All `assets`
 *      - `manifest` -> metadata for given item
 *      - `module` -> Large logical data-pack of assets. Each running game is composed of one or more modules. Example: large chapters/Total Conversion mods; See: `submodule`
 *      - `private` -> A thing to be used by developer himself and not included in final build
 *      - `submodule` -> Smaller logical data-pack of assets. Each running game can have zero or more active submodules. Example: DLC or mini-mods; See: `module`
 * @todo Complete @ref code_convention_naming "Naming conventions" section
 */