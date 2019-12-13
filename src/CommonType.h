

/*
주로 변경되지 않는 부분들을 선언합니다.
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

#define MOUSE_CLICK_DELAY 100 //마우스 클릭 딜레이 0.1초
using namespace std;

//마우스 클릭 종류
enum MOUSECLICKTYPE{
	NONE,
	MOUSELEFTCLICK,
	MOUSERIGHTCLICK,
	MOUSELEFTDOUBLECLICK,
};

//엘리먼트를 찾은 결과를 저장하기 위한 데이터 타입 
typedef vector<IUIAutomationElement*> ELEMENTLIST;

//All Control Pattern Property Identifiers

#endif // !COMMONTYPE_H

