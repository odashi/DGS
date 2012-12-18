// Mouse.h
// �}�E�X�֘A�̏���
#pragma once

#include <string>
#include <windows.h>
#include "window.h"
#include "utility.h"

namespace DGS {

	// �}�E�X�{�^���̎��
	enum MouseButtons {
		DGS_MOUSE_BUTTON_LEFT,
		DGS_MOUSE_BUTTON_RIGHT,
		DGS_MOUSE_BUTTON_MIDDLE,
	};

	// �}�E�X�̊Ǘ�
	class Mouse : private Uncopyable {
		Window& window_;
		POINT p_;
		bool lb_, rb_, mb_;
		bool old_lb_, old_rb_, old_mb_;
	public:
		Mouse(Window& window);
		~Mouse() {}

		// ���̍X�V
		void update();

		// ���W�̎擾
		inline long x() const { return p_.x; }
		inline long y() const { return p_.y; }

		// �{�^�����オ���Ă��邩�ǂ���
		bool up(MouseButtons button) const;

		// �{�^�����������Ă��邩�ǂ���
		bool down(MouseButtons button) const;

		// �{�^�����������ꂽ���ǂ���
		bool pressed(MouseButtons button) const;

		// �{�^�����������ꂽ���ǂ���
		bool released(MouseButtons button) const;
	};
}
