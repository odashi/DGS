// Game.h
// ���j�̏���
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
		// ���݂̃I�u�W�F�N�g��Ԃ�
		// �Ȃ���Η�O�𓊂���
		static Game& getCurrentObject();

		// ���b�Z�[�W�L���[����|����
		static void flushMessages();

		// �G���[���̃��b�Z�[�W�{�b�N�X��\��
		static void showErrorDialog(const std::string& message);

		// �Q�[���S�̂̏�����
		Game(::HINSTANCE hinst, const std::string& identifier, unsigned int frame_rate_);

		// �Q�[���S�̂̏I��
		~Game();

		// �C���X�^���X�n���h���̎擾
		::HINSTANCE instanceHandle() const { return hinst_; }

		// �Q�[�����ʎq�̎擾
		std::string identifier() const { return identifier_; }

		// �t���[�����[�g�̎擾
		unsigned int frameRate() const { return frame_rate_; }

		// �ҋ@
		bool wait();
	};
}
