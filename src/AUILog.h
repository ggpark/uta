#ifndef AUILOG_H
#define AUILOG_H
#include <fstream> //���� ����� ���� �Լ� ��� ����
#include "CommonType.h"
#include <locale>
#include <codecvt>
using namespace std;

#define LOG_MAX_LEN 2048

namespace Automation{

	//�α� ����� ���� Ŭ����
	class CAUILog {

	private:
		wofstream logFout;
		wchar_t* szLogFileDir = L"c:\\temp\\AUIAutomationLog.txt";
		bool bLogEnable;
		wstring CurrentDateTime();
	public:
		CAUILog();
		~CAUILog();
		void Log(wchar_t* szLogLevel, wchar_t* szLogFormat, ...);
		void Log2(wstring szLogLevel, wstring szLogContent, const char* errorMsg);
	};
}


#endif // !AUILOG_H

