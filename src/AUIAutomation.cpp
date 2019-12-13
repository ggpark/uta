/*
UI Element를 반환 할 때는 CAUIElement 객체에 대한 포인터를 인자로 하여 반환 하도록 합니다.
CAUIElement 클래스는 IUIAutomationElement 인터페이스에 대한 포인터를 멤버변수로 갖습니다.

*/

#include "AUIAutomation.h"
#include "AUIElementLib.h"
#include "AUIElement.h"
#include "AUIWindow.h"
#include "AUIControl.h"
#include "CommonType.h"
#include "AUIError.h"

using namespace Automation;

IUIAutomation* g_pAutomation; // 전역 변수 정의 

HRESULT CAUIAutomation::InitializeUIAutomation(IUIAutomation **ppAutomation){
	return CoCreateInstance(CLSID_CUIAutomation, NULL,
		CLSCTX_INPROC_SERVER, IID_IUIAutomation, reinterpret_cast<void**>(ppAutomation));
}

CAUIAutomation::CAUIAutomation() : CAUILog(){
	HRESULT hr = CoInitialize(NULL);
	this->InitializeUIAutomation(&g_pAutomation);
	elementLib = new CAUIElementLib();
	abstractElement = new CAUIElement();
	windowControl = new CAUIWindow();
	elementControl = new CAUIControl();
}


CAUIAutomation::~CAUIAutomation(){
	Log(L"DEBUG", L"CAUIAutomation Delete");
}

/*
이름이 szFindText에 해당하는 엘리먼트의 윈도우를 TOP으로 올린다.
*/
void CAUIAutomation::TargetWindowByName(PWCHAR szProcessName, PWCHAR szFindText){

	//엘리먼트를 찾는다.
	IUIAutomationElement* pTargetElement = NULL;
	pTargetElement = elementLib->FindElementByName(szProcessName, szFindText);
	
	UIA_HWND hHwnd;
	IUIAutomationElement* pParent = NULL;
	hHwnd = elementLib->GetNativeWindowHandle(pTargetElement);
	pTargetElement->get_CurrentNativeWindowHandle(&hHwnd);
	while (hHwnd == NULL){
		//nativewindowhandle이 없는 경우에 부모를 추적하여 handle을 구한다.
		Log(L"DEBUG", L"[TargetWindowByName] This Element don't hane NativeWindowHandle. ProcessName : %s, ElementName : %s", szProcessName, szFindText);
		pParent = elementLib->GetParentElement(pTargetElement);
		if (pParent == NULL){
			break;
		}
		pParent->get_CurrentNativeWindowHandle(&hHwnd);
		pTargetElement = pParent;
	}
	Log(L"DEBUG", L"[TargetWindowByName] Call SetTopWindow. ProcessName : %s, ElementName : %s", szProcessName, szFindText);
	windowControl->SetTopWindow(hHwnd);

	return ;
}

/*
Automation ID가 szAuotmationID에 해당하는 엘리먼트의 윈도우를 TOP으로 올린다.
*/
void CAUIAutomation::TargetWindowByID(PWCHAR szProcessName, PWCHAR szAuotmationID){

	//엘리먼트를 찾는다.
	IUIAutomationElement* pTargetElement = NULL;
	pTargetElement = elementLib->FindElementByID(szProcessName, szAuotmationID);


	UIA_HWND hHwnd;
	IUIAutomationElement* pParent = NULL;
	hHwnd = elementLib->GetNativeWindowHandle(pTargetElement);
	while (hHwnd == NULL){
		Log(L"DEBUG", L"[TargetWindowByID] This Element don't hane NativeWindowHandle. ProcessName : %s, AutomationID : %s", szProcessName, szAuotmationID);
		//nativewindowhandle이 없는 경우에 부모를 추적하여 handle을 구한다.
		pParent = elementLib->GetParentElement(pTargetElement);
		if (pParent == NULL){
			break;
		}
		pParent->get_CurrentNativeWindowHandle(&hHwnd);
		pTargetElement = pParent;
	}
	Log(L"DEBUG", L"[TargetWindowByID] Call SetTopWindow. ProcessName : %s, AutomationID : %s", szProcessName, szAuotmationID);
	windowControl->SetTopWindow(hHwnd);
	return;
}


