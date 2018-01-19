#include "utility/platform/debug.hpp"

#if PIXIE_WINDOWS

#include "debugbreak.h"
#include "core/data/output.hpp"

#include "utility/text/ustring.hpp"

#include <clocale>
#include <string>
#include <map>
#include <stack>
#include <mutex>

#include <SDL.h>
#include <SDL_syswm.h>

#define WIN32_LEAN_AND_MEAN
#define _NO_CVCONST_H
#include <windows.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shellapi.h>
#include <DbgHelp.h>

#include <cstdio>
#include <cstdlib>
#include <io.h>
#include <fcntl.h>

void engine::platform::trigger_breakpoint()
{
	debug_break();
}

namespace
{
	class symbol_t
	{

		public:

			symbol_t(intptr_t address, const engine::ustring_t & name) : address(address), name(name)
			{

			}

			intptr_t get_address() const
			{
				return address;
			}

			const engine::ustring_t & get_name() const
			{
				return name;
			}

		private:

			friend bool operator< (const symbol_t &lhs, const symbol_t &rhs);

			intptr_t address;
			engine::ustring_t name;
	};

    inline bool operator< (const symbol_t &lhs, const symbol_t &rhs)
    {
        return lhs.address < rhs.address;
    }

	typedef std::map<intptr_t, symbol_t> symbols_t;

	class module_t
	{

		public:

			module_t(HMODULE mod) : mod(mod)
			{
				wchar_t utf16_name[32 * 1024] = L"";

				const DWORD buffsize = sizeof utf16_name / sizeof *utf16_name;
				DWORD len = GetModuleFileNameW(mod, utf16_name, buffsize);

				if (len == buffsize)
					utf16_name[--len] = L'\0';

				name = engine::ustring_t::from_wide(utf16_name);

				HMODULE temp = 0;
				own_ref = GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, 
                                                 reinterpret_cast<const char *>(mod),
                                                 &temp);
				
				try_to_load_msvc_symbols();
			}

			~module_t()
			{
				if (own_ref)
					FreeLibrary(mod);
			}

			HMODULE get_mod() const
			{
				return mod;
			}

			const engine::ustring_t & get_name() const
			{
				return name;
			}

			const engine::ustring_t & resolve_symbol(intptr_t symbol) const
			{
				static engine::ustring_t def = ""_u;
				engine::ustring_t & ret = def;

				auto iter = symbols.upper_bound(symbol);
				if(iter != symbols.begin())
				{
					--iter;
					return iter->second.get_name();
				}

				return def;
			}

			void get_file_and_line_from_symbol(intptr_t symbol, engine::ustring_t & file, int & line) const
			{
				IMAGEHLP_LINE64 LineInfo; 
				std::memset(&LineInfo, 0, sizeof LineInfo);
				LineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
				DWORD LineDisplacement = 0;

				if(SymGetLineFromAddr64(GetCurrentProcess(), symbol, &LineDisplacement, &LineInfo))
				{
					file = engine::ustring_t::from_utf8(LineInfo.FileName);
					line = LineInfo.LineNumber;
				}
				
			}

		private:

			bool try_to_load_msvc_symbols()
			{
				std::lock_guard<std::mutex> guard(symbol_scanner);

				const HANDLE process = GetCurrentProcess();

				DWORD Options = SymGetOptions(); 
				Options |= SYMOPT_LOAD_LINES;
				SymSetOptions( Options ); 

				if(!SymInitialize(process, 0, TRUE))
					return false;

				std::wstring image_name = this->name.to_wide();

				BaseOfDll = SymLoadModuleEx(process,
                                NULL,
                                this->name.get_cstring(),
                                NULL,
                                0,
                                0,
                                NULL,
                                0);

				if(!SymEnumSymbols(process, BaseOfDll, "*", EnumSymProc, this))
				{
					DWORD error = GetLastError();
					SymCleanup(process);
					return false;
				}

				SymCleanup(process);
				return true;
			}

