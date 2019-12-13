#ifndef AUIAUTOMATION_H
#define AUIAUTOMATION_H

#include <UIAutomationClient.h>
#include "AUILog.h"

//전역 변수 선언
//해당 전역 변수를 2개 이상의 cpp파일에서 정의하면 중복정의 에러가 발생.
//해당 전역변수를 cpp에서 사용 하려면 AUIAutomation.h를 include 하자.
extern IUIAutomation* g_pAutomation;

namespace Automation{
	class CAUIElementLib;
	class CAUIElement;
	class CAUIWindow;
	class CAUIControl;

	//실제로 사용 될 인터페이스들이 노출될 클래스
	class CAUIAutomation : public Automation::CAUILog{

	public:
		CAUIAutomation();
			//IUIAumation 인터페이스를 pAutomation 포인터로 반환

		~CAUIAutomation();
			//객체가 소멸될 때 정리해야할 작업들을 작성
		
		//TO-DO : 실제로 RF에서 사용될 라이브러리
		void TargetWindowByName(PWCHAR szProcessName, PWCHAR szFindText);
		void TargetWindowByID(PWCHAR szProcessName, PWCHAR szAuotmationID);
		void VerifyTopWindowWait(PWCHAR szProcessName, PWCHAR szFindText, int waitTime);
		void VerifyTextWait(PWCHAR szProcessName, PWCHAR szFindText, int waitTime);
		void VerifySelectItem(PWCHAR szProcessName, PWCHAR szFindText);
		void VerifyUnSelectItem(PWCHAR szProcessName, PWCHAR szFindText);
		void VerifyCheckBoxON(PWCHAR szProcessName, PWCHAR szFindText);
		void VerifyCheckBoxOFF(PWCHAR szProcessName, PWCHAR szFindText);
		void MouseLeftByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID = NULL);
		void MouseRightByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID = NULL);
		void MouseLeftDoubleByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID = NULL);
		void MouseLeftByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID = NULL);
		void MouseRightByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID = NULL);
		void MouseLeftDoubleByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID = NULL);
		void CloseWindow(PWCHAR szProcessName, PWCHAR szFindText);
		void ClearWindows();
		void MouseLeftPreviousByName(PWCHAR szProcessName, PWCHAR szFindText,int iPreviousStep);
		void MouseLeftNextByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep);
		void MouseLeftDoublePreviousByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep);
		void MouseLeftDoubleNextByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep);
		void InvokeByID(PWCHAR szProcessName, PWCHAR szAutomationID);
		void ToggleByID(PWCHAR szProcessName, PWCHAR szAutomationID);
		void ToggleByName(PWCHAR szProcessName, PWCHAR szFindText);
		void InvokeByName(PWCHAR szProcessName, PWCHAR szFindText);
		void SelectByID(PWCHAR szProcessName, PWCHAR szAutomationID);
		void SelectByName(PWCHAR szProcessName, PWCHAR szFindText);
		PWCHAR GetTextById(PWCHAR szProcessName, PWCHAR szAutomationID);
		void SetTextByID(PWCHAR szProcessName, PWCHAR szAutomationID, PWCHAR szText);

		//아래 멤버들은 나중에 private으로 바꿔야함 (각 클래스를 메인에서 테스트 하기 위해 풀어놓은 상태)
		HRESULT InitializeUIAutomation(IUIAutomation **ppAutomation);
		CAUIElementLib* elementLib=NULL;
		CAUIElement* abstractElement = NULL;
		CAUIWindow* windowControl = NULL;
		CAUIControl* elementControl = NULL;
	};
}


#endif  // !AUIAUTOMATION_H