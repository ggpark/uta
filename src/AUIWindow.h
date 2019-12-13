
#ifndef AUIWINDOW_H
#define AUIWINDOW_H
#include <UIAutomationClient.h>
#include "AUILog.h"
#include "CommonType.h"

namespace Automation{
	//Ŭ���� ���� ����
	class CAUIElement;

	class CAUIWindow : public Automation::CAUILog{

	public:
		CAUIWindow();
		~CAUIWindow();

		bool SetTopWindow(UIA_HWND uiHwnd);
		bool IsWindowExist(UIA_HWND uiHwnd);
	};
}
#endif // !AUIWINDOW_H
