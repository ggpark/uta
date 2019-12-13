/*
테스트용 메인 함수가 정의됨
*/

#include "AUIAutomation.h"
#include "AUILog.h"
#include "AUIElementLib.h"
#include "CommonType.h"
#include "CommonLib.h"
#include "AUIElement.h"
#include "AUIMouse.h"
#include <iostream>
using namespace Automation;
using namespace std;
/*
lstrcmp 를 사용해서 유니코드 문자열 비교 가능
*/
/*
IN32,INT64 자료형을 사용하면 32비트/64비트 관계없이 32/64 비트를 사용함. (부호 있음)
*/

int wmain(int argc, wchar_t* argv[]){
	
	setlocale(LC_CTYPE, "");

	CAUIAutomation* pAUI = new CAUIAutomation();
	//pAUI->TargetWindow(L"setup.exe", L"Policy Admin 설치");
	//pAUI->VerifyWindow(L"SETUP.EXE", L"Policy Admin 설치 ",3);
	//pAUI->CloseWindow(L"PRVCUI.EXE", L"AhnLab Privacy Management");
	//pAUI->CloseWindow(L"PRVCUI.EXE", L"AhnLab Privacy Management");
	//pAUI->CloseWindow(L"V3Lite4Exp", L"");
	delete pAUI;
	
	return 0;
}