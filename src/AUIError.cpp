#include "AUIError.h"

/*
_com_eror Ŭ������ ��� �Լ� 

ErrorMessage : ������ �����ϴ� const TCHAR * ����

Error : HRESULT ��, �� ���� �ڵ尪 ����

Source : IErrorInfo::GetSource ��� �Լ��� ȣ�� ���(_bstr_t) ��, ���� �ҽ� ����

Description : IErrorInfo::GetDescription ��� �Լ��� ȣ�� ���(_bstr_t) ����

*/

void Throw_if_fail(HRESULT hr)
{
	if (FAILED(hr)){
		throw _com_error(hr);
	}
}



