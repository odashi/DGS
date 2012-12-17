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
		Thread(ThreadFunc func, void* data);
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
	};
}