			static BOOL CALLBACK EnumSymProc( 
				PSYMBOL_INFO pSymInfo,   
				ULONG SymbolSize,      
				PVOID UserContext)
			{
				module_t * module = reinterpret_cast<module_t*>(UserContext);
				module->symbols.emplace(pSymInfo->Address - module->BaseOfDll, symbol_t(pSymInfo->Address - module->BaseOfDll, engine::ustring_t::from_utf8(pSymInfo->Name)));

				return TRUE;
			}

			bool own_ref;
			HMODULE mod;
			DWORD64 BaseOfDll;
			engine::ustring_t name;
			symbols_t symbols;
			static std::mutex symbol_scanner;

	};

	std::mutex module_t::symbol_scanner;
}

engine::callstack_t engine::platform::dump_callstack(std::size_t skip_front)
{
	static std::map<HMODULE, module_t> modules;

	STACKFRAME64 frame;
	std::memset(&frame, 0, sizeof frame);

	CONTEXT context;
	std::memset(&context, 0, sizeof context);
	context.ContextFlags = CONTEXT_FULL;

	RtlCaptureContext(&context);

#if defined(_M_AMD64)
	frame.AddrPC.Offset = context.Rip;
	frame.AddrPC.Mode = AddrModeFlat;
	frame.AddrStack.Offset = context.Rsp;
	frame.AddrStack.Mode = AddrModeFlat;
	frame.AddrFrame.Offset = context.Rbp;
	frame.AddrFrame.Mode = AddrModeFlat;

	const DWORD machine = IMAGE_FILE_MACHINE_AMD64;
#else
	frame.AddrPC.Offset = context.Eip;
	frame.AddrPC.Mode = AddrModeFlat;
	frame.AddrStack.Offset = context.Esp;
	frame.AddrStack.Mode = AddrModeFlat;
	frame.AddrFrame.Offset = context.Ebp;
	frame.AddrFrame.Mode = AddrModeFlat;

	const DWORD machine = IMAGE_FILE_MACHINE_I386;
#endif

	HANDLE process = GetCurrentProcess();
	HANDLE thread = GetCurrentThread();

	engine::callstack_t ret;
	
	unsigned level = 0;

	while (StackWalk64(machine, process, thread, &frame, &context, 0, SymFunctionTableAccess64, SymGetModuleBase64, 0))
	{
		while (skip_front)
		{
			--skip_front;
			continue; // don't capture current frame
		}

		intptr_t pc = frame.AddrPC.Offset;

		MEMORY_BASIC_INFORMATION mbinfo;
        std::memset(&mbinfo, 0, sizeof mbinfo);

		ustring_t data_module = ""_u;
		ustring_t data_file = ""_u;
		ustring_t data_function = ""_u;
		int data_line = -1;

		if (VirtualQuery(reinterpret_cast<const void *>(pc), &mbinfo, static_cast<DWORD>(sizeof(mbinfo))) == sizeof (mbinfo))
        {
			HMODULE mod = static_cast<HMODULE>(mbinfo.AllocationBase);
			auto module = modules.find(mod);

			if(module == modules.end())
				module = modules.emplace(mod, module_t(mod)).first;

			pc -= reinterpret_cast<intptr_t>(mbinfo.AllocationBase);

			data_module = module->second.get_name();
			data_function = module->second.resolve_symbol(pc);
			module->second.get_file_and_line_from_symbol(pc, data_file, data_line);
		}
	
		ret.items.emplace_back(pc, data_module, data_file, data_line, data_function);
	}

	return ret;
}

engine::ustring_t engine::platform::canonize_debug_filename(const ustring_t & filename)
{
	static std::string engine_tag = "_engine/code/";

	std::string file_tmp = filename.get_cstring();
	std::replace(file_tmp.begin(), file_tmp.end(), '\\', '/');

	std::size_t index = file_tmp.find(engine_tag);
	if (index != std::string::npos)
		file_tmp = file_tmp.substr(index);

	return ustring_t::from_utf8(file_tmp.c_str());
}

#endif