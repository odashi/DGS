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

// 情報の更新
void DGS::Mouse::update()
{
	// スクリーン座標でカーソル位置を取得してクライアント座標に変換
	::GetCursorPos(&p_);
	::ScreenToClient(hwnd_, &p_);

	// マウスが押されているかどうかの更新
	old_lb_ = lb_;
	old_rb_ = rb_;
	old_mb_ = mb_;
	lb_ = ::GetKeyState(VK_LBUTTON);
	rb_ = ::GetKeyState(VK_RBUTTON);
	mb_ = ::GetKeyState(VK_MBUTTON);
}

// キーが上がっているかどうか
bool  DGS::Mouse::up(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return !(lb_ & 0x80);
	case DGS_MOUSE_BUTTON_RIGHT: return !(rb_ & 0x80);
	case DGS_MOUSE_BUTTON_MIDDLE: return !(mb_ & 0x80);
	}
	return false;
}

// キーが下がっているかどうか
bool DGS::Mouse::down(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return !!(lb_ & 0x80);
	case DGS_MOUSE_BUTTON_RIGHT: return !!(rb_ & 0x80);
	case DGS_MOUSE_BUTTON_MIDDLE: return !!(mb_ & 0x80);
	}
	return false;
}

// キーがこのフレームで押されたかどうか
bool DGS::Mouse::pressed(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return (!!(lb_ & 0x80) && !(old_lb_ & 0x80));
	case DGS_MOUSE_BUTTON_RIGHT: return (!!(rb_ & 0x80) && !(old_rb_ & 0x80));
	case DGS_MOUSE_BUTTON_MIDDLE: return (!!(mb_ & 0x80) && !(old_mb_ & 0x80));
	}
	return false;
}

// キーがこのフレームで離されたかどうか
bool DGS::Mouse::released(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return (!(lb_ & 0x80) && !!(old_lb_ & 0x80));
	case DGS_MOUSE_BUTTON_RIGHT: return (!(rb_ & 0x80) && !!(old_rb_ & 0x80));
	case DGS_MOUSE_BUTTON_MIDDLE: return (!(mb_ & 0x80) && !!(old_mb_ & 0x80));
	}
	return false;
}
