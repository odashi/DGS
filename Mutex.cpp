// Mutex.cpp
#include "Mutex.h"

#include "Game.h"

// ���ʂ̏���������
void DGS::Mutex::open()
{
	// �~���[�e�b�N�X����
	mutex_ = ::CreateMutexA(NULL, TRUE, name_.c_str());
	Win32ErrorCode err;

	if (!mutex_)
		throw Win32Exception(err);

	// ���Ɏ擾����Ă��邩�ǂ���
	duplicated_ = (err == ERROR_ALREADY_EXISTS);
}

// c-tor
// �Q�[�����ʎq�Ń~���[�e�b�N�X�𐶐�
DGS::Mutex::Mutex()
	: name_(Game::getCurrentObject().identifier())
{
	open();
}

// c-tor
// ���O���w�肵�ă~���[�e�b�N�X�𐶐�
DGS::Mutex::Mutex(const std::string& name)
	: name_(name)
{
	// �Q�[�����ʎq���g�p���Ȃ��ꍇ�ł� Game �̏��������K�v�Ȃ悤�ɂ���B
	Game::getCurrentObject();

	open();
}

// d-tor
DGS::Mutex::~Mutex()
{
	if (mutex_)
		::ReleaseMutex(mutex_);
}
