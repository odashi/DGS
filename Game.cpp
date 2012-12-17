// Game.cpp
#include "Game.h"
#include "utility.h"

// 現在のオブジェクト
DGS::Game* DGS::Game::s_current_object_ = nullptr;

// 現在のオブジェクトを返す
// なければ例外を投げる
DGS::Game& DGS::Game::getCurrentObject()
{
	if (!s_current_object_)
		throw Exception("Game オブジェクトが作成されていません。");

	return *s_current_object_;
}

// メッセージキューを一掃する
void DGS::Game::flushMessages()
{
	MSG msg;
	while (::PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) break;
	}
}

// エラー時のメッセージボックスを表示
void DGS::Game::showErrorDialog(const std::string& message)
{
	::MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONERROR);
}

// ゲーム全体の初期化
DGS::Game::Game(::HINSTANCE hinst, const std::string& identifier, unsigned int frame_rate)
	: hinst_(hinst)
	, identifier_(identifier)
	, frame_rate_(frame_rate)
	, elapsed_(0)
	, old_time_(0)
{
	if (s_current_object_)
		throw Exception("Game オブジェクトは複数作成できません。");

	if (!hinst)
		throw Exception("インスタンスハンドルが NULL です。");

	if (identifier == "")
		throw Exception("このゲーム識別子は使用できません。");

	s_current_object_ = this;
}

// ゲーム全体の終了
DGS::Game::~Game()
{
	s_current_object_ = nullptr;
}

// 待機
bool DGS::Game::wait()
{
	unsigned long long int freq, new_time;
	MSG msg;
	
	do {
		// メッセージ処理
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				// ウィンドウが閉じられた
				return false;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		
		// フレーム同期待ち
		::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&freq));
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&new_time));
		elapsed_ += frame_rate_ * (new_time - old_time_);
		old_time_ = new_time;

	} while (elapsed_ < freq);
	
	// 誤差を次の更新に残す
	elapsed_ %= freq;
	
	return true;
}
