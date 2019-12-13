#ifndef AUIMOUSE_H
#define AUIMOUSE_H
#include <UIAutomationClient.h>
#include "AUILog.h"



namespace Automation{

	/*
	UI Element 각각에 대한 클래스
	*/
	class CAUIMouse : public Automation::CAUILog{

	public:
		int x;
		int y;
		CAUIMouse(int _x, int _y);
		~CAUIMouse();
		
		//x,y좌표가 주어지면 마우스를 해당 좌표 위치로 이동
		void MoveMouse(const int x,const  int y);
		void MouseLeftClick(int x, int y);
		void MouseLeftDoubleClick(int x, int y);
		void MouseRightClick(int x, int y);
	};
}
#endif