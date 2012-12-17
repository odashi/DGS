// Hook.cpp
#include "Hook.h"

#include "Game.h"

::HHOOK DGS::KeyboardHook::s_hhook_ = NULL;

// フックプロシージャ
LRESULT CALLBACK DGS::KeyboardHook::hookProc(int code, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT *state = (KBDLLHOOKSTRUCT *)lp;
	const unsigned int keycodes[] = {VK_LWIN, VK_RWIN, VK_APPS};

	if (code < 0)
		return ::CallNextHookEx(s_hhook_, code, wp, lp);
	
	// トップレベルウィンドウが自分のアプリケーションのものかどうか調べる
	// プロパティリストにゲーム識別子と同じキーが存在するか確認すればよい
	::HWND top_window = ::GetForegroundWindow();
	::HANDLE flag = ::GetPropA(top_window, Game::getCurrentObject().identifier().c_str());
	
	// トップレベルウィンドウが別のアプリケーションのもののときは無視
	if (!flag)
		return ::CallNextHookEx(s_hhook_, code, wp, lp);

	// システムキーが押下されたかどうかを確認して無効にする
	for (unsigned int i = 0; i < sizeof(keycodes)/sizeof(int); i++)
		if (state->vkCode == keycodes[i])
			return 1;

	return ::CallNextHookEx(s_hhook_, code, wp, lp);
}

// c-tor
DGS::KeyboardHook::KeyboardHook()
{
	if (s_hhook_)
		throw Exception("KeyboardHook オブジェクトは複数作成できません。");

	// フックの生成
	s_hhook_ = ::SetWindowsHookExA(
		WH_KEYBOARD_LL,
		KeyboardHook::hookProc,
		Game::getCurrentObject().instanceHandle(),
		0);

	if (!s_hhook_)
		throw Win32Exception();
}

// d-tor
DGS::KeyboardHook::~KeyboardHook()
{
	// フックの解除
	::UnhookWindowsHookEx(s_hhook_);
	s_hhook_ = NULL;
}
