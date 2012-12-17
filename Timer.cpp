// Timer.cpp
#include "Timer.h"

#include <windows.h>



/* class DGS::Stopwatch */

// c-tor
DGS::Stopwatch::Stopwatch()
{
	// パフォーマンスカウンタの周波数
	unsigned long long f;
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&f));
	freq_ = static_cast<float>(f);

	start();
}

// 現在時刻を基準時刻に設定
void DGS::Stopwatch::start()
{
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&begin_));
}

// 基準時刻からの経過時間
float DGS::Stopwatch::time()
{
	unsigned long long now_;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&now_));
	return (now_ - begin_) / freq_;
}