void CAUIAutomation::MouseLeftByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID){
	IUIAutomationElement* pFoundElement = NULL;
	//엘리먼트를 찾는다.
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText, controlTypeID);
	
	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseLeftByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}
	
	//마우스 클릭
	elementLib->MouseAction(pFoundElement,MOUSELEFTCLICK);
}

void CAUIAutomation::MouseRightByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID){
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText, controlTypeID);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseRightByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}
	//마우스 클릭
	elementLib->MouseAction(pFoundElement, MOUSERIGHTCLICK);
}

void CAUIAutomation::MouseLeftDoubleByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID){
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText, controlTypeID);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseLeftDoubleByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//마우스 클릭	
	elementLib->MouseAction(pFoundElement, MOUSELEFTDOUBLECLICK);
}

/*
AutomationID 속성값을 이용하여 해당 엘리먼트를 마우스 왼쪽 클릭 한다.
*/
void CAUIAutomation::MouseLeftByID(PWCHAR szProcessName, PWCHAR szAutomationID,int controlTypeID){
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID, controlTypeID);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseLeftByID] Element Is Not Top Window");
		TargetWindowByID(szProcessName, szAutomationID);
	}

	elementLib->MouseAction(pFoundElement, MOUSELEFTCLICK);
}

/*
AutomationID 속성값을 이용하여 해당 엘리먼트를 마우스 오른쪽 클릭 한다.
*/
void CAUIAutomation::MouseRightByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID){
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID, controlTypeID);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseRightByID] Element Is Not Top Window");
		TargetWindowByID(szProcessName, szAutomationID);
	}
	elementLib->MouseAction(pFoundElement, MOUSERIGHTCLICK);
}

/*
AutomationID 속성값을 이용하여 해당 엘리먼트를 마우스 왼쪽 더블클릭 한다.
*/
void CAUIAutomation::MouseLeftDoubleByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID){
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID, controlTypeID);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseLeftDoubleByID] Element Is Not Top Window");
		TargetWindowByID(szProcessName, szAutomationID);
	}
	elementLib->MouseAction(pFoundElement, MOUSELEFTDOUBLECLICK);
}

/*
TOP Level 윈도우 중에 이름 속성이 szFindText와 매칭되는 윈도우의 존재 여부를 확인
TOP Level 윈도우에 해당하는 szFindText를 사용 해야 합니다.
*/
void CAUIAutomation::VerifyTopWindowWait(PWCHAR szProcessName, PWCHAR szFindText, int waitTime){
	IUIAutomationElement* pFoundElement = NULL;
	int delay = 500; //0.5초 딜레이
	int verifyCount = (waitTime*1000) / 500; // delay하면서 waitTime만큼 체크 한다.

	while (verifyCount>0){
		pFoundElement = elementLib->GetTopElement(szProcessName, szFindText);
		if (pFoundElement != NULL){
			//szFindText에 해당하는 엘리먼트가 없어졌으면 종료
			return;
		}
		Sleep(delay);
		verifyCount--;
	}
	throw exception("VerfiWindow Fail");
}

