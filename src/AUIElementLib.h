#ifndef AUIELEMENTLIB_H
#define AUIELEMENTLIB_H
#include <UIAutomationClient.h>
#include "CommonType.h"
#include "AUILog.h"

namespace Automation{
	class CAUIElement;

	/*
	UI Element 관련 기능을 제공하는 클래스
	*/
	class CAUIElementLib : public Automation::CAUILog{
	
	private:
		ELEMENTLIST ListElementArray(IUIAutomationElementArray* pElementArray);
		IUIAutomationElement* FindFirst(IUIAutomationElement* pStartElement, TreeScope scope, IUIAutomationCondition* condition);
		IUIAutomationElementArray* FindAll(IUIAutomationElement* pStartElement, TreeScope scope, IUIAutomationCondition* condition);
		
	public :
		IUIAutomationElement* pRootElement;
		CAUIElementLib();
		~CAUIElementLib();
		ELEMENTLIST GetALLElements();
		IUIAutomationElementArray* GetTopElements(); // desktop window를 부모로 갖는 window 엘리먼트들을 모두 반환
		IUIAutomationElement* GetTopElement(PWCHAR szProcessName);
		IUIAutomationElement* GetTopElement(PWCHAR szProcessName, PWCHAR szFindText);
		IUIAutomationElement* GetParentElement(IUIAutomationElement* pChildElement);
		IUIAutomationElement* GetPreviousElement(IUIAutomationElement* pIUIElement,int iPrevious=1);
		IUIAutomationElement* GetNextElement(IUIAutomationElement* pIUIElement, int iNext=1);
		IUIAutomationElement* FindElementByIDFromElement(IUIAutomationElement* pStartElement, PWCHAR szAutomationID, int controlTypeID);
		IUIAutomationElement* FindElementByNameFromElement(IUIAutomationElement* pStartElement, PWCHAR szElementName, int controlTypeID);
		IUIAutomationElement* FindElementByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID=NULL);
		IUIAutomationElement* FindElementByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID=NULL);
		bool IsBelongToForegroundWindow(IUIAutomationElement* element);
		bool IsTopWindow(IUIAutomationElement* pIUIElement);
		void CloseWindow(IUIAutomationElement* pIUIElement);
		void ClearWindows();
		void MouseAction(IUIAutomationElement* pTargetElement, MOUSECLICKTYPE type);
		wchar_t* GetElementName(IUIAutomationElement* pIUIElement);
		UIA_HWND GetNativeWindowHandle(IUIAutomationElement* pIUIElement);
		
	};
}
#endif // !AUIELEMENTLIB_H