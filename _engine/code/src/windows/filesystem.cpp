/*

#include "component/filesystem.hpp"

#if PIXIE_WINDOWS

#include "core/data/output.hpp"

#include "utility/text/ustring.hpp"

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


std::shared_ptr<engine::filesystem_t::file_t> engine::filesystem_provider_winapi_t::fopen(const std::filesystem::path & path, filesystem_t::file_mode_t mode)
{
	std::wstring filename = path.wstring();

	HANDLE handle = INVALID_HANDLE_VALUE;

	if(mode == filesystem_t::file_mode_t::read)
		handle = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	else if(mode == filesystem_t::file_mode_t::write)
		handle = CreateFileW(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (handle == INVALID_HANDLE_VALUE)
		return nullptr;

	return std::make_shared<file_real_t>(handle);
}

engine::filesystem_provider_winapi_t::file_real_t::~file_real_t()
{
	CloseHandle(handle);
}

std::size_t engine::filesystem_provider_winapi_t::file_real_t::read(uint8_t * buffer, std::size_t size)
{
	DWORD read;
	ReadFile(handle, buffer, size, &read, nullptr);
	return read;
}

std::size_t engine::filesystem_provider_winapi_t::file_real_t::write(const uint8_t * buffer, std::size_t size)
{
	DWORD written;
	WriteFile(handle, buffer, size, &written, nullptr);
	return written;
}

std::size_t engine::filesystem_provider_winapi_t::file_real_t::tell()
{
	return SetFilePointer(handle, 0, nullptr, FILE_CURRENT);
}
bool engine::filesystem_provider_winapi_t::file_real_t::is_eof()
{
	return (SetFilePointer(handle, 0, nullptr, FILE_CURRENT) >= GetFileSize(handle, nullptr));
}

void engine::filesystem_provider_winapi_t::file_real_t::flush()
{
	FlushFileBuffers(handle);
}

void engine::filesystem_provider_winapi_t::file_real_t::seek(int32_t offset, file_seek_origin_t origin)
{
	switch (origin)
	{
	case file_seek_origin_t::begin: SetFilePointer(handle, offset, nullptr, FILE_BEGIN); break;
	case file_seek_origin_t::current: SetFilePointer(handle, offset, nullptr, FILE_CURRENT); break;
	case file_seek_origin_t::end: SetFilePointer(handle, offset, nullptr, FILE_END); break;
	}
}



#endif

*/