/*
루트를 제외하고 현재 모든 엘리먼트 중에 szFindText와 매칭되는 엘리먼트의 존재 여부를 확인
waitTime : 1 (1초)
*/
void CAUIAutomation::VerifyTextWait(PWCHAR szProcessName, PWCHAR szFindText, int waitTime)
{
	IUIAutomationElement* pProcessTopElement = NULL;
	IUIAutomationElement* pFoundElement = NULL;

	int delay = 500; //0.5초 딜레이
	int verifyCount = (waitTime * 1000) / 500; // delay하면서 waitTime만큼 체크 한다.

	while (verifyCount>0){
		pProcessTopElement = elementLib->GetTopElement(szProcessName);
		if (pProcessTopElement == NULL){
			//탑레벨 윈도우가 없으면
			verifyCount--;
			continue;
		}
		pFoundElement = elementLib->FindElementByNameFromElement(pProcessTopElement, szFindText, NULL);
		if (pFoundElement != NULL){
			//szFindText에 해당하는 엘리먼트가 있으면 종료
			return;
		}
		Sleep(delay);
		verifyCount--;
	}
	throw exception("VerfiWindow Fail");
}

/*
szProcessName, szFindText를 AND 조건으로 하여 일치하는 윈도우를 종료합니다.
LocalizedControlType = "창"인 경우 정상 동작 
*/
void CAUIAutomation::CloseWindow(PWCHAR szProcessName, PWCHAR szFindText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);
	elementLib->CloseWindow(pFoundElement);
}

/*
모든 윈도우를 종료합니다.
*/
void CAUIAutomation::ClearWindows()
{
	elementLib->ClearWindows();
}

/*
szProcessName, szFindText를 AND 조건으로 하여 일치하는 엘리먼트를 기준으로 iPreviousStep번째 이전 엘리먼트를 마우스 왼쪽 클릭
*/
void CAUIAutomation::MouseLeftPreviousByName(PWCHAR szProcessName, PWCHAR szFindText, int iPreviousStep=1)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseLeftPreviousByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//n번째 이전 노드를 구해 마우스 클릭 한다.
	IUIAutomationElement* pPreviousElement = NULL;
	pPreviousElement = elementLib->GetPreviousElement(pFoundElement,iPreviousStep);
	if (pPreviousElement == NULL){
		Log(L"ERROR", L"[MouseLeftPreviousByName] pPreviousElement Not Founded. ProcessName : %s, FindText : %s, previousCount : %d",szProcessName,szFindText,iPreviousStep);
		throw exception("[MouseLeftPreviousByName] pPreviousElement Not Founded");
	}
	elementLib->MouseAction(pPreviousElement, MOUSELEFTCLICK);
}

/*
szProcessName, szFindText를 AND 조건으로 하여 일치하는 엘리먼트를 기준으로 iNextStep번째 다음 엘리먼트를 마우스 왼쪽 클릭
*/
void CAUIAutomation::MouseLeftNextByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseLeftNextByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//n번째 다음 노드를 구해 마우스 왼쪽 더블 클릭 한다.
	IUIAutomationElement* pNextElement = NULL;
	pNextElement = elementLib->GetNextElement(pFoundElement, iNextStep);
	if (pNextElement == NULL){
		Log(L"ERROR", L"[MouseLeftNextByName] pNextElement Not Founded. ProcessName : %s, FindText : %s, nextCount : %d", szProcessName, szFindText, iNextStep);
		throw exception("[MouseLeftNextByName] pNextElement Not Founded");
	}
	elementLib->MouseAction(pNextElement, MOUSELEFTCLICK);
}

/*
szProcessName, szFindText를 AND 조건으로 하여 일치하는 엘리먼트를 기준으로 iPreviousStep번째 이전 엘리먼트를 마우스 왼쪽 더블 클릭
*/
void CAUIAutomation::MouseLeftDoublePreviousByName(PWCHAR szProcessName, PWCHAR szFindText, int iPreviousStep)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseLeftDoublePreviousByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//n번째 다음 노드를 구해 마우스 왼쪽 더블 클릭 한다.
	IUIAutomationElement* pPreviousElement = NULL;
	pPreviousElement = elementLib->GetPreviousElement(pFoundElement, iPreviousStep);
	if (pPreviousElement == NULL){
		Log(L"ERROR", L"[MouseLeftDoublePreviousByName] pPreviousElement Not Founded. ProcessName : %s, FindText : %s, previousCount : %d", szProcessName, szFindText, iPreviousStep);
		throw exception("[MouseLeftDoublePreviousByName] pPreviousElement Not Founded");
	}
	elementLib->MouseAction(pPreviousElement, MOUSELEFTDOUBLECLICK);
}

