
#include "AUIMouse.h"

using namespace Automation;




CAUIMouse::CAUIMouse(int _x,int _y){
	this->x = _x;
	this->y = _y;
}
CAUIMouse::~CAUIMouse(){}


/*
//�Է¹��� X,Y ��ǥ ��ġ�� ���콺�� ���� �̵� ��Ų��.
*/
void CAUIMouse::MoveMouse(const int x,const int y){
	
	int xScreen =GetSystemMetrics(SM_CXSCREEN);
	int yScreen = GetSystemMetrics(SM_CYSCREEN);
	
	int reX = 65535 * x / xScreen;
	int reY = 65535 * y / yScreen;
	
	mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, (DWORD)reX, (DWORD)reY, 0, 0);
	//BOOL T =SetCursorPos(reX, reY);
	//if (!T){
	//	cout << "������ ���� : " << GetLastError() << endl;
	//}
	Sleep(200);
	
	return;
}

void CAUIMouse::MouseLeftClick(int x, int y){
	this->MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(MOUSE_CLICK_DELAY);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void CAUIMouse::MouseLeftDoubleClick(int x, int y){
	this->MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(MOUSE_CLICK_DELAY);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(MOUSE_CLICK_DELAY);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(MOUSE_CLICK_DELAY);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void CAUIMouse::MouseRightClick(int x, int y){
	this->MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
	Sleep(MOUSE_CLICK_DELAY);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}
