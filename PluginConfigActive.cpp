#include <Windows.h>
#include <string>
#include "../../API/RainmeterAPI.h"

struct Measure
{
	std::wstring configName;
	Measure() : configName(L"") {};
};

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
	Measure* measure = new Measure;
	*data = measure;
}

PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
	Measure* measure = (Measure*)data;
	measure->configName = RmReadString(rm, L"CONFIGNAME", L"");
}

PLUGIN_EXPORT double Update(void* data)
{
	Measure* measure = (Measure*)data;
	if (measure->configName.empty()) return 0.0;

	HWND trayWnd = FindWindow(L"RainmeterTrayClass", NULL);
	if (trayWnd)
	{
		COPYDATASTRUCT cds;
		cds.dwData = 5101;
		cds.cbData = (DWORD)(measure->configName.size() + 1) * sizeof(WCHAR);
		cds.lpData = (void*)measure->configName.c_str();

		if (SendMessage(trayWnd, WM_COPYDATA, 0, (LPARAM)&cds)) return 1.0;
	}

	return -1.0;
}

//PLUGIN_EXPORT LPCWSTR GetString(void* data)
//{
//   Measure* measure = (Measure*)data;
//   return L"";
//}

//PLUGIN_EXPORT void ExecuteBang(void* data, LPCWSTR args)
//{
//   Measure* measure = (Measure*)data;
//}

PLUGIN_EXPORT LPCWSTR IsActive(void* data, const int argc, const WCHAR* argv[])
{
   Measure* measure = (Measure*)data;
   if (argc > 0) {
	   
	   std::wstring svConfigName = argv[0];
	   HWND trayWnd = FindWindow(L"RainmeterTrayClass", NULL);
	   if (trayWnd)
	   {
		   COPYDATASTRUCT cds;
		   cds.dwData = 5101;
		   cds.cbData = (DWORD)(svConfigName.size() + 1) * sizeof(WCHAR);
		   cds.lpData = (void*)svConfigName.c_str();

		   if (SendMessage(trayWnd, WM_COPYDATA, 0, (LPARAM)&cds)) {
			   return L"1";
		   }
		   else
		   {
			   return L"-1";
		   }
	   }

	   return L"0";

   }
   return L"0";
}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;
	delete measure;
}
