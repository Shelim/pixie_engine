#include "common/platform.hpp"

#if PIXIE_WINDOWS

#include "common/logger.hpp"
#include "common/injector.hpp"
#include "common/config.hpp"
#include "common/data/database.hpp"
#include "common/data/item_content.hpp"
#include "common/logger_container.hpp"
#include "common/manifest_app.hpp"
#include "common/data/database_items.hpp"
//#include "common/module/database.hpp"
#include "common/config_container.hpp"
//#include "common/asset.hpp"
//#include "common/modules.hpp"

#include <clocale>
#include <string>
#include <stack>

#include <SDL.h>
#include <SDL_syswm.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <dbghelp.h>

#include <cstdio>
#include <cstdlib>
#include <io.h>
#include <fcntl.h>
#include <thread>
#include <chrono>

class item_content_text_t final : public engine::data::item_content_base_t
{

public:

	item_content_text_t(engine::data::item_generic_t * item) : item_content_base_t(item)
	{
		item->allow_changes_when_deleted();
		item->mark_as_both_auto_resave_and_auto_reload();
	}

	item_content_text_t(destroyed_t destroyed)
	{
		data = _U("");
	}

	engine::ustring_t get_data()
	{
		return data;
	}

	void mark_for_save()
	{
		data = _U("Sample");
		set_dirty();
	}

private:

	item_content_text_t * clone() const final
	{
		return new item_content_text_t(*this);
	}

	bool resave(engine::data::output_t * output) final
	{
		output->write_ustring(data);
		return true;
	}

	bool execute_input_operation(const engine::data::item_operation_t::step_t & step, engine::data::item_operation_t * operation) final
	{
		if (step.get_id() == 0)
		{
			operation->add_step('asyn', engine::data::item_operation_t::step_t::caller_t::async);
		}
		else if (step.get_id() == 'asyn')
		{
			data = operation->get_input()->read_ustring();
		}
		return true;
	}

	bool execute_output_operation(const engine::data::item_operation_t::step_t & step, engine::data::item_operation_t * operation) final
	{
		resave(operation->get_output());
		return true;
	}

	void execute_free_operation(const engine::data::item_operation_t::step_t & step, engine::data::item_operation_t * operation)
	{
		data = _U("");
	}

	engine::ustring_t data;

};

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	CHAR szFileName[MAX_PATH];
	GetModuleFileNameA(NULL, szFileName, MAX_PATH);

	HANDLE handle = CreateMutexA(NULL, TRUE, "Pixie" PIXIE_OUTPUT_TYPE_STR);
	if (handle == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND existingApp = FindWindowA("PixieGameWndClass", 0);
		if (existingApp) SetForegroundWindow(existingApp);
		return EXIT_FAILURE;
	}

	auto injector = make_di(std::string(szFileName));

	auto platform = injector.create<std::shared_ptr<engine::platform_t>>();
	auto logger = injector.create<std::shared_ptr<engine::logger_t>>();
	auto config = injector.create<std::shared_ptr<engine::config_t>>();
	auto database_data = injector.create<std::shared_ptr<engine::data::database_t>>();
	auto database_items = injector.create<std::shared_ptr<engine::data::database_items_t>>();
