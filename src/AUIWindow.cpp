#include "AUIWindow.h"
#include "AUIElement.h"
using namespace Automation;

CAUIWindow::CAUIWindow(){}
CAUIWindow::~CAUIWindow(){}

/**
윈도우 핸들을 입력받아 해당 윈도우를 TOP으로 올린다.
*/
bool CAUIWindow::SetTopWindow(UIA_HWND uiHwnd)
{

	HWND targetHwnd = (HWND)uiHwnd;
	//SetWindowPos를 사용하려면 해당 윈도우를 소유한 프로세스가 foreground여야 함.
	BOOL isForeground = SetForegroundWindow(targetHwnd);
	if (isForeground == 0){
		Log(L"DEBUG", L"[SetTopWindow] SetForegroundWindow Fail. HWND : %p", targetHwnd);
		//만약 나중에 setforegroundwindow가 실패하는 경우가 발생하면 그 때 처리 하자.
		return false;
	}

	BOOL ret = ::SetWindowPos(targetHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	if (ret == 0){
		Log(L"DEBUG", L"[SetTopWindow] Set TOPMOST Window Fail. HWND : %p", targetHwnd);
		return false;
	}
	Log(L"DEBUG", L"[SetTopWindow] Set TOPMOST Window Success. HWND : %p", targetHwnd);
	Sleep(200);
	::SetWindowPos(targetHwnd, HWND_TOP, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	Sleep(500);
	return true;
}

/*
Window가 존재 하는지 확인 한다.
0.2초마다 5번씩 1초동안 확인 한다.
*/
bool CAUIWindow::IsWindowExist(UIA_HWND uiHwnd)
{
	int iRetryCount = 5;
	BOOL bTemp;
	bool bWindowExist = true;
	while (iRetryCount > 0)
	{
		bTemp = IsWindow((HWND)uiHwnd);
		if (!bTemp)
		{
			//윈도우가 존재하지 않는다면
			Log(L"DEBUG",L"[IsWindowExist] Windows Not Exist. hwnd : %p",uiHwnd);
			bWindowExist = false;
			break;
		}
		Sleep(200);
		iRetryCount--;
	}
}


