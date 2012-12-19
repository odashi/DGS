// Thread.cpp
#include "Thread.h"

#include <process.h>



/* class DGS::Thread */

// スレッドプロシージャ
unsigned int CALLBACK DGS::Thread::callback(void* data)
{
	Thread* thread = reinterpret_cast<Thread*>(data);
	unsigned int ret = 0;

	try {
		// 登録されている関数の実行
		ret = (*thread->func_)(thread->data_);
	} catch (...) {
		// 例外が上に戻るとまずいので、ここで全て受け止める。
		// 本来は func_ の内部で適切に処理するべきである。
	}

	// STILL_ACTIVE をリターンコードとして返してはいけないので、
	// ret がこの値だった場合は 0 を返す。
	if (ret == STILL_ACTIVE)
		ret = 0;

	return ret;
}

// スレッドの休止
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

// スレッドの再開
void DGS::Thread::resume()
{
	while (::ResumeThread(th_) > 1);
}

// スレッドの停止
void DGS::Thread::suspend()
{
	::SuspendThread(th_);
}

// スレッドが終了するまで待機
void DGS::Thread::join()
{
	resume();
	::WaitForSingleObject(th_, INFINITE);
}

// スレッドが生きているかどうか
bool DGS::Thread::isAlive()
{
	return (exitCode() == STILL_ACTIVE);
}

// スレッドの戻り値を取得
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

// 権限を取得
void DGS::CriticalSection::enter()
{
	::EnterCriticalSection(&cs_);
}

// 権限の解放
void DGS::CriticalSection::leave()
{
	::LeaveCriticalSection(&cs_);
}
