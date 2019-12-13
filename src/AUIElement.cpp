#include "AUIElement.h"

using namespace Automation;


CAUIElement::CAUIElement(){ }
CAUIElement::~CAUIElement(){
	// pUIElement�� pControlInterface�� release �մϴ�.;
	delete this->pIUIElement;
}

bool CAUIElement::SetElement(IUIAutomationElement* element){
	// IUIAutomationElement ��ü�� ���� �ϰ� 
	// �ش� ������Ʈ ��ü�� �Ӽ������� ���� �մϴ�.
	if (element == NULL){
		//element�� �������� �ʴ� �ٸ�.
		Log(L"DEBUG", L"[SetElement] Failed");
		return false;
	}
	this->pIUIElement = element;
	this->pIUIElement->get_CurrentNativeWindowHandle((&(this->hNativeWindowHandle)));
	this->pIUIElement->get_CurrentAutomationId(&(this->szAutomationID));
	this->pIUIElement->get_CurrentName(&(this->szName));
	this->pIUIElement->get_CurrentControlType(&(this->controlTypeId));
	this->pIUIElement->get_CurrentLocalizedControlType(&(this->localizedControlType));
	return true;
}


/*
bool GetElementControlPattern(IUIAutomationElement* pElement, IUnknown* pControlInterface){
//  this -> patternId�� �����մϴ�.

//const long UIA_InvokePatternId = 10000;
IUIAutomationInvokePattern* pInvokePattern = NULL;
HRESULT hr = pElement->GetCurrentPattern(UIA_InvokePatternId, &pControlInterface);
if (!hr){
// success get control pattern
hr = pControlInterface->QueryInterface(IID_IUIAutomationInvokePattern, (void**)&pInvokePattern);
if (!hr){
//success query interface
return true;
}
}

//const long UIA_SelectionPatternId = 10001;
IUIAutomationSelectionPattern* pSelectionPattern = NULL;
HRESULT hr = pElement->GetCurrentPattern(UIA_SelectionPatternId, &pControlInterface);
if (!hr){
hr = pControlInterface->QueryInterface(IID_IUIAutomationSelectionPattern, (void**)&pSelectionPattern);
if (!hr){
return true;
}
}


//const long UIA_ValuePatternId = 10002;
IUIAutomationSelectionPattern* pValuePattern = NULL;
HRESULT hr = pElement->GetCurrentPattern(UIA_ValuePatternId, &pControlInterface);
if (!hr){
hr = pControlInterface->QueryInterface(IID_IUIAutomationValuePattern, (void**)&pValuePattern);
if (!hr){
return true;
}
}


const long UIA_RangeValuePatternId = 10003;
IUIAutomationSelectionPattern* pSelectionPattern = NULL;
HRESULT hr = pElement->GetCurrentPattern(UIA_SelectionPatternId, &pControlInterface);
if (!hr){
hr = pControlInterface->QueryInterface(IID_IUIAutomationSelectionPattern, (void**)&pSelectionPattern);
if (!hr){
return true;
}
}


const long UIA_ScrollPatternId = 10004;
IUIAutomationSelectionPattern* pSelectionPattern = NULL;
HRESULT hr = pElement->GetCurrentPattern(UIA_SelectionPatternId, &pControlInterface);
if (!hr){
hr = pControlInterface->QueryInterface(IID_IUIAutomationSelectionPattern, (void**)&pSelectionPattern);
if (!hr){
return true;
}
}


const long UIA_ExpandCollapsePatternId = 10005;

const long UIA_GridPatternId = 10006;

const long UIA_GridItemPatternId = 10007;

const long UIA_MultipleViewPatternId = 10008;

const long UIA_WindowPatternId = 10009;

const long UIA_SelectionItemPatternId = 10010;

const long UIA_DockPatternId = 10011;

const long UIA_TablePatternId = 10012;

const long UIA_TableItemPatternId = 10013;

const long UIA_TextPatternId = 10014;

const long UIA_TogglePatternId = 10015;

const long UIA_TransformPatternId = 10016;

const long UIA_ScrollItemPatternId = 10017;

const long UIA_LegacyIAccessiblePatternId = 10018;

const long UIA_ItemContainerPatternId = 10019;

const long UIA_VirtualizedItemPatternId = 10020;

const long UIA_SynchronizedInputPatternId = 10021;

//���� ���� �°Ÿ� �ش� ������Ʈ�� ��Ʈ�� ������ ���� ��.
return false;


}
*/
