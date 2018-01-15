#include "component/config_storage/provider/windows_registry.hpp"

#if PIXIE_WINDOWS

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

engine::config_storage_provider_windows_registry_t::config_storage_provider_windows_registry_t(std::shared_ptr<manifest_windows_t> manifest_windows, std::unique_ptr<service_t<config_storage_provider_windows_registry_rescan_service_t> > config_storage_provider_windows_registry_rescan_service) : manifest_windows(manifest_windows), config_storage_provider_windows_registry_rescan_service(std::move(config_storage_provider_windows_registry_rescan_service))
{
	this->config_storage_provider_windows_registry_rescan_service->start();
}

void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, bool val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD data = 0;
		if (val) data = 1;
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, uint8_t val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD data = val;
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, uint16_t val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD data = val;
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, uint32_t val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD data = val;
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, uint64_t val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_QWORD, (LPBYTE)&val, sizeof(val));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, int8_t val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		int32_t val_long = val;
		DWORD data = *(DWORD*)(void*)&val_long;
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, int16_t val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		int32_t val_long = val;
		DWORD data = *(DWORD*)(void*)&val_long;
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, int32_t val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD data = *(DWORD*)(void*)&val;
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, int64_t val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_QWORD, (LPBYTE)&val, sizeof(val));

		RegCloseKey(hkey);
	}
}
void engine::config_storage_provider_windows_registry_t::store(const ustring_t & key, const ustring_t & val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, nullptr, REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hkey, nullptr);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		std::wstring data = val.to_wide();
		RegSetValueExW(hkey, key_name.c_str(), 0, REG_SZ, (LPBYTE)data.c_str(), (data.length() * sizeof(std::wstring::value_type)) + 1);

		RegCloseKey(hkey);
	}
}

bool engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, bool def_val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD result;
		DWORD size = sizeof(DWORD);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_DWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return result;
	}
	return def_val;
}
uint8_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, uint8_t def_val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD result;
		DWORD size = sizeof(DWORD);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_DWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return result;
	}
	return def_val;
}
uint16_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, uint16_t def_val)
{

	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD result;
		DWORD size = sizeof(DWORD);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_DWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return result;
	}
	return def_val;
}
uint32_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, uint32_t def_val)
{

	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD result;
		DWORD size = sizeof(DWORD);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_DWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return result;
	}
	return def_val;
}
uint64_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, uint64_t def_val)
{

	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		uint64_t result;
		DWORD size = sizeof(uint64_t);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_QWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return result;
	}
	return def_val;
}
int8_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, int8_t def_val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD result;
		DWORD size = sizeof(DWORD);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_DWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return *(int32_t*)(void*)&result;
	}
	return def_val;
}
int16_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, int16_t def_val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD result;
		DWORD size = sizeof(DWORD);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_DWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return *(int32_t*)(void*)&result;
	}
	return def_val;
}
int32_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, int32_t def_val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD result;
		DWORD size = sizeof(DWORD);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_DWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return *(int32_t*)(void*)&result;
	}
	return def_val;
}
int64_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, int64_t def_val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		uint64_t result;
		DWORD size = sizeof(uint64_t);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_QWORD, nullptr, &result, &size);
		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return *(int64_t*)(void*)&result;
	}
	return def_val;
}
engine::ustring_t engine::config_storage_provider_windows_registry_t::retrieve(const ustring_t & key, const ustring_t & def_val)
{
	HKEY hkey;
	std::wstring path = L"SOFTWARE\\";
	path.append(manifest_windows->get_storage_registry().replace("/"_u, "\\"_u).to_wide());

	LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, path.c_str(), 0, KEY_READ, &hkey);

	if (res == ERROR_SUCCESS)
	{
		std::wstring key_name = key.to_wide();
		DWORD size = sizeof(uint64_t);
		res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_SZ, nullptr, nullptr, &size);

		std::wstring str;

		if (res == ERROR_SUCCESS)
		{
			str.resize((size - 1) / 2);
			res = RegGetValueW(hkey, nullptr, key_name.c_str(), RRF_RT_REG_SZ, nullptr, &str[0], &size);
		}

		RegCloseKey(hkey);

		if (res == ERROR_SUCCESS)
			return ustring_t::from_wide(str.c_str());
	}
	return def_val;
}


#endif