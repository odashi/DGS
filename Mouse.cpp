// Mouse.cpp
#include "Mouse.h"

DGS::Mouse::Mouse(Window& window)
	: window_(window)
	, lb_(false)
	, rb_(false)
	, mb_(false)
	, old_lb_(false)
	, old_rb_(false)
	, old_mb_(false)
{
	p_.x = 0L;
	p_.y = 0L;
}

// ���̍X�V
void DGS::Mouse::update()
{
	// �X�N���[�����W�ŃJ�[�\���ʒu���擾���ăN���C�A���g���W�ɕϊ�
	::GetCursorPos(&p_);
	::ScreenToClient(window_.rawHandle(), &p_);

	// �}�E�X��������Ă��邩�ǂ����̍X�V
	old_lb_ = lb_;
	old_rb_ = rb_;
	old_mb_ = mb_;
	lb_ = window_.mouseDownL();
	rb_ = window_.mouseDownR();
	mb_ = window_.mouseDownM();
}

// �L�[���オ���Ă��邩�ǂ���
bool  DGS::Mouse::up(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return !lb_;
	case DGS_MOUSE_BUTTON_RIGHT: return !rb_;
	case DGS_MOUSE_BUTTON_MIDDLE: return !mb_;
	}
	return false;
}

// �L�[���������Ă��邩�ǂ���
bool DGS::Mouse::down(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return lb_;
	case DGS_MOUSE_BUTTON_RIGHT: return rb_;
	case DGS_MOUSE_BUTTON_MIDDLE: return mb_;
	}
	return false;
}

// �L�[�����̃t���[���ŉ����ꂽ���ǂ���
bool DGS::Mouse::pressed(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return (lb_ && !old_lb_);
	case DGS_MOUSE_BUTTON_RIGHT: return (rb_ && !old_rb_);
	case DGS_MOUSE_BUTTON_MIDDLE: return (mb_ && !old_mb_);
	}
	return false;
}

// �L�[�����̃t���[���ŗ����ꂽ���ǂ���
bool DGS::Mouse::released(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return (!lb_ && old_lb_);
	case DGS_MOUSE_BUTTON_RIGHT: return (!rb_ && old_rb_);
	case DGS_MOUSE_BUTTON_MIDDLE: return (!mb_ && old_mb_);
	}
	return false;
}
