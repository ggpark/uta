

/*
�ַ� ������� �ʴ� �κе��� �����մϴ�.
*/
#ifndef COMMONTYPE_H
#define COMMONTYPE_H

#include <UIAutomationClient.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <string>
#include <winUser.h>
#include <windows.h>

#define MOUSE_CLICK_DELAY 100 //���콺 Ŭ�� ������ 0.1��
using namespace std;

//���콺 Ŭ�� ����
enum MOUSECLICKTYPE{
	NONE,
	MOUSELEFTCLICK,
	MOUSERIGHTCLICK,
	MOUSELEFTDOUBLECLICK,
};

//������Ʈ�� ã�� ����� �����ϱ� ���� ������ Ÿ�� 
typedef vector<IUIAutomationElement*> ELEMENTLIST;

//All Control Pattern Property Identifiers

#endif // !COMMONTYPE_H

