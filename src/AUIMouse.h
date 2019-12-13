#ifndef AUIMOUSE_H
#define AUIMOUSE_H
#include <UIAutomationClient.h>
#include "AUILog.h"



namespace Automation{

	/*
	UI Element ������ ���� Ŭ����
	*/
	class CAUIMouse : public Automation::CAUILog{

	public:
		int x;
		int y;
		CAUIMouse(int _x, int _y);
		~CAUIMouse();
		
		//x,y��ǥ�� �־����� ���콺�� �ش� ��ǥ ��ġ�� �̵�
		void MoveMouse(const int x,const  int y);
		void MouseLeftClick(int x, int y);
		void MouseLeftDoubleClick(int x, int y);
		void MouseRightClick(int x, int y);
	};
}
#endif