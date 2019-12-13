
#include "AUIAutomation.h"
#include "AUIElementLib.h"
#include "AUIElement.h"
#include "CommonLib.h"
#include "AUIMouse.h"
#include "AUIWindow.h"

using namespace Automation;

CAUIElementLib::CAUIElementLib(){
	//���� 
	HRESULT hr = g_pAutomation->GetRootElement(&pRootElement);
	if (FAILED(hr)){
		Log(L"DEBUG", L"[CAUIElementLib] GetRootElement Failed");
	}
}

CAUIElementLib::~CAUIElementLib(){
	this->pRootElement->Release();
}

/*
����ũž ������Ʈ�� �θ�� �ϴ� ž ���� ������Ʈ���� ��� ��ȯ
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
	//����
	pTrueCondtion->Release();

	return pFoundElementArray;
}

/*
Root ������Ʈ ���� ������Ʈ�� ���μ����̸��� �ش��ϴ� ������Ʈ�� ��ȯ 
������Ʈ�� ã�� ���ϸ� false�� ��ȯ
*/
IUIAutomationElement* CAUIElementLib::GetTopElement(PWCHAR szProcessName){
	// desktop window�� �θ�� ���� ������Ʈ�� ���μ��� �̸��� ��ġ�ϴ� ������Ʈ ��ȯ
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
	//����
	pProcessCondtion->Release();
	return pFoundElement;
}

IUIAutomationElement* CAUIElementLib::GetTopElement(PWCHAR szProcessName, PWCHAR szFindText){
	// desktop window�� �θ�� ���� ������Ʈ�� ���μ��� �̸��� ��ġ�ϰ� ��Ʈ��Ÿ���� ��ġ�ϴ� ������Ʈ ��ȯ
	IUIAutomationCondition* pProcessCondtion = NULL;
	IUIAutomationCondition* pNameCondition = NULL;
	IUIAutomationCondition* pProcessAndNameCondition = NULL;
	IUIAutomationElement* pFoundElement = NULL;
	//���μ��� ����
	VARIANT varProcess;
	DWORD processId;
	processId = commonLib::GetProcessIdFromName(szProcessName);
	commonLib::SetVariantAsI4(varProcess, processId);
	g_pAutomation->CreatePropertyCondition(UIA_ProcessIdPropertyId, varProcess, &pProcessCondtion);

	//��Ʈ�� Ÿ�� ����
	VARIANT varName;
	commonLib::SetVariantAsBSTR(varName, szFindText);
	g_pAutomation->CreatePropertyCondition(UIA_NamePropertyId, varName, &pNameCondition);
	//SysFreeString(varName.bstrVal);

	//���μ��� + ��Ʈ�� Ÿ�� ����
	g_pAutomation->CreateAndCondition(pProcessCondtion, pNameCondition, &pProcessAndNameCondition);

	pFoundElement = this->FindFirst(this->pRootElement, TreeScope_Children, pProcessAndNameCondition);
	if (pFoundElement==NULL){
		Log(L"DEBUG", L"[GetTopElement] FindFirst Failed");
		return pFoundElement;
	}

	return pFoundElement;
}



/**
Automation ID �Ӽ����� ������Ʈ�� ã�� CAUIElement ��ü�� ��ȯ
*/
IUIAutomationElement* CAUIElementLib::FindElementByIDFromElement(IUIAutomationElement* pStartElement, PWCHAR szAutomationID, int controlTypeID){
	IUIAutomationElement* pFoundElement = NULL;
	IUIAutomationElementArray* pFoundedElements = NULL;
	IUIAutomationCondition* pControlCondition = NULL;
	int foundedElementCount;

	//localizedControlType �Է¿� ���� ���� ó�� 
	if (controlTypeID == NULL){
		//NULL�� ���
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
		//ã���� ������Ʈ���� Ž���ϸ鼭 AutomationID�� ������ ������Ʈ�� ã�´�.
		pFoundedElements->GetElement(i, &pSearchElement);
		pSearchElement->get_CurrentAutomationId(&id);
		if (id == NULL){
			//NULL�̸� ������ �ʴ´�.
			continue;
		}
		if (_wcsicmp(szAutomationID, id) == 0){
			//automation id�� ��ġ�ϴ� ������Ʈ�� ã�� ���
			pFoundElement = pSearchElement;
			return pFoundElement;
		}
	}
	//����� �������̽� ���� 
	pFoundedElements->Release();
	pControlCondition->Release();
	return pFoundElement;
}

