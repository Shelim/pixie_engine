#ifndef ENGINE_COMPONENT_FILESYSTEM_PROVIDER_WINDOWS_HPP
#define ENGINE_COMPONENT_FILESYSTEM_PROVIDER_WINDOWS_HPP
#pragma once

#if PIXIE_WINDOWS

#include "component/filesystem/real.hpp"
#include <fstream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#undef min
#undef max

namespace engine
{

	class filesystem_provider_windows_t : public filesystem_provider_base_t
	{

	private:

		class file_output_windows_t : public file_output_t
		{

		public:

			file_output_windows_t(const std::filesystem::path & path) : handle(INVALID_HANDLE_VALUE)
			{
				std::wstring filename = path.wstring();

				const wchar_t * p = filename.c_str();
				wchar_t folder[MAX_PATH];
				const wchar_t *end;
				ZeroMemory(folder, MAX_PATH * sizeof(wchar_t));

				end = wcschr(p, L'\\');

				while (end != nullptr)
				{
					wcsncpy(folder, p, end - p + 1);
					CreateDirectoryW(folder, nullptr);
					end = wcschr(++end, L'\\');
				}

				handle = CreateFileW(p, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			}

			~file_output_windows_t()
			{
				CloseHandle(handle);
			}

			uint32_t write(const uint8_t * buffer, uint32_t size) final
			{
				uint32_t ret;
				WriteFile(handle, buffer, size, reinterpret_cast<LPDWORD>(&ret), nullptr);
				return ret;
			}

			void flush() final
			{
				FlushFileBuffers(handle);
			}

		private:

			HANDLE handle;

		};

		class file_input_windows_t : public file_input_t
		{

		public:

			file_input_windows_t(std::filesystem::path path)
			{
				std::wstring filename = path.wstring();

				handle = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

				seek(0, file_seek_origin_t::end);
				len = tell();
				seek(0, file_seek_origin_t::begin);
			}

			~file_input_windows_t()
			{
				CloseHandle(handle);
			}

			void seek(int32_t position, file_seek_origin_t origin) final
			{
				switch (origin)
				{
				case file_seek_origin_t::begin: SetFilePointer(handle, position, nullptr, FILE_BEGIN); break;
				case file_seek_origin_t::current: SetFilePointer(handle, position, nullptr, FILE_CURRENT); break;
				case file_seek_origin_t::end: SetFilePointer(handle, position, nullptr, FILE_END); break;
				}
			}
			bool is_eof() const final
			{
				return (tell() < len);
			}
			uint32_t tell() const final
			{
				return SetFilePointer(handle, 0, nullptr, FILE_CURRENT);
			}
			uint32_t read(uint8_t * buffer, uint32_t size) final
			{
				uint32_t ret;
				ReadFile(handle, buffer, size, reinterpret_cast<LPDWORD>(&ret), nullptr);
				return ret;
			}

		private:

			HANDLE handle;
			uint32_t len;
		};

		std::filesystem::file_time_type filetime_to_std_filetime(const FILETIME & ft)
		{
			SYSTEMTIME st = { 0 };
			if (!FileTimeToSystemTime(&ft, &st))
			{
				return std::chrono::system_clock::time_point((std::chrono::system_clock::time_point::min)());
			}

			// number of seconds 
			ULARGE_INTEGER ull;
			ull.LowPart = ft.dwLowDateTime;
			ull.HighPart = ft.dwHighDateTime;

			time_t secs = ull.QuadPart / 10000000ULL - 11644473600ULL;
			std::chrono::milliseconds ms((ull.QuadPart / 10000ULL) % 1000);

			auto tp = std::chrono::system_clock::from_time_t(secs);
			tp += ms;
			return tp;
		}

	public:

		filesystem_provider_windows_t() : tmp_item(0)
		{

		}

		std::filesystem::file_time_type get_mod_time(std::filesystem::path path) final
		{
			std::wstring filename = path.wstring();
			FILETIME writetime;

			HANDLE handle = CreateFileW(filename.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
			GetFileTime(handle, nullptr, nullptr, &writetime);
			CloseHandle(handle);

			return filetime_to_std_filetime(writetime);
		}

		paths_t iterate_files_in_directory(std::filesystem::path path, const ustring_t & pattern = "*.*"_u) final
		{
			std::wstring directory = path.wstring();
			if (directory.back() != '/' && directory.back() != '\\')
				directory += '\\';
			directory += pattern.to_wide();
			const wchar_t * p = directory.c_str();

			WIN32_FIND_DATA ffd;
			paths_t ret;

			HANDLE hFind = FindFirstFileW(p, &ffd);

			if (INVALID_HANDLE_VALUE == hFind)
			{
				return ret;
			}
			do
			{
				if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					if(match(pattern.get_cstring(), ffd.cFileName))
						ret.push_back(ffd.cFileName);
				}
			} while (FindNextFile(hFind, &ffd) != 0);

			return ret;
		}
		paths_t iterate_directories(std::filesystem::path path, const ustring_t & pattern = "*"_u) final
		{
			std::wstring directory = path.wstring();
			if (directory.back() != '/' && directory.back() != '\\')
				directory += '\\';
			directory += pattern.to_wide();
			const wchar_t * p = directory.c_str();

			WIN32_FIND_DATA ffd;
			paths_t ret;

			HANDLE hFind = FindFirstFileW(p, &ffd);

			if (INVALID_HANDLE_VALUE == hFind)
			{
				return ret;
			}
			do
			{
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (match(pattern.get_cstring(), ffd.cFileName) && wcscmp(ffd.cFileName, L".") && wcscmp(ffd.cFileName, L".."))
						ret.push_back(ffd.cFileName);
				}
			} while (FindNextFile(hFind, &ffd) != 0);

