#include <Windows.h>
#include "../../API/RainmeterAPI.h"
#include <string>

#define RAINMETER_QUERY_ID_SKIN_WINDOWHANDLE	5101
#define RAINMETER_SKIN_CLASSNAME L"RainmeterMeterWindow"
#define RAINMETER_TRAY_CLASSNAME L"RainmeterTrayClass"

enum class Type
{
	SKIN,
	CONFIG,
	LOADEDCOUNT
};

struct Measure
{
	Measure() {}

	std::wstring m_SkinPath;
	std::wstring m_ConfigName;
	std::wstring m_SkinName;
	int m_Index = 0;
	Type m_Type = Type::SKIN;

	std::wstring m_String;
	std::wstring m_SectionVariable;
};

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
	Measure* measure = new Measure;
	*data = measure;
}

PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
	Measure* measure = (Measure*)data;

	measure->m_SkinPath = RmReplaceVariables(rm, L"#SKINSPATH#");
	measure->m_ConfigName = RmReadString(rm, L"ConfigName", L"");
	measure->m_Index = RmReadInt(rm, L"Index", 1);

	std::wstring type = RmReadString(rm, L"Type", L"Skin");

	if (_wcsicmp(type.c_str(), L"SKIN") == 0) measure->m_Type = Type::SKIN;
	else if (_wcsicmp(type.c_str(), L"CONFIG") == 0) measure->m_Type = Type::CONFIG;
	else if (_wcsicmp(type.c_str(), L"COUNT") == 0) measure->m_Type = Type::LOADEDCOUNT;
}

HWND GetLoadedConfig(const std::wstring& configName)
{
	HWND trayHWND = FindWindow(RAINMETER_TRAY_CLASSNAME, NULL);
	if (trayHWND == NULL) return NULL;

	std::wstring config = configName;
	for(int i = 0; i < config.size()-1; ++i)
	{
		if (config[i] == L'\\' && config[i + 1] == L'\\') {
			config.erase(i, 1);
		}
	}

	COPYDATASTRUCT cds = {};
	cds.dwData = RAINMETER_QUERY_ID_SKIN_WINDOWHANDLE;
	cds.cbData = (DWORD)(config.size() + 1) * sizeof(WCHAR);
	cds.lpData = (void*)config.c_str();

	return (HWND)SendMessage(trayHWND, WM_COPYDATA, 0, (LPARAM)&cds);;
}

PLUGIN_EXPORT LPCWSTR IsHidden(void* data, const int argc, const WCHAR* argv[])
{
	HWND trayHWND = FindWindow(RAINMETER_TRAY_CLASSNAME, NULL);
	if (trayHWND == NULL) return NULL;

	COPYDATASTRUCT cds = {};
	cds.dwData = RAINMETER_QUERY_ID_SKIN_WINDOWHANDLE;
	cds.cbData = (DWORD)(wcslen(argv[0]) + 1) * sizeof(WCHAR);
	cds.lpData = (void*)argv[0];

	HWND configHandle = (HWND)SendMessage(trayHWND, WM_COPYDATA, 0, (LPARAM)&cds);
	if (configHandle == NULL) return L"0";

	BOOL visibleState = IsWindowVisible(configHandle);
	if (!visibleState) {
		return L"1";
	}
	return L"-1";
}

std::wstring GetWindowTitle(HWND hwnd)
{
	const int length = GetWindowTextLength(hwnd);
	std::wstring buff;
	buff.resize(length+1);
	GetWindowText(hwnd, &buff[0], (int)buff.size());
	return buff;
}

HWND QueryConfig(int index)
{
	HWND hwnd = NULL;
	int current = index;
	do
	{
		hwnd = FindWindowEx(NULL, hwnd, RAINMETER_SKIN_CLASSNAME, NULL);
		// limit reached 
		if (hwnd == NULL) break;
		current--;
	} while (current != 0);
	return hwnd;
}

int LoadedSkinCount()
{
	HWND hwnd = NULL;
	int count = 0;
	do
	{
		hwnd = FindWindowEx(NULL, hwnd, RAINMETER_SKIN_CLASSNAME, NULL);
		if (hwnd == NULL) break;
		count++;
	} while (true);
	return count;
}

PLUGIN_EXPORT double Update(void* data)
{
	Measure* measure = (Measure*)data;

	HWND config;
	if(!measure->m_ConfigName.empty())
	{
		// index is irrelevant here as there can only be one config
		config = GetLoadedConfig(measure->m_ConfigName);
	}
	else
	{
		// Fetch the config based on index instead
		config = QueryConfig(measure->m_Index);
	}

	if (config == NULL)
	{
		// Specified config is not loaded, so clear the return string
		measure->m_String.clear();
		return -1;
	}

	std::wstring title = GetWindowTitle(config);
	const size_t last = title.find_last_of(L"\\");

	switch(measure->m_Type)
	{
	case Type::SKIN: 
		// Title is not set correctly, so we can't retrieve the skin. The config is still loaded.
		if (last == std::wstring::npos && last + 1 < title.size()) return 1;
		measure->m_String = title.substr(last + 1);
		break;
	case Type::CONFIG: 
	{
		measure->m_String = measure->m_ConfigName;
		// Title is not set correctly, so we can't retrieve the skin. The config is still loaded.
		if (last == std::wstring::npos && last + 1 < title.size()) return 1;
		if (!measure->m_ConfigName.empty()) return 1;

		size_t start = measure->m_SkinPath.size();
		measure->m_String = title.substr(start, last - start);
	}
		break;
	case Type::LOADEDCOUNT: 
		measure->m_String.clear();
		return LoadedSkinCount();
	}

	return 1;
}

PLUGIN_EXPORT LPCWSTR GetString(void* data)
{
	Measure* measure = (Measure*)data;
	return measure->m_String.c_str();
}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;
	delete measure;
}

PLUGIN_EXPORT LPCWSTR IsActive(void* data, const int argc, const WCHAR* argv[])
{
	if (argc > 0) return GetLoadedConfig(argv[0]) ? L"1" : L"-1";
	return nullptr;
}

PLUGIN_EXPORT LPCWSTR ConfigVariantName(void* data, const int argc, const WCHAR* argv[])
{
	if (argc > 0)
	{
		Measure* measure = (Measure*)data;
		HWND config = GetLoadedConfig(argv[0]);
		if (!config) return L"";

		std::wstring title = GetWindowTitle(config);
		size_t last = title.find_last_of(L"\\");
		// Title is not set correctly, so we can't retrieve the skin. The config is still loaded.
		if (last == std::wstring::npos && last + 1 < title.size()) return L"";
		measure->m_SectionVariable = title.substr(last + 1);
		return measure->m_SectionVariable.c_str();

	}
	return nullptr;
}

PLUGIN_EXPORT LPCWSTR LoadedCount(void* data, const int argc, const WCHAR* argv[])
{
	Measure* measure = (Measure*)data;
	measure->m_SectionVariable = std::to_wstring(LoadedSkinCount());
	return measure->m_SectionVariable.c_str();
}
