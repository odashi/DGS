// Hook.h
// 一部のキーを無効にする機能
#pragma once

#include <windows.h>
#include "utility.h"

namespace DGS {
	// キーボードフック
	// APPキーやWINキーを無効にする
	class KeyboardHook : private Uncopyable {
		static ::HHOOK s_hhook_;

		// フックプロシージャ
		static LRESULT CALLBACK hookProc(int code, WPARAM wp, LPARAM lp);

	public:
		KeyboardHook();
		~KeyboardHook();
	};
}
