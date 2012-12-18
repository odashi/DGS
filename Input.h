// Input.h
// ���͌n���̊Ǘ�
#pragma once

// DirectInput�̃o�[�W�����w��
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif // DIRECTINPUT_VERSIO

#include <string>
#include <list>
#include <map>
#include <windows.h>
#include <dinput.h>
#include <atlbase.h>
#include "window.h"
#include "utility.h"

namespace DGS {

	// ���͌n���̊��N���X
	class Input : private Uncopyable {
	public:
		Input() {}
		virtual ~Input() {}
		
		// ���̍X�V
		virtual void update() = 0;

		// ���W�l�̎擾
		virtual long axis(const std::string& key) const = 0;

		// �{�^�����オ���Ă��邩�ǂ���
		virtual bool up(const std::string& key) const = 0;

		// �{�^�����������Ă��邩�ǂ���
		virtual bool down(const std::string& key) const = 0;

		// �{�^�����������ꂽ���ǂ���
		virtual bool pressed(const std::string& key) const = 0;

		// �{�^�����������ꂽ���ǂ���
		virtual bool released(const std::string& key) const = 0;
	};

	// �}�E�X�̊Ǘ�
	class Mouse : public Input {
		Window& window_;
		POINT p_, old_p_;
		bool lb_, rb_, mb_;
		bool old_lb_, old_rb_, old_mb_;
	public:
		Mouse(Window& window);
		~Mouse() {}

		virtual void update();
		virtual long axis(const std::string& name) const;
		virtual bool up(const std::string& key) const;
		virtual bool down(const std::string& key) const;
		virtual bool pressed(const std::string& key) const;
		virtual bool released(const std::string& key) const;
	};

	// �L�[�{�[�h���͂̊Ǘ�
	class Keyboard : public Input {
		static const unsigned int BUFFER_SIZE = 256;

		std::map<std::string, int> table_;
		unsigned char buf_[BUFFER_SIZE];
		unsigned char oldbuf_[BUFFER_SIZE];

		unsigned int lookup(const std::string& key) const;

	public:
		Keyboard();
		~Keyboard() {}

		void update();
		long axis(const std::string& key) const;
		bool up(const std::string& key) const;
		bool down(const std::string& key) const;
		bool pressed(const std::string& key) const;
		bool released(const std::string& key) const;
	};

	// �W���C�p�b�h�̊Ǘ�
	class Joypad : public Input {
		static ::CComPtr<::IDirectInput8A> s_di_;
		static std::list<GUID> s_guids_;

		HWND hwnd_;
		long min_value_, max_value_;
		::CComPtr<::IDirectInputDevice8> dev_;
		GUID guid_;
		DIJOYSTATE2 state_, old_state_;
		unsigned int frame_;

		static BOOL CALLBACK joypadEnum(const DIDEVICEINSTANCE* ddi, void* ref);

		bool getDevice();
		void destroyDevice();

	public:
		Joypad(Window& window, long min_value = -1000L, long max_value = 1000L);
		~Joypad();

		void update();
		long axis(const std::string& key) const;
		bool up(const std::string& key) const;
		bool down(const std::string& key) const;
		bool pressed(const std::string& key) const;
		bool released(const std::string& key) const;
	};
}
