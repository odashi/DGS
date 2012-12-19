// Thread.cpp
#include "Thread.h"

#include <process.h>



/* class DGS::Thread */

// �X���b�h�v���V�[�W��
unsigned int CALLBACK DGS::Thread::callback(void* data)
{
	Thread* thread = reinterpret_cast<Thread*>(data);
	unsigned int ret = 0;

	try {
		// �o�^����Ă���֐��̎��s
		ret = (*thread->func_)(thread->data_);
	} catch (...) {
		// ��O����ɖ߂�Ƃ܂����̂ŁA�����őS�Ď󂯎~�߂�B
		// �{���� func_ �̓����œK�؂ɏ�������ׂ��ł���B
	}

	// STILL_ACTIVE �����^�[���R�[�h�Ƃ��ĕԂ��Ă͂����Ȃ��̂ŁA
	// ret �����̒l�������ꍇ�� 0 ��Ԃ��B
	if (ret == STILL_ACTIVE)
		ret = 0;

	return ret;
}

// �X���b�h�̋x�~
void DGS::Thread::sleep(unsigned int msec)
{
	::Sleep(msec);
}

// c-tor
DGS::Thread::Thread(ThreadFunc func, void* data, bool run_immediately)
	: func_(func)
	, data_(data)
{
	th_ = reinterpret_cast<::HANDLE>(::_beginthreadex(
		nullptr,
		0,
		callback,
		this,
		run_immediately ? 0 : CREATE_SUSPENDED,
		nullptr));
	if (!th_)
		throw Win32Exception();
}

// d-tor
DGS::Thread::~Thread()
{
	join();
	::CloseHandle(th_);
}

// �X���b�h�̍ĊJ
void DGS::Thread::resume()
{
	while (::ResumeThread(th_) > 1);
}

// �X���b�h�̒�~
void DGS::Thread::suspend()
{
	::SuspendThread(th_);
}

// �X���b�h���I������܂őҋ@
void DGS::Thread::join()
{
	resume();
	::WaitForSingleObject(th_, INFINITE);
}

// �X���b�h�������Ă��邩�ǂ���
bool DGS::Thread::isAlive()
{
	return (exitCode() == STILL_ACTIVE);
}

// �X���b�h�̖߂�l���擾
unsigned int DGS::Thread::exitCode()
{
	DWORD code;
	::GetExitCodeThread(th_, &code);
	return static_cast<unsigned int>(code);
}



/* class DGS::CriticalSection */

// c-tor
DGS::CriticalSection::CriticalSection()
{
	::InitializeCriticalSection(&cs_);
}

// d-tor
DGS::CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection(&cs_);
}

// �������擾
void DGS::CriticalSection::enter()
{
	::EnterCriticalSection(&cs_);
}

// �����̉��
void DGS::CriticalSection::leave()
{
	::LeaveCriticalSection(&cs_);
}
