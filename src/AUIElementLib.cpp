
#include "AUIAutomation.h"
#include "AUIElementLib.h"
#include "AUIElement.h"
#include "CommonLib.h"
#include "AUIMouse.h"
#include "AUIWindow.h"

using namespace Automation;

CAUIElementLib::CAUIElementLib(){
	//만약 
	HRESULT hr = g_pAutomation->GetRootElement(&pRootElement);
	if (FAILED(hr)){
		Log(L"DEBUG", L"[CAUIElementLib] GetRootElement Failed");
	}
}

CAUIElementLib::~CAUIElementLib(){
	this->pRootElement->Release();
}

/*
데스크탑 엘리먼트를 부모로 하는 탑 레벨 엘리먼트들을 모두 반환
*/
IUIAutomationElementArray* CAUIElementLib::GetTopElements(){
	IUIAutomationCondition* pTrueCondtion = NULL;
	IUIAutomationElementArray* pFoundElementArray = NULL;
	g_pAutomation->CreateTrueCondition(&pTrueCondtion);
	pFoundElementArray = this->FindAll(pRootElement, TreeScope_Children, pTrueCondtion);
	if (pFoundElementArray==NULL){
		Log(L"DEBUG", L"[CAUIElementLib::FindElementByName] FindAll Failed");
		throw;
	}
	//정리
	pTrueCondtion->Release();

	return pFoundElementArray;
}

/*
Root 엘리먼트 하위 엘리먼트중 프로세스이름에 해당하는 엘리먼트를 반환 
엘리먼트를 찾지 못하면 false를 반환
*/
IUIAutomationElement* CAUIElementLib::GetTopElement(PWCHAR szProcessName){
	// desktop window를 부모로 갖는 엘리먼트중 프로세스 이름이 일치하는 엘리먼트 반환
	// TODO : 
	IUIAutomationElement* pFoundElement = NULL;
	IUIAutomationCondition* pProcessCondtion = NULL;
	VARIANT var;
	DWORD processId;

	processId = commonLib::GetProcessIdFromName(szProcessName);
	commonLib::SetVariantAsI4(var, processId);
	g_pAutomation->CreatePropertyCondition(UIA_ProcessIdPropertyId, var, &pProcessCondtion);
	
	pFoundElement = FindFirst(this->pRootElement, TreeScope_Children, pProcessCondtion);
	if (pFoundElement==NULL){
		Log(L"DEBUG", L"[GetTopElement] FindFirst Failed");
		return pFoundElement;
	}
	//정리
	pProcessCondtion->Release();
	return pFoundElement;
}

IUIAutomationElement* CAUIElementLib::GetTopElement(PWCHAR szProcessName, PWCHAR szFindText){
	// desktop window를 부모로 갖는 엘리먼트중 프로세스 이름이 일치하고 컨트롤타입이 일치하는 엘리먼트 반환
	IUIAutomationCondition* pProcessCondtion = NULL;
	IUIAutomationCondition* pNameCondition = NULL;
	IUIAutomationCondition* pProcessAndNameCondition = NULL;
	IUIAutomationElement* pFoundElement = NULL;
	//프로세스 조건
	VARIANT varProcess;
	DWORD processId;
	processId = commonLib::GetProcessIdFromName(szProcessName);
	commonLib::SetVariantAsI4(varProcess, processId);
	g_pAutomation->CreatePropertyCondition(UIA_ProcessIdPropertyId, varProcess, &pProcessCondtion);

	//컨트롤 타입 조건
	VARIANT varName;
	commonLib::SetVariantAsBSTR(varName, szFindText);
	g_pAutomation->CreatePropertyCondition(UIA_NamePropertyId, varName, &pNameCondition);
	//SysFreeString(varName.bstrVal);

	//프로세스 + 컨트롤 타입 조건
	g_pAutomation->CreateAndCondition(pProcessCondtion, pNameCondition, &pProcessAndNameCondition);

	pFoundElement = this->FindFirst(this->pRootElement, TreeScope_Children, pProcessAndNameCondition);
	if (pFoundElement==NULL){
		Log(L"DEBUG", L"[GetTopElement] FindFirst Failed");
		return pFoundElement;
	}

	return pFoundElement;
}



