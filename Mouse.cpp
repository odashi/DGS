// Mouse.cpp
#include "Mouse.h"

DGS::Mouse::Mouse(Window& window)
	: hwnd_(window.rawHandle())
	, lb_(0)
	, rb_(0)
	, mb_(0)
	, old_lb_(0)
	, old_rb_(0)
	, old_mb_(0)
{
	p_.x = 0L;
	p_.y = 0L;
}

// ���̍X�V
void DGS::Mouse::update()
{
	// �X�N���[�����W�ŃJ�[�\���ʒu���擾���ăN���C�A���g���W�ɕϊ�
	::GetCursorPos(&p_);
	::ScreenToClient(hwnd_, &p_);

	// �}�E�X��������Ă��邩�ǂ����̍X�V
	old_lb_ = lb_;
	old_rb_ = rb_;
	old_mb_ = mb_;
	lb_ = ::GetKeyState(VK_LBUTTON);
	rb_ = ::GetKeyState(VK_RBUTTON);
	mb_ = ::GetKeyState(VK_MBUTTON);
}

// �L�[���オ���Ă��邩�ǂ���
bool  DGS::Mouse::up(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return !(lb_ & 0x80);
	case DGS_MOUSE_BUTTON_RIGHT: return !(rb_ & 0x80);
	case DGS_MOUSE_BUTTON_MIDDLE: return !(mb_ & 0x80);
	}
	return false;
}

// �L�[���������Ă��邩�ǂ���
bool DGS::Mouse::down(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return !!(lb_ & 0x80);
	case DGS_MOUSE_BUTTON_RIGHT: return !!(rb_ & 0x80);
	case DGS_MOUSE_BUTTON_MIDDLE: return !!(mb_ & 0x80);
	}
	return false;
}

// �L�[�����̃t���[���ŉ����ꂽ���ǂ���
bool DGS::Mouse::pressed(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return (!!(lb_ & 0x80) && !(old_lb_ & 0x80));
	case DGS_MOUSE_BUTTON_RIGHT: return (!!(rb_ & 0x80) && !(old_rb_ & 0x80));
	case DGS_MOUSE_BUTTON_MIDDLE: return (!!(mb_ & 0x80) && !(old_mb_ & 0x80));
	}
	return false;
}

// �L�[�����̃t���[���ŗ����ꂽ���ǂ���
bool DGS::Mouse::released(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return (!(lb_ & 0x80) && !!(old_lb_ & 0x80));
	case DGS_MOUSE_BUTTON_RIGHT: return (!(rb_ & 0x80) && !!(old_rb_ & 0x80));
	case DGS_MOUSE_BUTTON_MIDDLE: return (!(mb_ & 0x80) && !!(old_mb_ & 0x80));
	}
	return false;
}
