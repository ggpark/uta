#include "AUIWindow.h"
#include "AUIElement.h"
using namespace Automation;

CAUIWindow::CAUIWindow(){}
CAUIWindow::~CAUIWindow(){}

/**
������ �ڵ��� �Է¹޾� �ش� �����츦 TOP���� �ø���.
*/
bool CAUIWindow::SetTopWindow(UIA_HWND uiHwnd)
{

	HWND targetHwnd = (HWND)uiHwnd;
	//SetWindowPos�� ����Ϸ��� �ش� �����츦 ������ ���μ����� foreground���� ��.
	BOOL isForeground = SetForegroundWindow(targetHwnd);
	if (isForeground == 0){
		Log(L"DEBUG", L"[SetTopWindow] SetForegroundWindow Fail. HWND : %p", targetHwnd);
		//���� ���߿� setforegroundwindow�� �����ϴ� ��찡 �߻��ϸ� �� �� ó�� ����.
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
Window�� ���� �ϴ��� Ȯ�� �Ѵ�.
0.2�ʸ��� 5���� 1�ʵ��� Ȯ�� �Ѵ�.
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
			//�����찡 �������� �ʴ´ٸ�
			Log(L"DEBUG",L"[IsWindowExist] Windows Not Exist. hwnd : %p",uiHwnd);
			bWindowExist = false;
			break;
		}
		Sleep(200);
		iRetryCount--;
	}
}