/**
Automation ID 속성으로 엘리먼트를 찾아 CAUIElement 객체로 반환
*/
IUIAutomationElement* CAUIElementLib::FindElementByIDFromElement(IUIAutomationElement* pStartElement, PWCHAR szAutomationID, int controlTypeID){
	IUIAutomationElement* pFoundElement = NULL;
	IUIAutomationElementArray* pFoundedElements = NULL;
	IUIAutomationCondition* pControlCondition = NULL;
	int foundedElementCount;

	//localizedControlType 입력에 따른 조건 처리 
	if (controlTypeID == NULL){
		//NULL인 경우
		g_pAutomation->CreateTrueCondition(&pControlCondition);
	}
	else{
		VARIANT var;
		commonLib::SetVariantAsI4(var, controlTypeID);
		g_pAutomation->CreatePropertyCondition(UIA_ControlTypePropertyId, var, &pControlCondition);
	}


	pFoundedElements = this->FindAll(pStartElement, TreeScope_Subtree, pControlCondition);
	if (pFoundedElements==NULL){
		Log(L"DEBUG", L"[CAUIElementLib::FindElementByName] FindAll Failed");
		return NULL;
	}

	pFoundedElements->get_Length(&foundedElementCount);

	IUIAutomationElement* pSearchElement=NULL;
	BSTR id = NULL;
	for (int i = 0; i < foundedElementCount; i++){
		//찾아진 엘리먼트들을 탐색하면서 AutomationID가 동일한 엘리먼트를 찾는다.
		pFoundedElements->GetElement(i, &pSearchElement);
		pSearchElement->get_CurrentAutomationId(&id);
		if (id == NULL){
			//NULL이면 비교하지 않는다.
			continue;
		}
		if (_wcsicmp(szAutomationID, id) == 0){
			//automation id가 일치하는 엘리먼트를 찾은 경우
			pFoundElement = pSearchElement;
			return pFoundElement;
		}
	}
	//사용한 인터페이스 정리 
	pFoundedElements->Release();
	pControlCondition->Release();
	return pFoundElement;
}

/*
Name속성으로 엘리먼트를 찾아 CAUIElement 객체로 반환
*/
IUIAutomationElement* CAUIElementLib::FindElementByNameFromElement(IUIAutomationElement* pStartElement, PWCHAR szElementName, int controlTypeID){
	IUIAutomationElement* pFoundElement = NULL;
	IUIAutomationElementArray* pFoundedElements = NULL;
	IUIAutomationCondition* pControlCondition = NULL;
	int foundedElementCount;

	//localizedControlType 입력에 따른 조건 처리 
	if (controlTypeID == NULL){
		//NULL인 경우
		g_pAutomation->CreateTrueCondition(&pControlCondition);
	}
	else{
		VARIANT var;
		commonLib::SetVariantAsI4(var, controlTypeID);
		g_pAutomation->CreatePropertyCondition(UIA_ControlTypePropertyId, var, &pControlCondition);
	}

	pFoundedElements = this->FindAll(pStartElement, TreeScope_Subtree, pControlCondition);
	if (pFoundedElements==NULL){
		Log(L"DEBUG", L"[CAUIElementLib::FindElementByName] FindAll Failed");
		return NULL;
	}

	pFoundedElements->get_Length(&foundedElementCount);
	IUIAutomationElement* pSearchElement = NULL;
	BSTR name = NULL;
	for (int i = 0; i < foundedElementCount; i++){
		//찾아진 엘리먼트들을 탐색하면서 NAME이 동일한 엘리먼트를 찾는다.
		pFoundedElements->GetElement(i, &pSearchElement);
		pSearchElement->get_CurrentName(&name);
		if (name == NULL){
			continue;
		}
		if (_wcsicmp(szElementName, name) == 0){
			//NAME이 일치하는 엘리먼트를 찾은 경우
			pFoundElement = pSearchElement;
			return pFoundElement;
		}
	}

	//사용한 인터페이스 정리 
	pFoundedElements->Release();
	pControlCondition->Release();
	return pFoundElement;
}

/*
*/
IUIAutomationElement* CAUIElementLib::FindElementByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID)
{
	IUIAutomationElement* pProcessTopElement = NULL;
	IUIAutomationElement* pFoundElement = NULL;
	pProcessTopElement = GetTopElement(szProcessName);
	if (pProcessTopElement == NULL){
		Log(L"ERROR", L"[FindElementByID] Process Top Element Not Founded");
		throw exception("[FindElementByID] Process Top Element Not Founded");
	}

	pFoundElement = FindElementByIDFromElement(pProcessTopElement, szAutomationID, controlTypeID);
	if (pFoundElement == NULL){
		Log(L"ERROR", L"[FindElementByID] Element Not Founded");
		throw exception("[FindElementByID] Element Not Founded");
	}
	Sleep(100);
	return pFoundElement;
}