//	auto database_module = injector.create<std::shared_ptr<engine::module::database_t>>();
	auto logger_container = injector.create<std::shared_ptr<engine::logger_container_t>>();
	auto config_container = injector.create<std::shared_ptr<engine::config_container_t>>();

	platform->show_splashscreen(engine::virtual_path_t(_U("splashscreen/game.tga"), engine::virtual_path_t::type_t::modules));

	engine::color_t col = 0xff000000;
	/*
	auto asset_db = injector.create<std::shared_ptr<engine::asset_database_launcher>>();
	
	asset_db->rescan();

	engine::module_manifest_t manifest1(_U("base"));
	engine::module_manifest_t manifest2(_U("test1"));
	engine::module_manifest_t manifest3(_U("test2"));
	engine::module_manifest_t manifest_wrong(_U("wrong_manifest"));

	manifest1.save();
	manifest2.save();
	manifest3.save();

	engine::sub_module_manifest_t sub_manifest1(_U("crs"));
	engine::sub_module_manifest_t sub_manifest2(_U("crs2"));
	engine::sub_module_manifest_t sub_manifest3(_U("sample"));
	engine::sub_module_manifest_t sub_manifest4(_U("sample_invalid"));
	engine::sub_module_manifest_t sub_manifest5(_U("sample_invalid2"));
	
	sub_manifest1.save();
	sub_manifest2.save();
	sub_manifest3.save();
	sub_manifest4.save();

	auto monitor = injector.create<std::unique_ptr<engine::asset_monitor<engine::asset_database_game>>>();

	auto module_resolver = injector.create<std::shared_ptr<engine::module_resolver>>();
	engine::manifest_database_t database;

	bool is_first = true;

	monitor->change_running(config->get<bool>(engine::config_t::item_t::engine_live_reload_assets));
	*/

	int32_t i = 0;

	std::shared_ptr<engine::data::item_t<item_content_text_t>> item = database_items->get_item<item_content_text_t>(engine::virtual_path_t(_U("base/_manifest.info"), engine::virtual_path_t::type_t::modules));
	std::shared_ptr<engine::data::item_t<item_content_text_t>> item2 = database_items->get_item<item_content_text_t>(engine::virtual_path_t(_U("base/_manifest.info"), engine::virtual_path_t::type_t::modules));
	
	std::shared_ptr<engine::data::item_t<item_content_text_t>> item_deatached;

	bool saved = false;
	
	for (;;)
	{
		if (!saved && !item2->is_operation_pending())
		{
			item_deatached = database_items->deatach<item_content_text_t>(item2);
			saved = true;
			item2 = database_items->save_copy_as<item_content_text_t>(item, engine::virtual_path_t(_U("base/test.info"), engine::virtual_path_t::type_t::modules));
			item2 = nullptr;
			item = nullptr;
		}

		database_data->init_update();
		database_items->init_update();
		//		database_module->init_update();
		//		database_module->log_problems_since_last_update();

		config_container->init_update();
		logger_container->update();

		//		logger->p_msg(item->get<item_content_text_t>()->get_data());
		
		++i;
		logger->p_msg(_U("Test of huge outputs: #1#!"), i);

		if (i == 10000)
			break;
		
		/*
		if (i == 100)
		database_module->get_database_manifest()->create_module(_U("HelloWorld"));
		*/
		/*
		if (i == 100)
		{
		int * a = 0;
		*a = 5;
		break;
		}
		*/
		/*
		logger->p_msg(_U("Test of huge outputs: #1#!"), i);
		++i;
		if (i == 100) break;
		*/

		//		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 50));

		/*
		config->init_update();
		if(config->updated(engine::config_t::item_t::engine_live_reload_assets))
		monitor->change_running(config->get<bool>(engine::config_t::item_t::engine_live_reload_assets));
		monitor->init_update();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
		if (is_first ||	monitor->is_changed())
		{
		logger->p_msg(_U("Updating resources..."));

		is_first = false;

		engine::asset_changes changes = monitor->get_changes();
		for (auto & iter : changes)
		{
		switch (iter.get_type())
		{
		case engine::asset_change::added: logger->p_msg(_U("Added #1#"), iter.get_path()); break;
		case engine::asset_change::changed_directory: logger->p_msg(_U("Changed directory #1#"), iter.get_path()); break;
		case engine::asset_change::deleted: logger->p_msg(_U("Deleted #1#"), iter.get_path()); break;
		case engine::asset_change::updated: logger->p_msg(_U("Updated #1#"), iter.get_path()); break;
		}
		}

		engine::module_problems_t problems = monitor->get_database_current()->get_problems_state();
		if (!problems.empty())
		logger->p_warn(_U("There are problems with CURRENT configuration:"));
		for (auto & iter : problems)
		{
		switch (iter.get_type())
		{
		case engine::module_problem_t::module_manifest_corrupted: logger->p_err(_U("Module '#1#' has corrupted manifest and will be unavailable"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::module_does_not_exists: logger->p_err(_U("Module '#1#' required here: #2# is not available"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::module_requested_does_not_exists: logger->p_err(_U("Main module '#1#' set in config is not available"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::module_required_here: logger->p_err(_U("Module '#1#' is required here: #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::module_forbidden_here: logger->p_err(_U("... and forbidden here: #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_manifest_corrupted: logger->p_err(_U("Sub module '#1#' has corrupted manifest and will be unavailable"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_does_not_exists: logger->p_err(_U("Sub module '#1#' required here: #2# is not available"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_requested_does_not_exists: logger->p_err(_U("Sub module '#1#' set in config is not not available"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_required_here: logger->p_err(_U("Sub module '#1#' is required here: #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_forbidden_here: logger->p_err(_U("... and forbidden here: #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::requires_module_not_enabled: logger->p_err(_U("#2# required module '#1#' which is not enabled"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::forbids_module_enabled: logger->p_err(_U("#2# forbids module '#1#' which is enabled"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::submodules_forbids_here: logger->p_err(_U("#2# forbids sub module '#1#' which is enabled"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::forbidden_by_module: logger->p_err(_U("#1# is forbidden by #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		}
		}
		problems = module_resolver->get_problems();
		if (!problems.empty())
		logger->p_warn(_U("There are problems with GLOBAL modules:"));
		for (auto & iter : problems)
		{
		switch (iter.get_type())
		{
		case engine::module_problem_t::module_manifest_corrupted: logger->p_err(_U("Module '#1#' has corrupted manifest and will be unavailable"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::module_does_not_exists: logger->p_err(_U("Module '#1#' required here: #2# is not available"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::module_requested_does_not_exists: logger->p_err(_U("Main module '#1#' set in config is not available"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::module_required_here: logger->p_err(_U("Module '#1#' is required here: #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::module_forbidden_here: logger->p_err(_U("... and forbidden here: #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_manifest_corrupted: logger->p_err(_U("Sub module '#1#' has corrupted manifest and will be unavailable"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_does_not_exists: logger->p_err(_U("Sub module '#1#' required here: #2# is not available"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_requested_does_not_exists: logger->p_err(_U("Sub module '#1#' set in config is not not available"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_required_here: logger->p_err(_U("Sub module '#1#' is required here: #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::sub_module_forbidden_here: logger->p_err(_U("... and forbidden here: #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::requires_module_not_enabled: logger->p_err(_U("#2# required module '#1#' which is not enabled"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::forbids_module_enabled: logger->p_err(_U("#2# forbids module '#1#' which is enabled"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::submodules_forbids_here: logger->p_err(_U("#2# forbids sub module '#1#' which is enabled"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		case engine::module_problem_t::forbidden_by_module: logger->p_err(_U("#1# is forbidden by #2#"), iter.get_target_name(), iter.get_mod_callstack_ustring()); break;
		}
		}
		engine::asset_problems problems2 = monitor->get_database_current()->get_problems();
		if (!problems2.empty())
		logger->p_warn(_U("There are problems within ASSET configuration:"));
		for (auto & iter : problems2)
		{
		switch (iter.get_type())
		{
		case engine::asset_problem_t::type_t::no_base_item: logger->p_err(_U("Asset is missing base item: '#1#' (located in '#2#')"), iter.get_scanner1(), iter.get_path()); break;
		case engine::asset_problem_t::type_t::double_definitions: logger->p_err(_U("Asset is defined multiply times: '#1#' and in '#2#'"), iter.get_scanner1(), iter.get_scanner2()); break;
		case engine::asset_problem_t::type_t::defined_and_deleted: logger->p_err(_U("Asset is defined and deleted: '#1#' and in '#2#'"), iter.get_scanner1(), iter.get_scanner2()); break;
		}

		}
		}
		*/
	}

//	engine::module_resolver resolv(config, platform);

	/*
	engine::asset_scanner_files asf(logger);
	asf.scan(u8"modules/base");
	for (auto & iter : asf.get_all())
	{
		if (iter.second.is_deleted())
			logger->p_warn(_U("#1# - deleted"), iter.first);
		else
			logger->p_msg(_U("#1#"), iter.first);
	}
	asf.scan(u8"modules/test2");
	for (auto & iter : asf.get_all())
	{
		if (iter.second.is_deleted())
			logger->p_warn(_U("#1# - deleted"), iter.first);
		else
			logger->p_msg(_U("#1#"), iter.first);
	}
	asf.scan(u8"sub_modules/crs");
	for (auto & iter : asf.get_all())
	{
		if (iter.second.is_deleted())
			logger->p_warn(_U("#1# - deleted"), iter.first);
		else
			logger->p_msg(_U("#1#"), iter.first);
	}
	*/

	logger->p_msg(_U("Completed main thread!"));
	ReleaseMutex(handle);
	CloseHandle(handle);

	return EXIT_SUCCESS;
}

#endif