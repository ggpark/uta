#include "AUIError.h"

/*
_com_eror 클래스의 멤버 함수 

ErrorMessage : 에러를 설명하는 const TCHAR * 리턴

Error : HRESULT 값, 즉 에러 코드값 리턴

Source : IErrorInfo::GetSource 멤버 함수의 호출 결과(_bstr_t) 즉, 에러 소스 리턴

Description : IErrorInfo::GetDescription 멤버 함수의 호출 결과(_bstr_t) 리턴

*/

void Throw_if_fail(HRESULT hr)
{
	if (FAILED(hr)){
		throw _com_error(hr);
	}
}