/*
*/
IUIAutomationElement* CAUIElementLib::FindElementByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID)
{
	IUIAutomationElement* pProcessTopElement = NULL;
	IUIAutomationElement* pFoundElement = NULL;
	pProcessTopElement = GetTopElement(szProcessName);
	if (pProcessTopElement == NULL){
		Log(L"ERROR", L"[FindElementByName] Process Top Element Not Founded");
		throw exception("[FindElementByName] Process Top Element Not Founded");
	}

	pFoundElement = FindElementByNameFromElement(pProcessTopElement, szFindText, controlTypeID);
	if (pFoundElement == NULL){
		Log(L"ERROR", L"[FindElementByName] Element Not Founded");
		throw exception("[FindElementByName] Element Not Founded");
	}
	Sleep(100);
	return pFoundElement;
}

/**
UIAutomationCore.dll FindFirst함수를 수행하고 결과에 따른 true/false 처리 함수
FindFirst에서 에러를 반환하거나 엘리먼트를 찾지 못하면 false를 반환
찾은 엘리먼트를 pFoundElement 포인터로 반환 한다.
*/
IUIAutomationElement* CAUIElementLib::FindFirst(IUIAutomationElement* pStartElement, TreeScope scope, IUIAutomationCondition* condition){
	IUIAutomationElement* pFoundElement = NULL;
	HRESULT hr = pStartElement->FindFirst(scope, condition, &pFoundElement);
	if (FAILED(hr) || pFoundElement == NULL){
		//오류가 발생했거나 엘리먼트를 찾지 못한 경우
		Log(L"DEBUG", L"[CAUIElementLib::FindFirst] FindFist Failed");
		return pFoundElement;
	}
	return pFoundElement;
}

/**
UIAutomationCore.dll FindAll를 수행하고 결과에 따른 true/false 처리 함수
FindAll에서 에러를 반환하거나 엘리먼트를 한개도 찾지 못하면 false를 반환
찾은 엘리먼트를 pFoundElementArray포인터로 반환 한다.
*/
IUIAutomationElementArray* CAUIElementLib::FindAll(IUIAutomationElement* pStartElement, TreeScope scope, IUIAutomationCondition* condition){
	IUIAutomationElementArray* pFoundElementArray = NULL;
	try{
		HRESULT hr = pStartElement->FindAll(scope, condition, &pFoundElementArray);
		if (FAILED(hr) || pFoundElementArray == NULL){
			Log(L"DEBUG", L"[CAUIElementLib::FindAll] FindALL Failed");
			return pFoundElementArray;
		}
	}
	catch (const std::exception& e){
		cout << "Exception Error\n";
		Log2(L"ERROR", L"CAUIElementLib::FindAll", e.what());
		throw exception(e.what());
	}
	return pFoundElementArray;
}


//타겟으로 하는 엘리먼트를 찾는 순간 해당 기록 할 정보들.
//나중에 해당 정보들을 가지고 element를 컨트롤 하거나 해당 element를 기준으로 탐색을 한다.
ELEMENTLIST CAUIElementLib::ListElementArray(IUIAutomationElementArray* pElementArray){
	ELEMENTLIST results;
	int elementCount;
	IUIAutomationElement* currentElement;

	pElementArray->get_Length(&elementCount);
	for (int i = 0; i < elementCount; i++){
		pElementArray->GetElement(i, &currentElement);
		results.push_back(currentElement);
	}
	return results;
}

/*
특정 엘리먼트에 대해서 type에 따라 마우스 클릭 동작을 한다.
*/
void CAUIElementLib::MouseAction(IUIAutomationElement* pTargetElement, MOUSECLICKTYPE type){

	RECT var;
	pTargetElement->get_CurrentBoundingRectangle(&var);
	//int zX = (int)var.left + (((int)var.right)/2);
	int zX = (int)var.left +10;
	int zY = (int)var.top + (((int)var.bottom- (int)var.top) / 2 );

	CAUIMouse* mouseControl = new CAUIMouse(zX,zY);
	//왼쪽클릭,왼쪽더블클릭,오른쪽더블클릭에 맞게 AUIMouse에서 멤버함수를 호출 한다.
	if (type == MOUSELEFTCLICK){
		mouseControl->MouseLeftClick(mouseControl->x,mouseControl->y);
	}
	else if (type == MOUSERIGHTCLICK){
		mouseControl->MouseRightClick(mouseControl->x, mouseControl->y);
	}
	else if (type == MOUSELEFTDOUBLECLICK){
		mouseControl->MouseLeftDoubleClick(mouseControl->x, mouseControl->y);
	}
	
}