/*
Name�Ӽ����� ������Ʈ�� ã�� CAUIElement ��ü�� ��ȯ
*/
IUIAutomationElement* CAUIElementLib::FindElementByNameFromElement(IUIAutomationElement* pStartElement, PWCHAR szElementName, int controlTypeID){
	IUIAutomationElement* pFoundElement = NULL;
	IUIAutomationElementArray* pFoundedElements = NULL;
	IUIAutomationCondition* pControlCondition = NULL;
	int foundedElementCount;

	//localizedControlType �Է¿� ���� ���� ó�� 
	if (controlTypeID == NULL){
		//NULL�� ���
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
		//ã���� ������Ʈ���� Ž���ϸ鼭 NAME�� ������ ������Ʈ�� ã�´�.
		pFoundedElements->GetElement(i, &pSearchElement);
		pSearchElement->get_CurrentName(&name);
		if (name == NULL){
			continue;
		}
		if (_wcsicmp(szElementName, name) == 0){
			//NAME�� ��ġ�ϴ� ������Ʈ�� ã�� ���
			pFoundElement = pSearchElement;
			return pFoundElement;
		}
	}

	//����� �������̽� ���� 
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
UIAutomationCore.dll FindFirst�Լ��� �����ϰ� ����� ���� true/false ó�� �Լ�
FindFirst���� ������ ��ȯ�ϰų� ������Ʈ�� ã�� ���ϸ� false�� ��ȯ
ã�� ������Ʈ�� pFoundElement �����ͷ� ��ȯ �Ѵ�.
*/
IUIAutomationElement* CAUIElementLib::FindFirst(IUIAutomationElement* pStartElement, TreeScope scope, IUIAutomationCondition* condition){
	IUIAutomationElement* pFoundElement = NULL;
	HRESULT hr = pStartElement->FindFirst(scope, condition, &pFoundElement);
	if (FAILED(hr) || pFoundElement == NULL){
		//������ �߻��߰ų� ������Ʈ�� ã�� ���� ���
		Log(L"DEBUG", L"[CAUIElementLib::FindFirst] FindFist Failed");
		return pFoundElement;
	}
	return pFoundElement;
}

/**
UIAutomationCore.dll FindAll�� �����ϰ� ����� ���� true/false ó�� �Լ�
FindAll���� ������ ��ȯ�ϰų� ������Ʈ�� �Ѱ��� ã�� ���ϸ� false�� ��ȯ
ã�� ������Ʈ�� pFoundElementArray�����ͷ� ��ȯ �Ѵ�.
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


//Ÿ������ �ϴ� ������Ʈ�� ã�� ���� �ش� ��� �� ������.
//���߿� �ش� �������� ������ element�� ��Ʈ�� �ϰų� �ش� element�� �������� Ž���� �Ѵ�.
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
Ư�� ������Ʈ�� ���ؼ� type�� ���� ���콺 Ŭ�� ������ �Ѵ�.
*/
void CAUIElementLib::MouseAction(IUIAutomationElement* pTargetElement, MOUSECLICKTYPE type){

	RECT var;
	pTargetElement->get_CurrentBoundingRectangle(&var);
	//int zX = (int)var.left + (((int)var.right)/2);
	int zX = (int)var.left +10;
	int zY = (int)var.top + (((int)var.bottom- (int)var.top) / 2 );

	CAUIMouse* mouseControl = new CAUIMouse(zX,zY);
	//����Ŭ��,���ʴ���Ŭ��,�����ʴ���Ŭ���� �°� AUIMouse���� ����Լ��� ȣ�� �Ѵ�.
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
������Ʈ�� parent�� ���󰡸鼭 ������ ù��° ������ Ÿ���� ������Ʈ�� ���ؼ�
foregroundwindow�� �ڵ�� ������ �Ǵ��Ѵ�.
*/
bool CAUIElementLib::IsBelongToForegroundWindow(IUIAutomationElement* element){
	IUIAutomationTreeWalker *pWalker = NULL;
	IUIAutomationElement* pParent = NULL;
	IUIAutomationElement* pNode = element;
	HRESULT hr;
	g_pAutomation->get_ControlViewWalker(&pWalker);

	//���� foregroundWindow �ڵ�
	HWND foregroundWindowHandle = GetForegroundWindow();
	UIA_HWND hHandle;

	//�Է¹��� ������Ʈ ���� Ȯ��
	pNode->get_CurrentNativeWindowHandle(&hHandle);
	if (foregroundWindowHandle == hHandle){
		return true;
	}

	//�θ� ���󰡸鼭 Ȯ��
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
	//���� ���� �°Ÿ� �� ã�� �Ŵ�.
	return false;
}

/*
������Ʈ�� �θ� ������Ʈ�� ��ȯ �Ѵ�.
������Ʈ�� �θ� ������ NULL�� ��ȯ �Ѵ�.
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

//Desktop Window�� �����ϰ� ��� ������Ʈ�� ã�´�.
ELEMENTLIST CAUIElementLib::GetALLElements(){

	ELEMENTLIST elementList;

	//�׻� True�� ������ �����.
	IUIAutomationCondition* trueCondition = NULL;
	g_pAutomation->CreateTrueCondition(&trueCondition);

	//desktop window�� ��� �ڽĿ�����Ʈ�� ã�´�.
	IUIAutomationElementArray* pFoundedElements = NULL;

	pFoundedElements = this->FindAll(pRootElement, TreeScope_Descendants, trueCondition);
	if (pFoundedElements==NULL){
		Log(L"DEBUG", L"[CAUIElementLib::GetALLElements] FindAll Failed");
		throw;
	}
	//IUIAutomationElementArray�� IUIAutomationElement���� ELEMENTLIST�� �ϳ��� PUSH �Ѵ�.
	elementList = ListElementArray(pFoundedElements);
	return elementList;
}


/*
������Ʈ�� NativeWindowHandle�� ���մϴ�.
*/
UIA_HWND CAUIElementLib::GetNativeWindowHandle(IUIAutomationElement* pIUIElement)
{
	UIA_HWND hNativeWindowHandle;
	pIUIElement->get_CurrentNativeWindowHandle(&hNativeWindowHandle);
	return hNativeWindowHandle;
}


/*
�����쿡 SC_CLOSE �޽����� ���� �����츦 ���� �Ѵ�.
*/
void CAUIElementLib::CloseWindow(IUIAutomationElement* pIUIElement)
{
	HWND hTargetHandle = (HWND)this->GetNativeWindowHandle(pIUIElement);
	PWCHAR elementName = GetElementName(pIUIElement);
	if (IsTopWindow(pIUIElement)){
		//Top Window �̸�
		Log(L"DEBUG", L"[CloseWindow] This Element is TOP Window. ElementName : %s", elementName);
		PostMessage(hTargetHandle, WM_DESTROY, 0, 0); // ������ �ı� 
		PostMessage(hTargetHandle, WM_QUIT, 0, 0); //�ı��Ǵ� �����찡 ���� �������� ��� �� LOOP�� ���� �ؾ� �޽��� ������ ��� ���� ����.
	}
	else{
		Log(L"DEBUG", L"[CloseWindow] This Element is Not Top Window. ElementName : %s", elementName);
		//Top Window�� �ƴϸ� SC_CLOSE�� �ؾ� �ش� �����찡 ���� ������.
		PostMessage(hTargetHandle, WM_SYSCOMMAND, SC_CLOSE, 0); // ����ڰ� X��ư �����ų�, ALT+F4���� �� �߻��ϴ� �޽��� (SC_CLOSE)
	}
}

/*
���� ������Ʈ�� ��´�.
������ ��� NULL�� ��ȯ
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
���� ������Ʈ�� ��´�.
������ ��� NULL�� ��ȯ
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
ELEMENT�� NAME�Ӽ��� ��ȯ �Ѵ�.
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
	//Top Element�� ��� ���Ѵ�.
	
	int iRetryCount = 0;
	pFoundedElements = GetTopElements();
	pFoundedElements->get_Length(&foundedElementCount);
	for (int i = 0; i < foundedElementCount; i++){
		//ã���� ������Ʈ���� Ž���ϸ鼭 NAME�� ������ ������Ʈ�� ã�´�.
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
			//�����Ű�� ���ƾ� ���μ��� �̸��� ��� �������� �ʴ´�.
			continue;
		}
		else{
			//�����츦 ���� ��Ų��.
			CloseWindow(pSearchElement);
		}
	}
}

/*
������Ʈ�� TOP WINDOW���� ���θ� ��ȯ �Ѵ�.
*/
bool CAUIElementLib::IsTopWindow(IUIAutomationElement* pIUIElement)
{
	IUIAutomationTreeWalker *pWalker = NULL;
	IUIAutomationElement* pParentElement = NULL;
	IUIAutomationElement* pPParentElement = NULL;

	g_pAutomation->get_ControlViewWalker(&pWalker);

	//�θ� ������Ʈ�� ���Ѵ�.
	pWalker->GetParentElement(pIUIElement, &pParentElement);
	pWalker->GetParentElement(pParentElement, &pPParentElement);

	if (pPParentElement == NULL){
		//�θ� ����ũ�鿤����Ʈ �̸� (������Ʈ�� �θ��� �θ� NULL�̸� TOP���� �Ǵ�.
		return true;
	}
	return false;
}
