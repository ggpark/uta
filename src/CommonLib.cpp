
#include "CommonLib.h"
#include <tlhelp32.h>
#include <windows.h>
#include "psapi.h"

namespace commonLib{

	void PrintELEMENTLIST(ELEMENTLIST& lst){
		ELEMENTLIST::iterator iter;
		BSTR name = NULL;
		int i = 1;
		for (iter = lst.begin(); iter != lst.end(); iter++){
			if (wcout.fail()) {
				wcout.clear();
				wcout.flush();
			}
			(*iter)->get_CurrentName(&name);
			wcout << name << endl;
		}
	}

	void SetVariantAsI4(VARIANT &var, INT32 val){
		//Window Control Type
		var.vt = VT_I4;
		if (true){
			//조건문에 VT_I4에 해당하는 컨트롤 타입 ID인지 확인하는 것 찾아서 넣기
			var.vt = VT_I4;
			var.lVal = val;
		}
	}

	void SetVariantAsBSTR(VARIANT &var, PWCHAR val){

		if (true){
			//조건문에 VT_BSTR에 해당하는 컨트롤 타입 ID인지 확인하는 것 찾아서 넣기
			var.vt = VT_BSTR;
			var.bstrVal = SysAllocString(val);  // BSTR == wchar_t* 이다.
		}
	}

	DWORD GetProcessIdFromName(const PWCHAR processName){

		PROCESSENTRY32 processInfo;
		processInfo.dwSize = sizeof(processInfo);

		HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		if (processesSnapshot == INVALID_HANDLE_VALUE) {
			return 0;
		}

		Process32First(processesSnapshot, &processInfo);
		if (_wcsicmp(processName, processInfo.szExeFile) == 0)
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}

		while (Process32Next(processesSnapshot, &processInfo))
		{

			if (_wcsicmp(processName, processInfo.szExeFile) == 0)
			{
				CloseHandle(processesSnapshot);
				return processInfo.th32ProcessID;
			}
		}

		CloseHandle(processesSnapshot);
		return 0;
	}



	void UpdateUITree(){
		SystemParametersInfo(SPI_SETSCREENREADER, TRUE, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
		PostMessage(HWND_BROADCAST, WM_WININICHANGE, SPI_SETSCREENREADER, 0);
	}

	wstring stringToWstring(string &s){
		wstring ws;
		ws.assign(s.begin(), s.end());
		return ws;
	}

	std::string ProcessIdToName(DWORD processId)
	{
		std::string ret;
		HANDLE handle = OpenProcess(
			PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE,
			processId /* This is the PID, you can find one from windows task manager */
			);
		if (handle)
		{
			DWORD buffSize = 1024;
			CHAR buffer[1024];
			if (QueryFullProcessImageNameA(handle, 0, buffer, &buffSize))
			{
				ret = buffer;
			}
			else
			{
				printf("Error GetModuleBaseNameA : %lu", GetLastError());
			}
			CloseHandle(handle);
		}
		else
		{
			printf("Error OpenProcess : %lu", GetLastError());
		}

		std::string base_filename = ret.substr(ret.find_last_of("/\\") + 1);
		return base_filename;
	}
}

