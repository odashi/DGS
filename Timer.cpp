// Timer.cpp
#include "Timer.h"

#include <windows.h>



/* class DGS::Stopwatch */

// c-tor
DGS::Stopwatch::Stopwatch()
{
	// �p�t�H�[�}���X�J�E���^�̎��g��
	unsigned long long f;
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&f));
	freq_ = static_cast<float>(f);

	start();
}

// ���ݎ�����������ɐݒ�
void DGS::Stopwatch::start()
{
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&begin_));
}

// ���������̌o�ߎ���
float DGS::Stopwatch::time()
{
	unsigned long long now_;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&now_));
	return (now_ - begin_) / freq_;
}
