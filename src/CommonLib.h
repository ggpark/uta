/*
��� ȭ�Ͽ��� ��� �� �� �ִ� �Լ����� ���� �մϴ�.
*/

#ifndef COMMONLIB_H
#define COMMONLIB_H

#include "AUILog.h"
#include "CommonType.h"
#include <UIAutomationClient.h>
#include <exception>

using namespace std;

namespace commonLib{
	void printELEMENTLIST(ELEMENTLIST& lst);
	void SetVariantAsI4(VARIANT &var, INT32 val);
	void SetVariantAsBSTR(VARIANT &var, PWCHAR text);
	DWORD GetProcessIdFromName(const PWCHAR processName);
	void UpdateUITree();
	wstring stringToWstring(string &s);
	string ProcessIdToName(DWORD processId);
}


#endif // !COMMONLIB_H
