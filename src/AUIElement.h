

#ifndef AUIELEMENT_H
#define AUIELEMENT_H

#include <UIAutomationClient.h>
#include "CommonType.h"
#include "AUILog.h"

namespace Automation{

	/*
	UI Element 각각에 대한 클래스
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
