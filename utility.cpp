// utility.cpp
#include "utility.h"

#include <dxerr.h>

/* class DGS::Win32ErrorCode */

// ::FormatMessage() ����G���[�̕�����\�L���擾���ĕԂ�
std::string DGS::Win32ErrorCode::toString() const
{
	LPSTR message;
	::FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		code_,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&message),
		0,
		NULL);
	std::string ret(message);
	::LocalFree(message);
	return ret;
}



/* class DGS::DirectXResult */

// ::DXGetErrorString() ����G���[�̕�����\�L���擾���ĕԂ�
std::string DGS::DirectXResult::toString() const
{
	return ::DXGetErrorStringA(code_);
}
