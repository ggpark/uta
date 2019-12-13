/*
UI Element�� Control ���� class
*/

#ifndef AUICONTROL_H
#define AUICONTROL_H

#include <UIAutomationClient.h>
#include "CommonType.h"
#include "AUILog.h"
#include "CommonLib.h"
namespace Automation{

	/*
	UI Element�� ���� ��Ʈ�� ���� Ŭ����
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
