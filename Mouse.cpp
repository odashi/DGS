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

// 情報の更新
void DGS::Mouse::update()
{
	// スクリーン座標でカーソル位置を取得してクライアント座標に変換
	::GetCursorPos(&p_);
	::ScreenToClient(window_.rawHandle(), &p_);

	// マウスが押されているかどうかの更新
	old_lb_ = lb_;
	old_rb_ = rb_;
	old_mb_ = mb_;
	lb_ = window_.mouseDownL();
	rb_ = window_.mouseDownR();
	mb_ = window_.mouseDownM();
}

// キーが上がっているかどうか
bool  DGS::Mouse::up(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return !lb_;
	case DGS_MOUSE_BUTTON_RIGHT: return !rb_;
	case DGS_MOUSE_BUTTON_MIDDLE: return !mb_;
	}
	return false;
}

// キーが下がっているかどうか
bool DGS::Mouse::down(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return lb_;
	case DGS_MOUSE_BUTTON_RIGHT: return rb_;
	case DGS_MOUSE_BUTTON_MIDDLE: return mb_;
	}
	return false;
}

// キーがこのフレームで押されたかどうか
bool DGS::Mouse::pressed(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return (lb_ && !old_lb_);
	case DGS_MOUSE_BUTTON_RIGHT: return (rb_ && !old_rb_);
	case DGS_MOUSE_BUTTON_MIDDLE: return (mb_ && !old_mb_);
	}
	return false;
}

// キーがこのフレームで離されたかどうか
bool DGS::Mouse::released(MouseButtons button) const
{
	switch (button) {
	case DGS_MOUSE_BUTTON_LEFT: return (!lb_ && old_lb_);
	case DGS_MOUSE_BUTTON_RIGHT: return (!rb_ && old_rb_);
	case DGS_MOUSE_BUTTON_MIDDLE: return (!mb_ && old_mb_);
	}
	return false;
}
