// Window.h
// ウィンドウの管理
#pragma once

#include <string>
#include <windows.h>
#include "utility.h"

namespace DGS {
	class Window : private Uncopyable {
		static unsigned int s_object_num_;
		::HWND hwnd_;
		std::string title_;
		bool lb_down_, rb_down_, mb_down_;

		// ウィンドウクラスの登録
		bool registerWindowClass();

		// ウィンドウプロシージャ
		static LRESULT CALLBACK windowProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);

	public:
		Window(const std::string& title);
		~Window();

		// ウィンドウを開く
		void open();

		// ウィンドウを閉じる
		void close();

		// ウィンドウが開かれているかどうか
		bool isOpen() const { return !!hwnd_; }

		// ウィンドウハンドルの取得
		::HWND rawHandle() const { return hwnd_; }

		// ウィンドウが開かれているかどうかを調べる
		// 開かれていなければ例外を投げる
		void assertOpen() const;

		/* 
		 * 以下のメソッドはウィンドウが開かれているときのみ実行可能である。
		 * それ以外の場合には例外を投げる。
		 */

		// クライアント領域のサイズを設定する
		// (ウィンドウの大きさも変更される)
		void setClientSize(unsigned int width, unsigned int height);

		// ウィンドウ全体のサイズを設定する
		void setWindowSize(unsigned int width, unsigned int height);

		// マウスボタンが押されているかどうかの確認
		bool mouseDownL() { return lb_down_; }
		bool mouseDownR() { return rb_down_; }
		bool mouseDownM() { return mb_down_; }
	};
}
