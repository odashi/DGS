// Mouse.h
// マウス関連の処理
#pragma once

#include <string>
#include <windows.h>
#include "window.h"
#include "utility.h"

namespace DGS {

	// マウスボタンの種類
	enum MouseButtons {
		DGS_MOUSE_BUTTON_LEFT,
		DGS_MOUSE_BUTTON_RIGHT,
		DGS_MOUSE_BUTTON_MIDDLE,
	};

	// マウスの管理
	class Mouse : private Uncopyable {
		Window& window_;
		POINT p_;
		bool lb_, rb_, mb_;
		bool old_lb_, old_rb_, old_mb_;
	public:
		Mouse(Window& window);
		~Mouse() {}

		// 情報の更新
		void update();

		// 座標の取得
		inline long x() const { return p_.x; }
		inline long y() const { return p_.y; }

		// ボタンが上がっているかどうか
		bool up(MouseButtons button) const;

		// ボタンが下がっているかどうか
		bool down(MouseButtons button) const;

		// ボタンが今押されたかどうか
		bool pressed(MouseButtons button) const;

		// ボタンが今離されたかどうか
		bool released(MouseButtons button) const;
	};
}
