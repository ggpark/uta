/*
UI Element�� ��ȯ �� ���� CAUIElement ��ü�� ���� �����͸� ���ڷ� �Ͽ� ��ȯ �ϵ��� �մϴ�.
CAUIElement Ŭ������ IUIAutomationElement �������̽��� ���� �����͸� ��������� �����ϴ�.

*/

#include "AUIAutomation.h"
#include "AUIElementLib.h"
#include "AUIElement.h"
#include "AUIWindow.h"
#include "AUIControl.h"
#include "CommonType.h"
#include "AUIError.h"

using namespace Automation;

IUIAutomation* g_pAutomation; // ���� ���� ���� 

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
�̸��� szFindText�� �ش��ϴ� ������Ʈ�� �����츦 TOP���� �ø���.
*/
void CAUIAutomation::TargetWindowByName(PWCHAR szProcessName, PWCHAR szFindText){

	//������Ʈ�� ã�´�.
	IUIAutomationElement* pTargetElement = NULL;
	pTargetElement = elementLib->FindElementByName(szProcessName, szFindText);
	
	UIA_HWND hHwnd;
	IUIAutomationElement* pParent = NULL;
	hHwnd = elementLib->GetNativeWindowHandle(pTargetElement);
	pTargetElement->get_CurrentNativeWindowHandle(&hHwnd);
	while (hHwnd == NULL){
		//nativewindowhandle�� ���� ��쿡 �θ� �����Ͽ� handle�� ���Ѵ�.
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
Automation ID�� szAuotmationID�� �ش��ϴ� ������Ʈ�� �����츦 TOP���� �ø���.
*/
void CAUIAutomation::TargetWindowByID(PWCHAR szProcessName, PWCHAR szAuotmationID){

	//������Ʈ�� ã�´�.
	IUIAutomationElement* pTargetElement = NULL;
	pTargetElement = elementLib->FindElementByID(szProcessName, szAuotmationID);


	UIA_HWND hHwnd;
	IUIAutomationElement* pParent = NULL;
	hHwnd = elementLib->GetNativeWindowHandle(pTargetElement);
	while (hHwnd == NULL){
		Log(L"DEBUG", L"[TargetWindowByID] This Element don't hane NativeWindowHandle. ProcessName : %s, AutomationID : %s", szProcessName, szAuotmationID);
		//nativewindowhandle�� ���� ��쿡 �θ� �����Ͽ� handle�� ���Ѵ�.
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
	//������Ʈ�� ã�´�.
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText, controlTypeID);
	
	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseLeftByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}
	
	//���콺 Ŭ��
	elementLib->MouseAction(pFoundElement,MOUSELEFTCLICK);
}

void CAUIAutomation::MouseRightByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID){
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText, controlTypeID);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseRightByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}
	//���콺 Ŭ��
	elementLib->MouseAction(pFoundElement, MOUSERIGHTCLICK);
}

void CAUIAutomation::MouseLeftDoubleByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID){
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText, controlTypeID);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseLeftDoubleByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//���콺 Ŭ��	
	elementLib->MouseAction(pFoundElement, MOUSELEFTDOUBLECLICK);
}

/*
AutomationID �Ӽ����� �̿��Ͽ� �ش� ������Ʈ�� ���콺 ���� Ŭ�� �Ѵ�.
*/
void CAUIAutomation::MouseLeftByID(PWCHAR szProcessName, PWCHAR szAutomationID,int controlTypeID){
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID, controlTypeID);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseLeftByID] Element Is Not Top Window");
		TargetWindowByID(szProcessName, szAutomationID);
	}

	elementLib->MouseAction(pFoundElement, MOUSELEFTCLICK);
}

/*
AutomationID �Ӽ����� �̿��Ͽ� �ش� ������Ʈ�� ���콺 ������ Ŭ�� �Ѵ�.
*/
void CAUIAutomation::MouseRightByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID){
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID, controlTypeID);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseRightByID] Element Is Not Top Window");
		TargetWindowByID(szProcessName, szAutomationID);
	}
	elementLib->MouseAction(pFoundElement, MOUSERIGHTCLICK);
}

/*
AutomationID �Ӽ����� �̿��Ͽ� �ش� ������Ʈ�� ���콺 ���� ����Ŭ�� �Ѵ�.
*/
void CAUIAutomation::MouseLeftDoubleByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID){
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID, controlTypeID);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseLeftDoubleByID] Element Is Not Top Window");
		TargetWindowByID(szProcessName, szAutomationID);
	}
	elementLib->MouseAction(pFoundElement, MOUSELEFTDOUBLECLICK);
}

