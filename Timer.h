// Timer.h
// �����Ԋ֘A�̏���
#pragma once

namespace DGS {
	// ���������̌o�ߎ��Ԃ𑪒肷��
	class Stopwatch {
		unsigned long long begin_;
		float freq_;
	public:
		Stopwatch();
		~Stopwatch() {}

		// ���ݎ�����������ɐݒ�
		void start();

		// ���������̌o�ߎ���
		float time();
	};
}
