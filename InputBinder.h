// InputBinder.h
// �����̓��͌n���𓝍��A�L�[�R���t�B�O�Ȃǂ�����c�[��
#pragma once

#include <map>
#include "Mouse.h"
#include "Keyboard.h"
#include "Joypad.h"
#include "Window.h"
#include "utility.h"

namespace DGS {
	// ���͌n���̓���
	class InputBinder : private Uncopyable {

		// �o�C���h���
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