/*
TOP Level ������ �߿� �̸� �Ӽ��� szFindText�� ��Ī�Ǵ� �������� ���� ���θ� Ȯ��
TOP Level �����쿡 �ش��ϴ� szFindText�� ��� �ؾ� �մϴ�.
*/
void CAUIAutomation::VerifyTopWindowWait(PWCHAR szProcessName, PWCHAR szFindText, int waitTime){
	IUIAutomationElement* pFoundElement = NULL;
	int delay = 500; //0.5�� ������
	int verifyCount = (waitTime*1000) / 500; // delay�ϸ鼭 waitTime��ŭ üũ �Ѵ�.

	while (verifyCount>0){
		pFoundElement = elementLib->GetTopElement(szProcessName, szFindText);
		if (pFoundElement != NULL){
			//szFindText�� �ش��ϴ� ������Ʈ�� ���������� ����
			return;
		}
		Sleep(delay);
		verifyCount--;
	}
	throw exception("VerfiWindow Fail");
}

/*
��Ʈ�� �����ϰ� ���� ��� ������Ʈ �߿� szFindText�� ��Ī�Ǵ� ������Ʈ�� ���� ���θ� Ȯ��
waitTime : 1 (1��)
*/
void CAUIAutomation::VerifyTextWait(PWCHAR szProcessName, PWCHAR szFindText, int waitTime)
{
	IUIAutomationElement* pProcessTopElement = NULL;
	IUIAutomationElement* pFoundElement = NULL;

	int delay = 500; //0.5�� ������
	int verifyCount = (waitTime * 1000) / 500; // delay�ϸ鼭 waitTime��ŭ üũ �Ѵ�.

	while (verifyCount>0){
		pProcessTopElement = elementLib->GetTopElement(szProcessName);
		if (pProcessTopElement == NULL){
			//ž���� �����찡 ������
			verifyCount--;
			continue;
		}
		pFoundElement = elementLib->FindElementByNameFromElement(pProcessTopElement, szFindText, NULL);
		if (pFoundElement != NULL){
			//szFindText�� �ش��ϴ� ������Ʈ�� ������ ����
			return;
		}
		Sleep(delay);
		verifyCount--;
	}
	throw exception("VerfiWindow Fail");
}

/*
szProcessName, szFindText�� AND �������� �Ͽ� ��ġ�ϴ� �����츦 �����մϴ�.
LocalizedControlType = "â"�� ��� ���� ���� 
*/
void CAUIAutomation::CloseWindow(PWCHAR szProcessName, PWCHAR szFindText)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);
	elementLib->CloseWindow(pFoundElement);
}

/*
��� �����츦 �����մϴ�.
*/
void CAUIAutomation::ClearWindows()
{
	elementLib->ClearWindows();
}

/*
szProcessName, szFindText�� AND �������� �Ͽ� ��ġ�ϴ� ������Ʈ�� �������� iPreviousStep��° ���� ������Ʈ�� ���콺 ���� Ŭ��
*/
void CAUIAutomation::MouseLeftPreviousByName(PWCHAR szProcessName, PWCHAR szFindText, int iPreviousStep=1)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseLeftPreviousByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//n��° ���� ��带 ���� ���콺 Ŭ�� �Ѵ�.
	IUIAutomationElement* pPreviousElement = NULL;
	pPreviousElement = elementLib->GetPreviousElement(pFoundElement,iPreviousStep);
	if (pPreviousElement == NULL){
		Log(L"ERROR", L"[MouseLeftPreviousByName] pPreviousElement Not Founded. ProcessName : %s, FindText : %s, previousCount : %d",szProcessName,szFindText,iPreviousStep);
		throw exception("[MouseLeftPreviousByName] pPreviousElement Not Founded");
	}
	elementLib->MouseAction(pPreviousElement, MOUSELEFTCLICK);
}

/*
szProcessName, szFindText�� AND �������� �Ͽ� ��ġ�ϴ� ������Ʈ�� �������� iNextStep��° ���� ������Ʈ�� ���콺 ���� Ŭ��
*/
void CAUIAutomation::MouseLeftNextByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseLeftNextByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//n��° ���� ��带 ���� ���콺 ���� ���� Ŭ�� �Ѵ�.
	IUIAutomationElement* pNextElement = NULL;
	pNextElement = elementLib->GetNextElement(pFoundElement, iNextStep);
	if (pNextElement == NULL){
		Log(L"ERROR", L"[MouseLeftNextByName] pNextElement Not Founded. ProcessName : %s, FindText : %s, nextCount : %d", szProcessName, szFindText, iNextStep);
		throw exception("[MouseLeftNextByName] pNextElement Not Founded");
	}
	elementLib->MouseAction(pNextElement, MOUSELEFTCLICK);
}

