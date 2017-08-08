#include "platform/debug.hpp"

#if PIXIE_WINDOWS

#include "debugbreak.h"
#include "utility/data/output.hpp"
#include "provider/save_location.hpp"

#include "windows/internal.hpp"
#include "utility/text/ustring.hpp"
#include "manifest_app.hpp"

#include "StackWalker.h"

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

void engine::platform::breakpoint()
{
	debug_break();
}

namespace
{
	class stack_walker_adapter_t : public StackWalker
	{
	public:

		stack_walker_adapter_t(std::size_t skip_front, engine::callstack_t::items_t & output) :
			StackWalker(StackWalker::RetrieveVerbose | StackWalker::SymBuildPath), // do not use public Microsoft-Symbol-Server
			discard_idx(skip_front + 2),
			output(output)
		{

		}
		~stack_walker_adapter_t()
		{
		
		}

	protected:

		void OnCallstackEntry(CallstackEntryType /*eType*/, CallstackEntry &entry) final
		{
			if (entry.lineFileName[0] == 0)
				discard_idx = -1; // skip all entries from now on

								  // discard first N stack entries
			if (discard_idx > 0)
			{
				discard_idx--;
			}
			else if (discard_idx == 0) 
			{
				engine::callstack_t::item_t item(engine::platform::canonize_debug_filename(engine::ustring_t::from_utf8(entry.lineFileName)), entry.lineNumber, engine::ustring_t::from_utf8(entry.name));
				output.push_back(item);
			}
		}
		void OnOutput(LPCSTR /*szText*/) final
		{
			// discard (should never be called)
		}
		void OnSymInit(LPCSTR /*szSearchPath*/, DWORD /*symOptions*/, LPCSTR /*szUserName*/) final
		{
			// discard
		}
		void OnLoadModule(LPCSTR /*img*/, LPCSTR /*mod*/, DWORD64 /*baseAddr*/, DWORD /*size*/, DWORD /*result*/, LPCSTR /*symType*/, LPCSTR /*pdbName*/, ULONGLONG /*fileVersion*/) final
		{
			// discard
		}
		void OnDbgHelpErr(LPCSTR /*szFuncName*/, DWORD /*gle*/, DWORD64 /*addr*/)  final 
		{
			// discard
		}

	private:

		engine::callstack_t::items_t & output;

		int discard_idx; ///< the number of stack entries to discard
	};

}

engine::callstack_t engine::platform::dump_callstack(std::size_t skip_front)
{
	engine::callstack_t ret;
	stack_walker_adapter_t stack_waker_adapter(skip_front, ret.items);
	stack_waker_adapter.ShowCallstack();
	return ret;
}

engine::ustring_t engine::platform::canonize_debug_filename(const ustring_t & filename)
{
	static std::string engine_tag = "_engine/code/";
	static std::string app_tag = engine::compilation_t::app_unix_name(); app_tag += "/code/";

	std::string file_tmp = filename.get_cstring();
	std::replace(file_tmp.begin(), file_tmp.end(), '\\', '/');

	std::size_t index = file_tmp.find(engine_tag);
	if (index != std::string::npos)
		file_tmp = file_tmp.substr(index);

	index = file_tmp.find(app_tag);
	if (index != std::string::npos)
		file_tmp = file_tmp.substr(index);

	return ustring_t::from_utf8(file_tmp.c_str());
}

#endif