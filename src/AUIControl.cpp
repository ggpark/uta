
#include "AUIControl.h"
using namespace Automation;



CAUIControl::CAUIControl(){}

CAUIControl::~CAUIControl(){}


HRESULT CAUIControl::Invoke(IUIAutomationElement* pElement){

	IUIAutomationInvokePattern* pInvokeVal = NULL;
	HRESULT hr =
		pElement->GetCurrentPatternAs(UIA_InvokePatternId,
		__uuidof(IUIAutomationInvokePattern),
		(void **)&pInvokeVal);

	if (FAILED(hr) || pInvokeVal == NULL)
	{
		return false;
	}
	pInvokeVal->Invoke();
	Log(L"DEBUG", L"Invoke Success");
	pInvokeVal->Release();
	return true;
}


HRESULT CAUIControl::Select(IUIAutomationElement* pElement){
	IUIAutomationSelectionItemPattern* pSelectionItemVal = NULL;
	HRESULT hr =
		pElement->GetCurrentPatternAs(UIA_SelectionItemPatternId,
		__uuidof(IUIAutomationSelectionItemPattern),
		(void **)&pSelectionItemVal);

	if (FAILED(hr) || pSelectionItemVal == NULL)
	{
		return hr;
	}
	pSelectionItemVal->Select();
	Log(L"DEBUG", L"Select Success");
	pSelectionItemVal->Release();
	return hr;
}


bool CAUIControl::IsSelected(IUIAutomationElement* pElement)
{
	IUIAutomationSelectionItemPattern* pSelectionItemVal = NULL;
	BOOL retVal;
	HRESULT hr =
		pElement->GetCurrentPatternAs(UIA_SelectionItemPatternId,
		__uuidof(IUIAutomationSelectionItemPattern),
		(void **)&pSelectionItemVal);

	if (FAILED(hr) || pSelectionItemVal == NULL)
	{
		return false;
	}
	pSelectionItemVal->get_CurrentIsSelected(&retVal);
	pSelectionItemVal->Release();

	if (retVal == 0){
		//선택되지 않은 상태
		return false;
	}
	return true;
}

HRESULT CAUIControl::Toggle(IUIAutomationElement* pElement)
{
	IUIAutomationTogglePattern* pToggleVal = NULL;
	HRESULT hr =
		pElement->GetCurrentPatternAs(UIA_TogglePatternId,
		__uuidof(IUIAutomationTogglePattern),
		(void **)&pToggleVal);

	if (FAILED(hr) || pToggleVal == NULL)
	{
		return hr;
	}
	pToggleVal->Toggle();
	Log(L"DEBUG", L"pToggle Success");
	pToggleVal->Release();
	return hr;
}

/*
엘리먼트의 ToggleState
*/
ToggleState CAUIControl::GetToggleState(IUIAutomationElement* pElement)
{
	IUIAutomationTogglePattern* pToggleVal = NULL;
	HRESULT hr =
		pElement->GetCurrentPatternAs(UIA_TogglePatternId,
		__uuidof(IUIAutomationTogglePattern),
		(void **)&pToggleVal);

	if (FAILED(hr) || pToggleVal == NULL)
	{
		return ToggleState_Indeterminate;
	}
	ToggleState retVal;
	HRESULT hRet = pToggleVal->get_CurrentToggleState(&retVal);
	return retVal;
}


/*
ValuePattern에 대해 SetValue를 한다.
*/
HRESULT CAUIControl::SetValue(IUIAutomationElement* pElement, PWCHAR szText)
{
	IUIAutomationValuePattern* pValueVal = NULL;
	HRESULT hr = pElement->GetCurrentPatternAs(UIA_ValuePatternId,
		__uuidof(IUIAutomationValuePattern),
		(void **)&pValueVal);

	if (FAILED(hr) || pValueVal == NULL)
	{
		return hr;
	}
	
	pValueVal->SetValue((BSTR)szText);
	return hr;
}