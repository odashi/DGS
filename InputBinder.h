// InputBinder.h
// 複数の入力系統を統合、キーコンフィグなどをするツール
#pragma once

#include <map>
#include "Mouse.h"
#include "Keyboard.h"
#include "Joypad.h"
#include "Window.h"
#include "utility.h"

namespace DGS {
	// 入力系統の統合
	class InputBinder : private Uncopyable {

		// バインド情報
		struct BindInfo {
			Input* input;
			std::string 
		};

		std::map<unsigned int, 
	public:
		InputBinder(Window& window);
		~InputBinder();

		// 
	};
}