			return ret;
		}

		std::shared_ptr<file_output_t> output(std::filesystem::path path) final
		{
			return std::make_shared<file_output_windows_t>(path);
		}

		std::shared_ptr<file_input_t> input(std::filesystem::path path) final
		{
			return std::make_shared<file_input_windows_t>(path);
		}


		std::filesystem::path construct_temp_file() final
		{
			std::lock_guard<std::mutex> guard(tmp_mutex);

			wchar_t buf[MAX_PATH];
			GetTempPathW(MAX_PATH, buf);
			
			ustring_t path = ustring_t::from_wide(buf);
			path.append_ascii(PIXIE_PROJECT_NAME);

			CreateDirectoryW(buf, nullptr);

			std::filesystem::path tmp_file;

			do
			{

				tmp_file = path.get_cstring();
				ustring_t filename = format_string("#1#_#2#.tmp"_u, ustring_t::from_utf8(PIXIE_PROJECT_NAME), ++tmp_item);
				tmp_file /= filename.get_cstring();

			} while (exists(tmp_file));

			std::wstring target = tmp_file.wstring();

			HANDLE handle = CreateFileW(target.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			CloseHandle(handle);

			return tmp_file;
		}

		bool exists(std::filesystem::path path) final
		{
			std::wstring target = path.wstring();
			DWORD dwAttrib = GetFileAttributesW(target.c_str());

			return (dwAttrib != INVALID_FILE_ATTRIBUTES);
		}

		bool is_read_only(std::filesystem::path path) final
		{
			std::wstring target = path.wstring();
			DWORD dwAttrib = GetFileAttributesW(target.c_str());

			return (dwAttrib != INVALID_FILE_ATTRIBUTES) &&
				(dwAttrib & FILE_ATTRIBUTE_READONLY);
		}

		bool is_directory(std::filesystem::path path) final
		{
			std::wstring target = path.wstring();
			DWORD dwAttrib = GetFileAttributesW(target.c_str());

			return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
				(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
		}

		void copy_file(std::filesystem::path src, std::filesystem::path trg) final
		{
			ensure_target_directory_exists(trg);

			std::wstring source = src.wstring();
			std::wstring target = trg.wstring();
			CopyFileW(source.c_str(), target.c_str(), false);
		}

		void copy_directory(std::filesystem::path src, std::filesystem::path trg) final
		{
			for (auto path : iterate_files_in_directory(src))
			{
				copy_file(src / path, trg / path);
			}
			for (auto path : iterate_directories(src))
			{
				copy_directory(src / path, trg / path);
			}
		}

		void delete_file(std::filesystem::path path) final
		{
			std::wstring source = path.wstring();
			DeleteFileW(source.c_str());
		}

		void delete_directory(std::filesystem::path path) final
		{
			for (auto file : iterate_files_in_directory(path))
			{
				delete_file(path / file);
			}
			for (auto dir : iterate_directories(path))
			{
				delete_directory(path / dir);
			}
			std::wstring dir = path.wstring();
			RemoveDirectoryW(dir.c_str());
		}

		void move_file(std::filesystem::path src, std::filesystem::path trg) final
		{
			ensure_target_directory_exists(trg);

			std::wstring source = src.wstring();
			std::wstring target = trg.wstring();
			MoveFileExW(source.c_str(), target.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
		}

	private:
		int tmp_item;
		std::mutex tmp_mutex;

		void ensure_target_directory_exists(std::filesystem::path path)
		{
			std::wstring filename = path.wstring();

			const wchar_t * p = filename.c_str();
			wchar_t folder[MAX_PATH];
			const wchar_t *end;
			ZeroMemory(folder, MAX_PATH * sizeof(wchar_t));

			end = wcschr(p, L'\\');

			while (end != nullptr)
			{
				wcsncpy(folder, p, end - p + 1);
				CreateDirectoryW(folder, nullptr);
				end = wcschr(++end, L'\\');
			}
		}

	};
}

#endif

#endif