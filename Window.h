// Window.h
// �E�B���h�E�̊Ǘ�
#pragma once

#include <string>
#include <windows.h>
#include "utility.h"

namespace DGS {
	class Window : private Uncopyable {
		static unsigned int s_object_num_;
		::HWND hwnd_;
		std::string title_;
		bool lb_down_, rb_down_, mb_down_;

		// �E�B���h�E�N���X�̓o�^
		bool registerWindowClass();

		// �E�B���h�E�v���V�[�W��
		static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);

	public:
		Window(const std::string& title);
		~Window();

		// �E�B���h�E���J��
		void open();

		// �E�B���h�E�����
		void close();

		// �E�B���h�E���J����Ă��邩�ǂ���
		bool isOpen() const { return !!hwnd_; }

		// �E�B���h�E�n���h���̎擾
		::HWND rawHandle() const { return hwnd_; }

		// �E�B���h�E���J����Ă��邩�ǂ����𒲂ׂ�
		// �J����Ă��Ȃ���Η�O�𓊂���
		void assertOpen() const;

		/* 
		 * �ȉ��̃��\�b�h�̓E�B���h�E���J����Ă���Ƃ��̂ݎ��s�\�ł���B
		 * ����ȊO�̏ꍇ�ɂ͗�O�𓊂���B
		 */

		// �N���C�A���g�̈�̃T�C�Y��ݒ肷��
		// (�E�B���h�E�̑傫�����ύX�����)
		void setClientSize(unsigned int width, unsigned int height);

		// �E�B���h�E�S�̂̃T�C�Y��ݒ肷��
		void setWindowSize(unsigned int width, unsigned int height);

		// �}�E�X�{�^����������Ă��邩�ǂ����̊m�F
		bool mouseDownL() { return lb_down_; }
		bool mouseDownR() { return rb_down_; }
		bool mouseDownM() { return mb_down_; }
	};
}
