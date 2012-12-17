// Hook.cpp
#include "Hook.h"

#include "Game.h"

::HHOOK DGS::KeyboardHook::s_hhook_ = NULL;

// �t�b�N�v���V�[�W��
LRESULT CALLBACK DGS::KeyboardHook::hookProc(int code, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT *state = (KBDLLHOOKSTRUCT *)lp;
	const unsigned int keycodes[] = {VK_LWIN, VK_RWIN, VK_APPS};

	if (code < 0)
		return ::CallNextHookEx(s_hhook_, code, wp, lp);
	
	// �g�b�v���x���E�B���h�E�������̃A�v���P�[�V�����̂��̂��ǂ������ׂ�
	// �v���p�e�B���X�g�ɃQ�[�����ʎq�Ɠ����L�[�����݂��邩�m�F����΂悢
	::HWND top_window = ::GetForegroundWindow();
	::HANDLE flag = ::GetPropA(top_window, Game::getCurrentObject().identifier().c_str());
	
	// �g�b�v���x���E�B���h�E���ʂ̃A�v���P�[�V�����̂��̂̂Ƃ��͖���
	if (!flag)
		return ::CallNextHookEx(s_hhook_, code, wp, lp);

	// �V�X�e���L�[���������ꂽ���ǂ������m�F���Ė����ɂ���
	for (unsigned int i = 0; i < sizeof(keycodes)/sizeof(int); i++)
		if (state->vkCode == keycodes[i])
			return 1;

	return ::CallNextHookEx(s_hhook_, code, wp, lp);
}

// c-tor
DGS::KeyboardHook::KeyboardHook()
{
	if (s_hhook_)
		throw Exception("KeyboardHook �I�u�W�F�N�g�͕����쐬�ł��܂���B");

	// �t�b�N�̐���
	s_hhook_ = ::SetWindowsHookExA(
		WH_KEYBOARD_LL,
		KeyboardHook::hookProc,
		Game::getCurrentObject().instanceHandle(),
		0);

	if (!s_hhook_)
		throw Win32Exception();
}

// d-tor
DGS::KeyboardHook::~KeyboardHook()
{
	// �t�b�N�̉���
	::UnhookWindowsHookEx(s_hhook_);
	s_hhook_ = NULL;
}
