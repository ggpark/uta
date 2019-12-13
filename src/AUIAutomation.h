#ifndef AUIAUTOMATION_H
#define AUIAUTOMATION_H

#include <UIAutomationClient.h>
#include "AUILog.h"

//���� ���� ����
//�ش� ���� ������ 2�� �̻��� cpp���Ͽ��� �����ϸ� �ߺ����� ������ �߻�.
//�ش� ���������� cpp���� ��� �Ϸ��� AUIAutomation.h�� include ����.
extern IUIAutomation* g_pAutomation;

namespace Automation{
	class CAUIElementLib;
	class CAUIElement;
	class CAUIWindow;
	class CAUIControl;

	//������ ��� �� �������̽����� ����� Ŭ����
	class CAUIAutomation : public Automation::CAUILog{

	public:
		CAUIAutomation();
			//IUIAumation �������̽��� pAutomation �����ͷ� ��ȯ

		~CAUIAutomation();
			//��ü�� �Ҹ�� �� �����ؾ��� �۾����� �ۼ�
		
		//TO-DO : ������ RF���� ���� ���̺귯��
		void TargetWindowByName(PWCHAR szProcessName, PWCHAR szFindText);
		void TargetWindowByID(PWCHAR szProcessName, PWCHAR szAuotmationID);
		void VerifyTopWindowWait(PWCHAR szProcessName, PWCHAR szFindText, int waitTime);
		void VerifyTextWait(PWCHAR szProcessName, PWCHAR szFindText, int waitTime);
		void VerifySelectItem(PWCHAR szProcessName, PWCHAR szFindText);
		void VerifyUnSelectItem(PWCHAR szProcessName, PWCHAR szFindText);
		void VerifyCheckBoxON(PWCHAR szProcessName, PWCHAR szFindText);
		void VerifyCheckBoxOFF(PWCHAR szProcessName, PWCHAR szFindText);
		void MouseLeftByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID = NULL);
		void MouseRightByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID = NULL);
		void MouseLeftDoubleByName(PWCHAR szProcessName, PWCHAR szFindText, int controlTypeID = NULL);
		void MouseLeftByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID = NULL);
		void MouseRightByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID = NULL);
		void MouseLeftDoubleByID(PWCHAR szProcessName, PWCHAR szAutomationID, int controlTypeID = NULL);
		void CloseWindow(PWCHAR szProcessName, PWCHAR szFindText);
		void ClearWindows();
		void MouseLeftPreviousByName(PWCHAR szProcessName, PWCHAR szFindText,int iPreviousStep);
		void MouseLeftNextByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep);
		void MouseLeftDoublePreviousByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep);
		void MouseLeftDoubleNextByName(PWCHAR szProcessName, PWCHAR szFindText, int iNextStep);
		void InvokeByID(PWCHAR szProcessName, PWCHAR szAutomationID);
		void ToggleByID(PWCHAR szProcessName, PWCHAR szAutomationID);
		void ToggleByName(PWCHAR szProcessName, PWCHAR szFindText);
		void InvokeByName(PWCHAR szProcessName, PWCHAR szFindText);
		void SelectByID(PWCHAR szProcessName, PWCHAR szAutomationID);
		void SelectByName(PWCHAR szProcessName, PWCHAR szFindText);
		PWCHAR GetTextById(PWCHAR szProcessName, PWCHAR szAutomationID);
		void SetTextByID(PWCHAR szProcessName, PWCHAR szAutomationID, PWCHAR szText);

		//�Ʒ� ������� ���߿� private���� �ٲ���� (�� Ŭ������ ���ο��� �׽�Ʈ �ϱ� ���� Ǯ����� ����)
		HRESULT InitializeUIAutomation(IUIAutomation **ppAutomation);
		CAUIElementLib* elementLib=NULL;
		CAUIElement* abstractElement = NULL;
		CAUIWindow* windowControl = NULL;
		CAUIControl* elementControl = NULL;
	};
}


#endif  // !AUIAUTOMATION_H