/*
엘리먼트의 parent를 따라가면서 나오는 첫번째 윈도우 타입의 엘리먼트에 대해서
foregroundwindow의 핸들과 같은지 판단한다.
*/
bool CAUIElementLib::IsBelongToForegroundWindow(IUIAutomationElement* element){
	IUIAutomationTreeWalker *pWalker = NULL;
	IUIAutomationElement* pParent = NULL;
	IUIAutomationElement* pNode = element;
	HRESULT hr;
	g_pAutomation->get_ControlViewWalker(&pWalker);

	//현재 foregroundWindow 핸들
	HWND foregroundWindowHandle = GetForegroundWindow();
	UIA_HWND hHandle;

	//입력받은 엘리먼트 먼저 확인
	pNode->get_CurrentNativeWindowHandle(&hHandle);
	if (foregroundWindowHandle == hHandle){
		return true;
	}

	//부모를 따라가면서 확인
	while (true){
	    hr = pWalker->GetParentElement(pNode, &pParent);
		if (FAILED(hr) || pParent == NULL)
		{
			CAUIElement *ce = new CAUIElement();
			ce->SetElement(pNode);
			break;
		}
		pParent->get_CurrentNativeWindowHandle(&hHandle);
		if (foregroundWindowHandle == hHandle){
			return true;
		}
		pNode = pParent;
	}
	//여기 까지 온거면 못 찾은 거다.
	return false;
}

/*
엘리먼트의 부모 엘리먼트를 반환 한다.
엘리먼트의 부모가 없으면 NULL를 반환 한다.
*/
IUIAutomationElement* CAUIElementLib::GetParentElement(IUIAutomationElement* pChild){
	IUIAutomationTreeWalker *pWalker = NULL;
	IUIAutomationElement* pParent = NULL;
	IUIAutomationElement* pNode = pChild;
	HRESULT hr;
	g_pAutomation->get_ControlViewWalker(&pWalker);

	hr = pWalker->GetParentElement(pNode, &pParent);
	if (FAILED(hr) || pParent == NULL){
		pNode->Release();
		pWalker->Release();
		return NULL;
	}
	return pParent;
}

//Desktop Window를 제외하고 모든 엘리먼트를 찾는다.
ELEMENTLIST CAUIElementLib::GetALLElements(){

	ELEMENTLIST elementList;

	//항상 True인 조건을 만든다.
	IUIAutomationCondition* trueCondition = NULL;
	g_pAutomation->CreateTrueCondition(&trueCondition);

	//desktop window의 모든 자식엘리먼트를 찾는다.
	IUIAutomationElementArray* pFoundedElements = NULL;

	pFoundedElements = this->FindAll(pRootElement, TreeScope_Descendants, trueCondition);
	if (pFoundedElements==NULL){
		Log(L"DEBUG", L"[CAUIElementLib::GetALLElements] FindAll Failed");
		throw;
	}
	//IUIAutomationElementArray의 IUIAutomationElement들을 ELEMENTLIST에 하나씩 PUSH 한다.
	elementList = ListElementArray(pFoundedElements);
	return elementList;
}


/*
엘리먼트의 NativeWindowHandle을 구합니다.
*/
UIA_HWND CAUIElementLib::GetNativeWindowHandle(IUIAutomationElement* pIUIElement)
{
	UIA_HWND hNativeWindowHandle;
	pIUIElement->get_CurrentNativeWindowHandle(&hNativeWindowHandle);
	return hNativeWindowHandle;
}


/*
윈도우에 SC_CLOSE 메시지를 보내 윈도우를 종료 한다.
*/
void CAUIElementLib::CloseWindow(IUIAutomationElement* pIUIElement)
{
	HWND hTargetHandle = (HWND)this->GetNativeWindowHandle(pIUIElement);
	PWCHAR elementName = GetElementName(pIUIElement);
	if (IsTopWindow(pIUIElement)){
		//Top Window 이면
		Log(L"DEBUG", L"[CloseWindow] This Element is TOP Window. ElementName : %s", elementName);
		PostMessage(hTargetHandle, WM_DESTROY, 0, 0); // 윈도우 파괴 
		PostMessage(hTargetHandle, WM_QUIT, 0, 0); //파괴되는 윈도우가 메인 윈도우인 경우 꼭 LOOP를 종료 해야 메시지 루프를 계속 돌지 않음.
	}
	else{
		Log(L"DEBUG", L"[CloseWindow] This Element is Not Top Window. ElementName : %s", elementName);
		//Top Window가 아니면 SC_CLOSE를 해야 해당 윈도우가 종료 가능함.
		PostMessage(hTargetHandle, WM_SYSCOMMAND, SC_CLOSE, 0); // 사용자가 X버튼 누르거나, ALT+F4했을 때 발생하는 메시지 (SC_CLOSE)
	}
}

