

#ifndef AUIELEMENT_H
#define AUIELEMENT_H

#include <UIAutomationClient.h>
#include "CommonType.h"
#include "AUILog.h"

namespace Automation{

	/*
	UI Element ������ ���� Ŭ����
	*/
	class CAUIElement  : public Automation::CAUILog{

	public:
		CAUIElement();
		~CAUIElement();
		IUIAutomationElement* pIUIElement;
		UIA_HWND hNativeWindowHandle;
		PWCHAR szName=NULL;
		PWCHAR szAutomationID=NULL;
		PATTERNID patternId;
		CONTROLTYPEID controlTypeId;
		PWCHAR localizedControlType;
		
		bool SetElement(IUIAutomationElement* element);
	};
}

#endif // !AUIELEMENT_H
