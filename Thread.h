// Thread.h
// �X���b�h�֘A�̏���
#pragma once

#include <windows.h>
#include "utility.h"

namespace DGS {
	
	// �X���b�h�֐��̌^
	typedef unsigned int (*ThreadFunc)(void* data);
	
	// �X���b�h�̊Ǘ�
	class Thread : private Uncopyable {
		::HANDLE th_;
		ThreadFunc func_;
		void* data_;

		static unsigned int CALLBACK callback(void* data);

	public:
		// �X���b�h�̋x�~
		static void sleep(unsigned int msec);

		Thread(ThreadFunc func, void* data, bool run_immediately = true);
		~Thread();

		// �X���b�h�̍ĊJ
		void resume();

		// �X���b�h�̒�~
		void suspend();

		// �X���b�h���I������܂őҋ@
		void join();

		// �X���b�h�������Ă��邩�ǂ���
		bool isAlive();

		// �X���b�h�̖߂�l���擾
		unsigned int exitCode();

		::HANDLE rawHandle() { return th_; }
	};

	// �N���e�B�J���Z�N�V����
	class CriticalSection : private Uncopyable {
		::CRITICAL_SECTION cs_;
	public:
		CriticalSection();
		~CriticalSection();

		// �����̎擾
		void enter();

		// �����̉��
		void leave();
	};
}
