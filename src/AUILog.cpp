#include "AUILog.h"
#include "CommonType.h"
#include "CommonLib.h"

using namespace Automation;

CAUILog::CAUILog(){
	//���߿� config�� ����� ������ ������ �о�ͼ� �ʱ�ȭ �Ѵ�.
	//����׷α� �ɼ�
	//�α����� ���

	//������ �����ϱ� ���� ������ clear �Ѵ�.
	DeleteFileW(this->szLogFileDir);
	logFout.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>));
	this->logFout.open(this->szLogFileDir, ios::app);
}

CAUILog::~CAUILog(){
	//���� close
	logFout.close();
}

wstring CAUILog::CurrentDateTime() {
	time_t     now = time(0); //���� �ð��� time_t Ÿ������ ����
	struct tm  tstruct;
	wchar_t       cBuff[80];
	localtime_s(&tstruct, &now);
	wcsftime(cBuff, sizeof(cBuff), L"%Y-%m-%d.%X", &tstruct); // YYYY-MM-DD.HH:mm:ss ������ ��Ʈ��

	return cBuff;
}



//����� �α׸� ����.
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
����� �α׸� ����.
exception �޽����� wstring���� ��ȯ�Ͽ� �α����Ͽ� ����.
*/
void CAUILog::Log2(wstring szLogLevel, wstring szLogContent, const char* errorMsg){

	wstring expMsg = commonLib::stringToWstring((string)errorMsg);
	wstring szCurTime = CurrentDateTime();
	logFout << '[' << szCurTime << ']' << '[' << szLogLevel << ']' << szLogContent << "\n[EXCEPTION ERROR Message]" << expMsg << endl;
}



