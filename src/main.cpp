/*
�׽�Ʈ�� ���� �Լ��� ���ǵ�
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
lstrcmp �� ����ؼ� �����ڵ� ���ڿ� �� ����
*/
/*
IN32,INT64 �ڷ����� ����ϸ� 32��Ʈ/64��Ʈ ������� 32/64 ��Ʈ�� �����. (��ȣ ����)
*/

int wmain(int argc, wchar_t* argv[]){
	
	setlocale(LC_CTYPE, "");

	CAUIAutomation* pAUI = new CAUIAutomation();
	//pAUI->TargetWindow(L"setup.exe", L"Policy Admin ��ġ");
	//pAUI->VerifyWindow(L"SETUP.EXE", L"Policy Admin ��ġ ",3);
	//pAUI->CloseWindow(L"PRVCUI.EXE", L"AhnLab Privacy Management");
	//pAUI->CloseWindow(L"PRVCUI.EXE", L"AhnLab Privacy Management");
	//pAUI->CloseWindow(L"V3Lite4Exp", L"");
	delete pAUI;
	
	return 0;
}