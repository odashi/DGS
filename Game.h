// Game.h
// 中核の処理
#pragma once

#include <string>
#include <windows.h>

namespace DGS {
	class Game {
		static Game* s_current_object_;
		::HINSTANCE hinst_;
		std::string identifier_;
		unsigned int frame_rate_;
		unsigned long long int elapsed_;
		unsigned long long int old_time_;
	public:
		// 現在のオブジェクトを返す
		// なければ例外を投げる
		static Game& getCurrentObject();

		// メッセージキューを一掃する
		static void flushMessages();

		// エラー時のメッセージボックスを表示
		static void showErrorDialog(const std::string& message);

		// ゲーム全体の初期化
		Game(::HINSTANCE hinst, const std::string& identifier, unsigned int frame_rate_);

		// ゲーム全体の終了
		~Game();

		// インスタンスハンドルの取得
		::HINSTANCE instanceHandle() const { return hinst_; }

		// ゲーム識別子の取得
		std::string identifier() const { return identifier_; }

		// フレームレートの取得
		unsigned int frameRate() const { return frame_rate_; }

		// 待機
		bool wait();
	};
}
