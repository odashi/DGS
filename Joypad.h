// Joypad.h
// ジョイパッド関連の処理
#pragma once

// DirectInputのバージョン指定
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif // DIRECTINPUT_VERSIO

#include <list>
#include <atlbase.h>
#include <dinput.h>
#include "window.h"
#include "utility.h"

namespace DGS {
	// ジョイパッドの管理
	class Joypad : private Uncopyable {
		static ::CComPtr<::IDirectInput8A> s_di_;
		static std::list<GUID> s_guids_;

		HWND hwnd_;
		long min_value_, max_value_;
		::CComPtr<::IDirectInputDevice8> dev_;
		GUID guid_;
		DIJOYSTATE2 state_, old_state_;
		unsigned int frame_;

		static BOOL CALLBACK joypadEnum(const DIDEVICEINSTANCE* ddi, void* ref);

		bool getDevice();
		void destroyDevice();

	public:
		Joypad(Window& window, long min_value = -1000L, long max_value = 1000L);
		~Joypad();

		// 情報の更新
		void update();

		// 座標の取得
		inline long x() const { return state_.lX; }
		inline long y() const { return state_.lY; }
		//inline long z() const { return state_.lZ; }
		//inline long rx() const { return state_.lRx; }
		//inline long ry() const { return state_.lRy; }
		//inline long rz() const { return state_.lRz; }
		//inline long vx() const { return state_.lVX; }
		//inline long vy() const { return state_.lVY; }
		//inline long vz() const { return state_.lVZ; }
		//inline long vrx() const { return state_.lVRx; }
		//inline long vry() const { return state_.lVRy; }
		//inline long vrz() const { return state_.lVRz; }
		//inline long ax() const { return state_.lAX; }
		//inline long ay() const { return state_.lAY; }
		//inline long az() const { return state_.lAZ; }
		//inline long arx() const { return state_.lARx; }
		//inline long ary() const { return state_.lARy; }
		//inline long arz() const { return state_.lARz; }
		//inline long fx() const { return state_.lFX; }
		//inline long fy() const { return state_.lFY; }
		//inline long fz() const { return state_.lFZ; }
		//inline long frx() const { return state_.lFRx; }
		//inline long fry() const { return state_.lFRy; }
		//inline long frz() const { return state_.lFRz; }

		// ボタンが上がっているかどうか
		bool up(unsigned int index) const;

		// ボタンが下がっているかどうか
		bool down(unsigned int index) const;

		// ボタンが今押されたかどうか
		bool pressed(unsigned int index) const;

		// ボタンが今離されたかどうか
		bool released(unsigned int index) const;
	};
}
