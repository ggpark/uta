/*
UI Element를 Control 관련 class
*/

#ifndef AUICONTROL_H
#define AUICONTROL_H

#include <UIAutomationClient.h>
#include "CommonType.h"
#include "AUILog.h"
#include "CommonLib.h"
namespace Automation{

	/*
	UI Element에 대한 컨트롤 관련 클래스
	*/
	class CAUIControl : public Automation::CAUILog{
	public:
		CAUIControl();
		~CAUIControl();

		HRESULT Invoke(IUIAutomationElement* pElement);
		HRESULT Select(IUIAutomationElement* pElement);
		bool IsSelected(IUIAutomationElement* pElement);
		HRESULT Toggle(IUIAutomationElement* pElement);
		HRESULT SetValue(IUIAutomationElement* pElement, PWCHAR szText);
		ToggleState GetToggleState(IUIAutomationElement* pElement);
	};
}

#endif // !AUICONTROLT_H
