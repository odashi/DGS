// Thread.cpp
#include "Thread.h"

#include <process.h>

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

// c-tor
DGS::Thread::Thread(ThreadFunc func, void* data)
	: func_(func)
	, data_(data)
{
	th_ = reinterpret_cast<::HANDLE>(::_beginthreadex(nullptr, 0, callback, this, 0, nullptr));
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
	while (::ResumeThread(th_) > 0);
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
