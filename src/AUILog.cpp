#include "AUILog.h"
#include "CommonType.h"
#include "CommonLib.h"

using namespace Automation;

CAUILog::CAUILog(){
	//나중에 config로 사용할 파일의 정보를 읽어와서 초기화 한다.
	//디버그로그 옵션
	//로그파일 경로

	//파일을 오픈하기 전에 내용을 clear 한다.
	DeleteFileW(this->szLogFileDir);
	logFout.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>));
	this->logFout.open(this->szLogFileDir, ios::app);
}

CAUILog::~CAUILog(){
	//파일 close
	logFout.close();
}

wstring CAUILog::CurrentDateTime() {
	time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
	struct tm  tstruct;
	wchar_t       cBuff[80];
	localtime_s(&tstruct, &now);
	wcsftime(cBuff, sizeof(cBuff), L"%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링

	return cBuff;
}



//디버그 로그를 쓴다.
void CAUILog::Log(wchar_t* szLogLevel, wchar_t* szLogFormat, ...){
	wchar_t szLogContent[LOG_MAX_LEN];
	va_list Marker;
	va_start(Marker, szLogFormat);
	vswprintf_s(szLogContent, szLogFormat, Marker);
	va_end(Marker);
	wstring szCurTime = CurrentDateTime();
	logFout << '[' << szCurTime << ']' << '[' << szLogLevel << ']' << ' ' << szLogContent << endl;
}

/**
디버그 로그를 쓴다.
exception 메시지를 wstring으로 변환하여 로그파일에 쓴다.
*/
void CAUILog::Log2(wstring szLogLevel, wstring szLogContent, const char* errorMsg){

	wstring expMsg = commonLib::stringToWstring((string)errorMsg);
	wstring szCurTime = CurrentDateTime();
	logFout << '[' << szCurTime << ']' << '[' << szLogLevel << ']' << szLogContent << "\n[EXCEPTION ERROR Message]" << expMsg << endl;
}



