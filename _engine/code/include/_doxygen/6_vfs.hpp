
/**
* \~English @page vfs Virtual File System (VFS)
* @tableofcontents
* @section vfs_intro Introduction
*		Pixie (as many other game engines) uses VFS (Virtual File System).
*		VFS allows layer of abstraction of asset path over physical file system.
*		This feature is composed out of a few features described below.
*
*		The Pixie's VFS serve not only input, but allows modyfing of any asset
*		effectivly serving as game-consumer and editor-producer at the same time.
*		The assets are reloaded dynamically during execution and can be updated
*		by any of Pixie application as well as physically by user.
*
* @section vfs_vpath Virtual path
*		Virtual path (vpath) represents asset inside VFS. Vpath is composed
*		out of two elements: @ref engine::virtual_path_t::type_t "type" and address.
*		Type represent search target for physical file, while address represents
*		simplified name of the file.
*
*		In general Vpath may represent more than one physical file, or even
*		no file at all (when it comes outside of @ref engine::data::state_t "state database").
*
*		Vpath will always resolve to the newest file satisfying requirenments. For input
*		the requirenment is to be readable. For output the requirenment is to be both readable and writable.
*
*		Because of that, input and output may point to different files on target machine.
*		After saving, the newly saved file will usually have the highest modification date,
*		and will be noted to reload for input operation.
*
*		@see engine::virtual_path_t
*
* @section vfs_input_output Input/Output
*       Input and Output classes provides generic way to read or write `data`. From the user perspective
*       it does not matter which provider is present underneath. It can be file, network stream, ftp, etc.
*       All reading should be done via @ref engine::data::input_t input_t and @ref engine::data::output_t output_t
*       classes. The only exception is logger file writer, because it has to be able to notify about errors on `data`
*       subsystem initialization
*
*       Engine provides standard wrappers for streams - @ref engine::data::input_streambuf_t input_streambuf_t
*       for input and @ref engine::data::output_streambuf_t output_streambuf_t for output. Both are based
*       on std::streambuf and allows to create custom C++ streams
*
*       It is also possible to spawn 'partial' input. If you know that single `data` contains more than one
*       asset (for example different LODs on texture), you can ask input_t to read data of given length to buffer
*       then use it to construct new input_t. Original stream position *will not be affected*.
*       Spawned input_t operates on copy, so it can outlive orignal input_t; Spawned input_t has access only
*       to this portion of data requested originally from @ref engine::data::input_t::spawn_partial spawn_partial method.
*
*		@see engine::data::input_t, engine::data::output_t
*
* @section vfs_providers Providers
*       Provider is a way to construct Input/Output classes for given `data` item. It is usually accessed from `data`
*       engine and does not require manual tampering.
*       @warning Input and Output may, in some cases point to different locations, even if virtual path for them is the same!
*       For example you could be reading input from CD then writing output to Local App Data.
*       Actual providers note last modification time. Final povider will always choose the newest provider available
*       (for output the provider must be able to write data).
*       If no provider for output is available, the new one will be constructed by @ref engine::platform::create_new create_new
*       platform function. If it is impossible to write any `data` at all, @ref engine::data::output_void_t output_void_t
*       object will be constructed and returned.
*
*		@see engine::data::provider_t, engine::data::provider_actual_t
*
* @section vfs_scanners Scanners
*       Scanner is a small class that constantly search for modification date of content. The process is completely
*       automatic and executed at small time interval.
*       Changes found from previous update called are available in @ref engine::data_manipulator_t::get_changes data_manipulator_t::get_changes
*       method. `Data` subsystem will reload them on the fly without any action required, using new Input created from new provider.
*       List of scanners is provided via @ref settings settings to bootstrapper. Usually includes @ref engine::data::scanner_directory_t scanner_directory_t
*       that recursivly checks physical filesystem.
*       @warning Only files that have changed their modification time since last scan will be reloaded.
*
*		@see engine::data::scanner_t, engine::data::scanners_t
*
*/