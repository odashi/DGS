// Hook.h
// �ꕔ�̃L�[�𖳌��ɂ���@�\
#pragma once

#include <windows.h>
#include "utility.h"

namespace DGS {
	// �L�[�{�[�h�t�b�N
	// APP�L�[��WIN�L�[�𖳌��ɂ���
	class KeyboardHook : private Uncopyable {
		static ::HHOOK s_hhook_;

		// �t�b�N�v���V�[�W��
		static LRESULT CALLBACK hookProc(int code, WPARAM wp, LPARAM lp);

	public:
		KeyboardHook();
		~KeyboardHook();
	};
}
