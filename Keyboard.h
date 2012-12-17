// Keyboard.h
// �L�[�{�[�h�֘A�̏���
#pragma once

#include <map>
#include <string>
#include "utility.h"

namespace DGS {
	// �L�[�{�[�h���͂̊Ǘ�
	class Keyboard : public Uncopyable {

		static const unsigned int BUFFER_SIZE = 256;

		std::map<std::string, int> table_;
		unsigned char buf_[BUFFER_SIZE];
		unsigned char oldbuf_[BUFFER_SIZE];

		unsigned int lookup(const std::string& key) const;

	public:
		Keyboard();
		~Keyboard() {}

		// ���̍X�V
		void update();

		// �L�[���オ���Ă��邩�ǂ���
		bool up(const std::string& key) const;

		// �L�[���������Ă��邩�ǂ���
		bool down(const std::string& key) const;

		// �L�[���������ꂽ���ǂ���
		bool pressed(const std::string& key) const;

		// �L�[���������ꂽ���ǂ���
		bool released(const std::string& key) const;
	};
}