/*
이전 엘리먼트를 얻는다.
실패한 경우 NULL을 반환
*/
IUIAutomationElement* CAUIElementLib::GetPreviousElement(IUIAutomationElement* pIUIElement, int iPrevious)
{
	IUIAutomationTreeWalker* pControlWalker = NULL;
	g_pAutomation->get_ControlViewWalker(&pControlWalker);
	IUIAutomationElement* pFoundElement = NULL;
	while (iPrevious > 0){
		pFoundElement = NULL;
		HRESULT ret = pControlWalker->GetPreviousSiblingElement(pIUIElement, &pFoundElement);
		if (FAILED(ret) || pFoundElement == NULL){
			return NULL;
		}
		pIUIElement = pFoundElement;
		iPrevious--;
	}
	pControlWalker->Release();
	Log(L"DEBUG", L"[GetPreviousElement] Get Previous Element. ElementName : %s", this->GetElementName(pFoundElement));
	return pFoundElement;

}

/*
다음 엘리먼트를 얻는다.
실패한 경우 NULL을 반환
*/
IUIAutomationElement* CAUIElementLib::GetNextElement(IUIAutomationElement* pIUIElement, int iNext)
{
	IUIAutomationTreeWalker* pControlWalker = NULL;
	g_pAutomation->get_ControlViewWalker(&pControlWalker);
	IUIAutomationElement* pFoundElement = NULL;
	while (iNext > 0){
		pFoundElement = NULL;
		HRESULT ret = pControlWalker->GetNextSiblingElement(pIUIElement, &pFoundElement);
		if (FAILED(ret) || pFoundElement == NULL){
			return NULL;
		}
		pIUIElement = pFoundElement;
		iNext--;
	}
	Log(L"DEBUG", L"[GetNextElement] Get Next Element. ElementName : %s", this->GetElementName(pFoundElement));
	pControlWalker->Release();
	return pFoundElement;
}

/*
ELEMENT의 NAME속성을 반환 한다.
*/
wchar_t* CAUIElementLib::GetElementName(IUIAutomationElement* pIUIElement)
{
	PWCHAR szName = NULL;
	pIUIElement->get_CurrentName(&szName);
	return szName;
}


/*

*/
void CAUIElementLib::ClearWindows()
{
	IUIAutomationElementArray *pFoundedElements = NULL;
	IUIAutomationElement* pSearchElement = NULL;
	int foundedElementCount=0;
	int processID = NULL;
	//Top Element를 모두 구한다.
	
	int iRetryCount = 0;
	pFoundedElements = GetTopElements();
	pFoundedElements->get_Length(&foundedElementCount);
	for (int i = 0; i < foundedElementCount; i++){
		//찾아진 엘리먼트들을 탐색하면서 NAME이 동일한 엘리먼트를 찾는다.
		pFoundedElements->GetElement(i, &pSearchElement);
		pSearchElement->get_CurrentProcessId(&processID);

		PWCHAR name = GetElementName(pSearchElement);
		if (processID == NULL){
			continue;
		}
		string processName = commonLib::ProcessIdToName(processID);
		if (_stricmp("cmd.exe", processName.c_str()) == 0 || _stricmp("explorer.exe", processName.c_str()) == 0 ||
			_stricmp("conhost.exe", processName.c_str()) == 0 || _stricmp("devenv.exe", processName.c_str()) == 0)
		{
			//종료시키지 말아야 프로세스 이름인 경우 종료하지 않는다.
			continue;
		}
		else{
			//윈도우를 종료 시킨다.
			CloseWindow(pSearchElement);
		}
	}
}

/*
엘리먼트가 TOP WINDOW인지 여부를 반환 한다.
*/
bool CAUIElementLib::IsTopWindow(IUIAutomationElement* pIUIElement)
{
	IUIAutomationTreeWalker *pWalker = NULL;
	IUIAutomationElement* pParentElement = NULL;
	IUIAutomationElement* pPParentElement = NULL;

	g_pAutomation->get_ControlViewWalker(&pWalker);

	//부모 엘리먼트를 구한다.
	pWalker->GetParentElement(pIUIElement, &pParentElement);
	pWalker->GetParentElement(pParentElement, &pPParentElement);

	if (pPParentElement == NULL){
		//부모가 데스크톱엘리먼트 이면 (엘리먼트의 부모의 부모가 NULL이면 TOP으로 판단.
		return true;
	}
	return false;
}