/*
szProcessName, szFindText를 AND 조건으로 하여 일치하는 엘리먼트를 기준으로 iNextStep번째 다음 엘리먼트를 마우스 왼쪽 더블 클릭
*/
void CAUIAutomation::MouseLeftDoubleNextByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//윈도우를 TOP으로 올린다.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement가 ForegroundWindow에 속하는지 않으면 다시 TOP으로 올린다.
		Log(L"DEBUG", L"[MouseLeftDoubleNextByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//n번째 다음 노드를 구해 마우스 왼쪽 더블 클릭 한다.
	IUIAutomationElement* pNextElement = NULL;
	pNextElement = elementLib->GetNextElement(pFoundElement, iNextStep);
	if (pNextElement == NULL){
		Log(L"ERROR", L"[MouseLeftDoubleNextByName] pNextElement Not Founded. ProcessName : %s, FindText : %s, nextCount : %d", szProcessName, szFindText, iNextStep);
		throw exception("[MouseLeftDoubleNextByName] pNextElement Not Founded");
	}
	elementLib->MouseAction(pNextElement, MOUSELEFTDOUBLECLICK);
}

/*
szProcessName,szAutomationID를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서 Invoke 호출
InvokePattern이 아니면 아무동작 안함
*/
void CAUIAutomation::InvokeByID(PWCHAR szProcessName, PWCHAR szAutomationID)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//InvokePattern 컨트롤 가능한 엘리먼트여야 동작함
	Throw_if_fail(elementControl->Invoke(pFoundElement));
	return;
}

/*
szProcessName,szFindText를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서 Invoke 호출
InvokePattern이 아니면 아무동작 안함
*/
void CAUIAutomation::InvokeByName(PWCHAR szProcessName, PWCHAR szFindText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//InvokePattern 컨트롤 가능한 엘리먼트여야 동작함
	Throw_if_fail(elementControl->Invoke(pFoundElement));
	return;
}

/*
szProcessName,szAutomationID를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서 Select 호출
SelectPattern이 아니면 아무동작 안함
*/
void CAUIAutomation::SelectByID(PWCHAR szProcessName, PWCHAR szAutomationID)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//SelectItemPattern인 경우 Select 한다. SelectItemPattern이 아니면 아무런 동작을 하지 않는다.
	Throw_if_fail(elementControl->Select(pFoundElement));
	return;
}

/*
szProcessName,szFindText를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서 Select 호출
SelectPattern이 아니면 아무동작 안함
*/
void CAUIAutomation::SelectByName(PWCHAR szProcessName, PWCHAR szFindText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);
	
	//SelectItemPattern인 경우 Select 한다. SelectItemPattern이 아니면 아무런 동작을 하지 않는다.
	Throw_if_fail(elementControl->Select(pFoundElement));
}

/*
szProcessName,szAutomationID를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서 Toggle 호출
TogglePattern이 아니면 아무동작 안함
*/
void CAUIAutomation::ToggleByID(PWCHAR szProcessName, PWCHAR szAutomationID)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//TogglePattern인 경우 Toggle 한다.
	Throw_if_fail(elementControl->Toggle(pFoundElement));
}

/*
szProcessName,szFindText를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서 Toggle 호출
TogglePattern이 아니면 아무동작 안함
*/
void CAUIAutomation::ToggleByName(PWCHAR szProcessName, PWCHAR szFindText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//TogglePattern인 경우 Toggle 한다.
	Throw_if_fail(elementControl->Toggle(pFoundElement));
}