/*
szProcessName, szFindText�� AND �������� �Ͽ� ��ġ�ϴ� ������Ʈ�� �������� iPreviousStep��° ���� ������Ʈ�� ���콺 ���� ���� Ŭ��
*/
void CAUIAutomation::MouseLeftDoublePreviousByName(PWCHAR szProcessName, PWCHAR szFindText, int iPreviousStep)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseLeftDoublePreviousByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//n��° ���� ��带 ���� ���콺 ���� ���� Ŭ�� �Ѵ�.
	IUIAutomationElement* pPreviousElement = NULL;
	pPreviousElement = elementLib->GetPreviousElement(pFoundElement, iPreviousStep);
	if (pPreviousElement == NULL){
		Log(L"ERROR", L"[MouseLeftDoublePreviousByName] pPreviousElement Not Founded. ProcessName : %s, FindText : %s, previousCount : %d", szProcessName, szFindText, iPreviousStep);
		throw exception("[MouseLeftDoublePreviousByName] pPreviousElement Not Founded");
	}
	elementLib->MouseAction(pPreviousElement, MOUSELEFTDOUBLECLICK);
}

/*
szProcessName, szFindText�� AND �������� �Ͽ� ��ġ�ϴ� ������Ʈ�� �������� iNextStep��° ���� ������Ʈ�� ���콺 ���� ���� Ŭ��
*/
void CAUIAutomation::MouseLeftDoubleNextByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//�����츦 TOP���� �ø���.
	if (!(elementLib->IsBelongToForegroundWindow(pFoundElement))){
		//pTargetElement�� ForegroundWindow�� ���ϴ��� ������ �ٽ� TOP���� �ø���.
		Log(L"DEBUG", L"[MouseLeftDoubleNextByName] Element Is Not Top Window");
		TargetWindowByName(szProcessName, szFindText);
	}

	//n��° ���� ��带 ���� ���콺 ���� ���� Ŭ�� �Ѵ�.
	IUIAutomationElement* pNextElement = NULL;
	pNextElement = elementLib->GetNextElement(pFoundElement, iNextStep);
	if (pNextElement == NULL){
		Log(L"ERROR", L"[MouseLeftDoubleNextByName] pNextElement Not Founded. ProcessName : %s, FindText : %s, nextCount : %d", szProcessName, szFindText, iNextStep);
		throw exception("[MouseLeftDoubleNextByName] pNextElement Not Founded");
	}
	elementLib->MouseAction(pNextElement, MOUSELEFTDOUBLECLICK);
}

/*
szProcessName,szAutomationID�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ� Invoke ȣ��
InvokePattern�� �ƴϸ� �ƹ����� ����
*/
void CAUIAutomation::InvokeByID(PWCHAR szProcessName, PWCHAR szAutomationID)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//InvokePattern ��Ʈ�� ������ ������Ʈ���� ������
	Throw_if_fail(elementControl->Invoke(pFoundElement));
	return;
}

/*
szProcessName,szFindText�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ� Invoke ȣ��
InvokePattern�� �ƴϸ� �ƹ����� ����
*/
void CAUIAutomation::InvokeByName(PWCHAR szProcessName, PWCHAR szFindText)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//InvokePattern ��Ʈ�� ������ ������Ʈ���� ������
	Throw_if_fail(elementControl->Invoke(pFoundElement));
	return;
}

/*
szProcessName,szAutomationID�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ� Select ȣ��
SelectPattern�� �ƴϸ� �ƹ����� ����
*/
void CAUIAutomation::SelectByID(PWCHAR szProcessName, PWCHAR szAutomationID)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//SelectItemPattern�� ��� Select �Ѵ�. SelectItemPattern�� �ƴϸ� �ƹ��� ������ ���� �ʴ´�.
	Throw_if_fail(elementControl->Select(pFoundElement));
	return;
}

/*
szProcessName,szFindText�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ� Select ȣ��
SelectPattern�� �ƴϸ� �ƹ����� ����
*/
void CAUIAutomation::SelectByName(PWCHAR szProcessName, PWCHAR szFindText)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);
	
	//SelectItemPattern�� ��� Select �Ѵ�. SelectItemPattern�� �ƴϸ� �ƹ��� ������ ���� �ʴ´�.
	Throw_if_fail(elementControl->Select(pFoundElement));
}

