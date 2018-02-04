#include "utility/platform/debug.hpp"

#if PIXIE_WINDOWS

#include "debugbreak.h"
#include "global/core/data/output.hpp"

#include "utility/text/ustring.hpp"

#include <clocale>
#include <string>
#include <map>
#include <stack>
#include <mutex>

#include <SDL.h>
#include <SDL_syswm.h>

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

			symbol_t(intptr_t address, const engine::ustring_t & name, const engine::ustring_t & file, int line) : address(address), name(name), file(file), line(line)
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

			const engine::ustring_t & get_file() const
			{
				return file;
			}

			int get_line() const
			{
				return line;
			}

		private:

			friend bool operator< (const symbol_t &lhs, const symbol_t &rhs);

			intptr_t address;
			engine::ustring_t name;
			engine::ustring_t file;
			int line;
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

			const symbol_t & resolve_symbol(intptr_t symbol) const
			{
				static symbol_t def(0, ""_u, ""_u, -1);

				auto iter = symbols.upper_bound(symbol);
				if(iter != symbols.begin())
				{
					--iter;
					return iter->second;
				}

				return def;
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
				engine::ustring_t file = ""_u;
				int line = -1;

				IMAGEHLP_LINE64 LineInfo; 
				std::memset(&LineInfo, 0, sizeof LineInfo);
				LineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
				DWORD LineDisplacement = 0;

				module_t * module = reinterpret_cast<module_t*>(UserContext);

				if(SymGetLineFromAddr64(GetCurrentProcess(), pSymInfo->Address - module->BaseOfDll, &LineDisplacement, &LineInfo))
				{
					file = engine::ustring_t::from_utf8(LineInfo.FileName);
					line = LineInfo.LineNumber;
				}

				module->symbols.emplace(pSymInfo->Address - module->BaseOfDll, symbol_t(pSymInfo->Address - module->BaseOfDll, engine::ustring_t::from_utf8(pSymInfo->Name), file, line));

				return TRUE;
			}

			bool own_ref;
			HMODULE mod;
			DWORD64 BaseOfDll;
			DWORD64 BaseOfPdb;
			engine::ustring_t name;
			symbols_t symbols;
			static std::mutex symbol_scanner;

	};

	std::mutex module_t::symbol_scanner;
}

engine::callstack_t engine::platform::dump_callstack(std::size_t skip_front)
{
	static std::map<HMODULE, module_t> modules;
	const int max_callers = 62; 
	callstack_t ret;

	intptr_t stack[max_callers];

	typedef USHORT (WINAPI *capture_stack_back_trace_type)(__in ULONG, __in ULONG, __out PVOID*, __out_opt PULONG);
    capture_stack_back_trace_type func = (capture_stack_back_trace_type)(GetProcAddress(LoadLibrary(L"kernel32.dll"), "RtlCaptureStackBackTrace"));

    if(func == NULL)
        return ret; // WOE 29.SEP.2010

	unsigned short frames = (func)( skip_front, max_callers - skip_front, reinterpret_cast<void**>(stack), NULL );

	for(unsigned short i = 1; i < frames; i++)
	{
		intptr_t pc =stack[i];

		ustring_t data_module = ""_u;
		ustring_t data_file = ""_u;
		ustring_t data_function = ""_u;
		int data_line = -1;

		MEMORY_BASIC_INFORMATION mbinfo;
        std::memset(&mbinfo, 0, sizeof mbinfo);

		if (VirtualQuery(reinterpret_cast<const void *>(pc), &mbinfo, static_cast<DWORD>(sizeof(mbinfo))) == sizeof (mbinfo))
        {
			HMODULE mod = static_cast<HMODULE>(mbinfo.AllocationBase);
			auto module = modules.find(mod);

			if(module == modules.end())
				module = modules.emplace(mod, module_t(mod)).first;

			pc -= reinterpret_cast<intptr_t>(mbinfo.AllocationBase);

			data_module = module->second.get_name();
			auto & symbol = module->second.resolve_symbol(pc);

			ret.items.emplace_back(pc, data_module, symbol.get_file(), symbol.get_line(), symbol.get_name());
		}
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