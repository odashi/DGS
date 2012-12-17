// Timer.h
// ÀŠÔŠÖ˜A‚Ìˆ—
#pragma once

namespace DGS {
	// Šî€‚©‚ç‚ÌŒo‰ßŠÔ‚ğ‘ª’è‚·‚é
	class Stopwatch {
		unsigned long long begin_;
		float freq_;
	public:
		Stopwatch();
		~Stopwatch() {}

		// Œ»İ‚ğŠî€‚Éİ’è
		void start();

		// Šî€‚©‚ç‚ÌŒo‰ßŠÔ
		float time();
	};
}