/*
szProcessName,szAutomationID�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ� Toggle ȣ��
TogglePattern�� �ƴϸ� �ƹ����� ����
*/
void CAUIAutomation::ToggleByID(PWCHAR szProcessName, PWCHAR szAutomationID)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//TogglePattern�� ��� Toggle �Ѵ�.
	Throw_if_fail(elementControl->Toggle(pFoundElement));
}

/*
szProcessName,szFindText�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ� Toggle ȣ��
TogglePattern�� �ƴϸ� �ƹ����� ����
*/
void CAUIAutomation::ToggleByName(PWCHAR szProcessName, PWCHAR szFindText)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//TogglePattern�� ��� Toggle �Ѵ�.
	Throw_if_fail(elementControl->Toggle(pFoundElement));
}

/*
szProcessName,szFindText�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ�
SelectItem�� ���� �� �Ǿ� �ִ��� Ȯ��
*/
void CAUIAutomation::VerifySelectItem(PWCHAR szProcessName, PWCHAR szFindText)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//�ش� ������Ʈ�� SelectItemPattern�� ��� selected �������� Ȯ�� �Ѵ�.
	if (!elementControl->IsSelected(pFoundElement)){
		Log(L"ERROR", L"[VerifySelectItem] Element Not Selected or Not SelectItemPattern. ElementName : %s",szFindText);
		throw exception("[VerifySelectItem] Element Not Selected or Not SelectItemPattern");
	}
	Log(L"DEBUG", L"[VerifySelectItem] Element Selected. ElementName : %s", szFindText);
}

/*
szProcessName,szFindText�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ�
SelectItem�� ���õǾ� �ִ��� Ȯ��
*/
void CAUIAutomation::VerifyUnSelectItem(PWCHAR szProcessName, PWCHAR szFindText)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByName(szProcessName, szFindText);

	//�ش� ������Ʈ�� SelectItemPattern�� ��� unselected �������� Ȯ�� �Ѵ�.
	if (elementControl->IsSelected(pFoundElement)){
		Log(L"ERROR", L"[VerifyUnSelectItem] Element Selected or Not SelectItemPattern. ElementName : %s", szFindText);
		throw exception("[VerifyUnSelectItem] Element Selected or Not SelectItemPattern");
	}
	Log(L"DEBUG", L"[VerifyUnSelectItem] Element UnSelected. ElementName : %s", szFindText);
}

/*
szProcessName,szFindText�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ�
ToggleState�� ����Ͽ� CheckBox�� Checked�Ǿ� �ִ��� Ȯ��
*/
void CAUIAutomation::VerifyCheckBoxON(PWCHAR szProcessName, PWCHAR szFindText)
{
	//������Ʈ�� ã�´�.
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
szProcessName,szFindText�� AND�������� �Ͽ� ��Ī �Ǵ� ������Ʈ�� ���ؼ�
ToggleState�� ����Ͽ� CheckBox�� UnChecked�Ǿ� �ִ��� Ȯ��
*/
void CAUIAutomation::VerifyCheckBoxOFF(PWCHAR szProcessName, PWCHAR szFindText)
{
	//������Ʈ�� ã�´�.
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
���μ��� �̸��� automationID�� AND�������� �Ͽ� ��Ī�Ǵ� ������Ʈ�� NAME�� ��ȯ
*/
PWCHAR CAUIAutomation::GetTextById(PWCHAR szProcessName, PWCHAR szAutomationID)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//������Ʈ�� NAME�Ӽ� ���� ���Ѵ�.
	PWCHAR szElementName = elementLib->GetElementName(pFoundElement);
	return szElementName;
}

/*
���μ��� �̸��� automationID�� AND�������� �Ͽ� ��Ī�Ǵ� ������Ʈ�� TEXT�� ���� �Ѵ�.
EditControlTypd�� ��쿡�� ���� �ϴ�.
*/
void CAUIAutomation::SetTextByID(PWCHAR szProcessName, PWCHAR szAutomationID, PWCHAR szText)
{
	//������Ʈ�� ã�´�.
	IUIAutomationElement* pFoundElement = NULL;
	pFoundElement = elementLib->FindElementByID(szProcessName, szAutomationID);

	//�ش� ������Ʈ�� EditControlType�� ��� �Է¹��� TEXT�� SET �Ѵ�.
	//ValuePattern ��Ʈ��? 
	Throw_if_fail(elementControl->SetValue(pFoundElement, szText));
}