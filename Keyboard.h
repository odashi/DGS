// Keyboard.h
// キーボード関連の処理
#pragma once

#include <map>
#include <string>
#include "utility.h"

namespace DGS {
	// キーボード入力の管理
	class Keyboard : public Uncopyable {

		static const unsigned int BUFFER_SIZE = 256;

		std::map<std::string, int> table_;
		unsigned char buf_[BUFFER_SIZE];
		unsigned char oldbuf_[BUFFER_SIZE];

		unsigned int lookup(const std::string& key) const;

	public:
		Keyboard();
		~Keyboard() {}

		// 情報の更新
		void update();

		// キーが上がっているかどうか
		bool up(const std::string& key) const;

		// キーが下がっているかどうか
		bool down(const std::string& key) const;

		// キーが今押されたかどうか
		bool pressed(const std::string& key) const;

		// キーが今離されたかどうか
		bool released(const std::string& key) const;
	};
}
