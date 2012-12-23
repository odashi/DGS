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
#include "Window.h"
#include "Thread.h"
#include "utility.h"

namespace DGS {

	// ���͌n���̊��N���X
	class Input : private Uncopyable {
	public:
		Input() {}
		virtual ~Input() {}
		
		// ���̍X�V
		virtual void update() = 0;

		// �ǂ̃{�^����������Ă��邩
		// �����̃{�^����������Ă���ꍇ�͍ŏ��Ɍ����������̂�Ԃ�
		// ����������Ă��Ȃ��ꍇ�͋󕶎����Ԃ�
		virtual std::string activeButton() const = 0;

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

		void update();
		std::string activeButton() const;
		long axis(const std::string& name) const;
		bool up(const std::string& key) const;
		bool down(const std::string& key) const;
		bool pressed(const std::string& key) const;
		bool released(const std::string& key) const;
	};

	// �L�[�{�[�h�̊Ǘ�
	class Keyboard : public Input {
		static const unsigned int BUFFER_SIZE = 256;

		std::map<std::string, int> table_;
		std::map<int, std::string> rev_table_;

		unsigned char buf_[BUFFER_SIZE];
		unsigned char oldbuf_[BUFFER_SIZE];

		unsigned int lookup(const std::string& key) const;

	public:
		Keyboard();
		~Keyboard() {}

		void update();
		std::string activeButton() const;
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
		static CriticalSection s_cs_;

		HWND hwnd_;
		long min_value_, max_value_;
		::CComPtr<::IDirectInputDevice8> dev_;
		GUID guid_;
		DIJOYSTATE2 state_, old_state_;

		Thread th_;
		bool continue_thread_;
		bool enabled_;

		static unsigned int joypadThread(void* data);

		static BOOL CALLBACK joypadEnum(const DIDEVICEINSTANCE* ddi, void* ref);

		void getDevice();
		void destroyDevice();

	public:
		Joypad(Window& window, long min_value = -1000L, long max_value = 1000L);
		~Joypad();

		void update();
		std::string activeButton() const;
		long axis(const std::string& key) const;
		bool up(const std::string& key) const;
		bool down(const std::string& key) const;
		bool pressed(const std::string& key) const;
		bool released(const std::string& key) const;
	};


	// ���͌n���̓���
	class InputBinder : public Input {
		// �o�C���h�̎��
		enum BindType {
			DGS_INPUT_BIND_BUTTON = 0x00,
			DGS_INPUT_BIND_AXIS_TO_BUTTON = 0x01,
			DGS_INPUT_BIND_AXIS = 0x10,
			DGS_INPUT_BIND_BUTTON_TO_AXIS = 0x11,
		};
		// �o�C���h���
		struct BindInfo {
			const Input& input;
			BindType type;
			const std::string src_key;
			union {
				struct {
					long up_val;
					long down_val;
					long axis;
				};
				struct {
					long threshold;
					bool upper_is_down;
					bool down;
					bool old_down;
				};
			};

			BindInfo(const Input& input_, BindType type_, const std::string& src_key_);
			BindInfo(BindInfo&& src);

			// �{�^�����ǂ���
			bool isButton() const;

			// �����ǂ���
			bool isAxis() const;
		};

		// �o�C���h���̃e�[�u��
		std::map<std::string, BindInfo> table_;

		// �w�肵�����O�Ńo�C���h����Ă���ꍇ�͗�O��Ԃ�
		void assertNotBinded(const std::string& key) const;

	public:
		InputBinder() {}
		~InputBinder() {}

		void update();
		std::string activeButton() const;
		long axis(const std::string& key) const;
		bool up(const std::string& key) const;
		bool down(const std::string& key) const;
		bool pressed(const std::string& key) const;
		bool released(const std::string& key) const;

		// �w�肵�����O�Ńo�C���h����Ă�����̂����邩�ǂ���
		bool isBinded(const std::string& key) const;

		// �o�C���h�̒ǉ�
		void bindButton(const std::string& key, const Input& input, const std::string src_key);
		void bindAxis(const std::string& key, const Input& input, const std::string src_key);
		void bindAxisToButton(const std::string& key, const Input& input, const std::string src_key, long threshold, bool upper_is_down);
		void bindButtonToAxis(const std::string& key, const Input& input, const std::string src_key, long up_val, long down_val);

		// �o�C���h�̍폜
		void clear();
		void unbind(const std::string& key);
	};
}