/*
szProcessName,szFindText를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서
SelectItem이 선택 안 되어 있는지 확인
*/
void CAUIAutomation::VerifySelectItem(PWCHAR szProcessName, PWCHAR szFindText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//해당 엘리먼트가 SelectItemPattern인 경우 selected 상태인지 확인 한다.
	if (!elementControl->IsSelected(pFoundElement)){
		Log(L"ERROR", L"[VerifySelectItem] Element Not Selected or Not SelectItemPattern. ElementName : %s",szFindText);
		throw exception("[VerifySelectItem] Element Not Selected or Not SelectItemPattern");
	}
	Log(L"DEBUG", L"[VerifySelectItem] Element Selected. ElementName : %s", szFindText);
}

/*
szProcessName,szFindText를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서
SelectItem이 선택되어 있는지 확인
*/
void CAUIAutomation::VerifyUnSelectItem(PWCHAR szProcessName, PWCHAR szFindText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//해당 엘리먼트가 SelectItemPattern인 경우 unselected 상태인지 확인 한다.
	if (elementControl->IsSelected(pFoundElement)){
		Log(L"ERROR", L"[VerifyUnSelectItem] Element Selected or Not SelectItemPattern. ElementName : %s", szFindText);
		throw exception("[VerifyUnSelectItem] Element Selected or Not SelectItemPattern");
	}
	Log(L"DEBUG", L"[VerifyUnSelectItem] Element UnSelected. ElementName : %s", szFindText);
}

/*
szProcessName,szFindText를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서
ToggleState를 사용하여 CheckBox가 Checked되어 있는지 확인
*/
void CAUIAutomation::VerifyCheckBoxON(PWCHAR szProcessName, PWCHAR szFindText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	ToggleState state = elementControl->GetToggleState(pFoundElement);
	if (state == ToggleState_On){
		Log(L"DEBUG", L"[VerifyCheckBoxON] Verify Success. CheckBox is ON. ProcessName : %s, FindText : %s", szProcessName, szFindText);
	}
	else{
		Log(L"ERROR", L"[VerifyCheckBoxON] Verify Fail. CheckBox is OFF. ProcessName : %s, FindText : %s", szProcessName, szFindText);
		throw exception();
	}
}

/*
szProcessName,szFindText를 AND조건으로 하여 매칭 되는 엘리먼트에 대해서
ToggleState를 사용하여 CheckBox가 UnChecked되어 있는지 확인
*/
void CAUIAutomation::VerifyCheckBoxOFF(PWCHAR szProcessName, PWCHAR szFindText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	ToggleState state = elementControl->GetToggleState(pFoundElement);
	if (state == ToggleState_Off){
		Log(L"DEBUG", L"[VerifyCheckBoxOFF] Verify Success. CheckBox is OFF. ProcessName : %s, FindText : %s", szProcessName, szFindText);
	}
	else{
		Log(L"ERROR", L"[VerifyCheckBoxOFF] Verify Fail. CheckBox is ON. ProcessName : %s, FindText : %s", szProcessName, szFindText);
		throw exception();
	}
}

/*
프로세스 이름과 automationID를 AND조건으로 하여 매칭되는 엘리먼트의 NAME을 반환
*/
PWCHAR CAUIAutomation::GetTextById(PWCHAR szProcessName, PWCHAR szAutomationID)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//엘리먼트의 NAME속성 값을 구한다.
	PWCHAR szElementName = elementLib->GetElementName(pFoundElement);
	return szElementName;
}

/*
프로세스 이름과 automationID를 AND조건으로 하여 매칭되는 엘리먼트의 TEXT를 셋팅 한다.
EditControlTypd인 경우에만 가능 하다.
*/
void CAUIAutomation::SetTextByID(PWCHAR szProcessName, PWCHAR szAutomationID, PWCHAR szText)
{
	//엘리먼트를 찾는다.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//해당 엘리먼트가 EditControlType인 경우 입력받은 TEXT를 SET 한다.
	//ValuePattern 컨트롤? 
	Throw_if_fail(elementControl->SetValue(pFoundElement, szText));
}