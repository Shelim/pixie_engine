#include "platform/filesystem.hpp"

#if PIXIE_WINDOWS

#include "utility/data/output.hpp"
#include "provider/save_location.hpp"

#include "windows/internal.hpp"
#include "utility/text/ustring.hpp"
#include "manifest_app.hpp"

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


void engine::platform::ensure_directory_exits(std::filesystem::path path)
{
	std::error_code ec;
	std::filesystem::create_directories(path, ec);
}


engine::platform::file_t engine::platform::fopen(const std::filesystem::path & path, file_mode_t mode)
{
	std::wstring filename = path.wstring();

	HANDLE handle = INVALID_HANDLE_VALUE;

	if(mode == file_mode_t::read)
		handle = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	else if(mode == file_mode_t::write)
		handle = CreateFileW(filename.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (handle == INVALID_HANDLE_VALUE)
		return nullptr;

	return handle;
}

void engine::platform::fclose(file_t file)
{
	CloseHandle(file);
}

std::size_t engine::platform::fread(uint8_t * buffer, std::size_t size, file_t file)
{
	DWORD read;
	ReadFile(file, buffer, size, &read, nullptr);
	return read;
}

std::size_t engine::platform::fwrite(const uint8_t * buffer, std::size_t size, file_t file)
{
	DWORD written;
	WriteFile(file, buffer, size, &written, nullptr);
	return written;
}

std::size_t engine::platform::ftell(file_t file)
{
	return SetFilePointer(file, 0, nullptr, FILE_CURRENT); 
}

void engine::platform::fseek(file_t file, int32_t offset, file_seek_origin_t origin)
{
	switch (origin)
	{
	case file_seek_origin_t::begin: SetFilePointer(file, offset, nullptr, FILE_BEGIN); break;
	case file_seek_origin_t::current: SetFilePointer(file, offset, nullptr, FILE_CURRENT); break;
	case file_seek_origin_t::end: SetFilePointer(file, offset, nullptr, FILE_END); break;
	}
}

bool engine::platform::is_eof(file_t file)
{
	return (SetFilePointer(file, 0, nullptr, FILE_CURRENT) >= GetFileSize(file, nullptr));
}

void engine::platform::fflush(file_t file)
{
	FlushFileBuffers(file);
}


#endif