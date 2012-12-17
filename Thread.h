// Thread.h
// スレッド関連の処理
#pragma once

#include <windows.h>
#include "utility.h"

namespace DGS {
	
	// スレッド関数の型
	typedef unsigned int (*ThreadFunc)(void* data);
	
	// スレッドの管理
	class Thread : private Uncopyable {
		::HANDLE th_;
		ThreadFunc func_;
		void* data_;

		static unsigned int CALLBACK callback(void* data);

	public:
		Thread(ThreadFunc func, void* data);
		~Thread();

		// スレッドの再開
		void resume();

		// スレッドの停止
		void suspend();

		// スレッドが終了するまで待機
		void join();

		// スレッドが生きているかどうか
		bool isAlive();

		// スレッドの戻り値を取得
		unsigned int exitCode();
	};
}
