// Timer.h
// 実時間関連の処理
#pragma once

namespace DGS {
	// 基準時刻からの経過時間を測定する
	class Stopwatch {
		unsigned long long begin_;
		float freq_;
	public:
		Stopwatch();
		~Stopwatch() {}

		// 現在時刻を基準時刻に設定
		void start();

		// 基準時刻からの経過時間
		float time();
	};
}
