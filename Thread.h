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
		// スレッドの休止
		static void sleep(unsigned int msec);

		Thread(ThreadFunc func, void* data, bool run_immediately = true);
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

		::HANDLE rawHandle() { return th_; }
	};

	// クリティカルセクション
	class CriticalSection : private Uncopyable {
		::CRITICAL_SECTION cs_;
	public:
		CriticalSection();
		~CriticalSection();

		// 権限の取得
		void enter();

		// 権限の解放
		void leave();
	};
}
