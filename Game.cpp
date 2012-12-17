// Game.cpp
#include "Game.h"
#include "utility.h"

// ���݂̃I�u�W�F�N�g
DGS::Game* DGS::Game::s_current_object_ = nullptr;

// ���݂̃I�u�W�F�N�g��Ԃ�
// �Ȃ���Η�O�𓊂���
DGS::Game& DGS::Game::getCurrentObject()
{
	if (!s_current_object_)
		throw Exception("Game �I�u�W�F�N�g���쐬����Ă��܂���B");

	return *s_current_object_;
}

// ���b�Z�[�W�L���[����|����
void DGS::Game::flushMessages()
{
	MSG msg;
	while (::PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) break;
	}
}

// �G���[���̃��b�Z�[�W�{�b�N�X��\��
void DGS::Game::showErrorDialog(const std::string& message)
{
	::MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONERROR);
}

// �Q�[���S�̂̏�����
DGS::Game::Game(::HINSTANCE hinst, const std::string& identifier, unsigned int frame_rate)
	: hinst_(hinst)
	, identifier_(identifier)
	, frame_rate_(frame_rate)
	, elapsed_(0)
	, old_time_(0)
{
	if (s_current_object_)
		throw Exception("Game �I�u�W�F�N�g�͕����쐬�ł��܂���B");

	if (!hinst)
		throw Exception("�C���X�^���X�n���h���� NULL �ł��B");

	if (identifier == "")
		throw Exception("���̃Q�[�����ʎq�͎g�p�ł��܂���B");

	s_current_object_ = this;
}

// �Q�[���S�̂̏I��
DGS::Game::~Game()
{
	s_current_object_ = nullptr;
}

// �ҋ@
bool DGS::Game::wait()
{
	unsigned long long int freq, new_time;
	MSG msg;
	
	do {
		// ���b�Z�[�W����
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				// �E�B���h�E������ꂽ
				return false;
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		
		// �t���[�������҂�
		::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&freq));
		::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&new_time));
		elapsed_ += frame_rate_ * (new_time - old_time_);
		old_time_ = new_time;

	} while (elapsed_ < freq);
	
	// �덷�����̍X�V�Ɏc��
	elapsed_ %= freq;
	
	return true;
}
