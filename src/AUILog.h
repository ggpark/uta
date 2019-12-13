#ifndef AUILOG_H
#define AUILOG_H
#include <fstream> //파일 입출력 관련 함수 사용 위함
#include "CommonType.h"
#include <locale>
#include <codecvt>
using namespace std;

#define LOG_MAX_LEN 2048

namespace Automation{

	//로그 기록을 위한 클래스
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

