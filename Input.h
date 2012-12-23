// Input.h
// 入力系統の管理
#pragma once

// DirectInputのバージョン指定
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

	// 入力系統の基底クラス
	class Input : private Uncopyable {
	public:
		Input() {}
		virtual ~Input() {}
		
		// 情報の更新
		virtual void update() = 0;

		// どのボタンが押されているか
		// 複数のボタンが押されている場合は最初に見つかったものを返す
		// 何も押されていない場合は空文字列を返す
		virtual std::string activeButton() const = 0;

		// 座標値の取得
		virtual long axis(const std::string& key) const = 0;

		// ボタンが上がっているかどうか
		virtual bool up(const std::string& key) const = 0;

		// ボタンが下がっているかどうか
		virtual bool down(const std::string& key) const = 0;

		// ボタンが今押されたかどうか
		virtual bool pressed(const std::string& key) const = 0;

		// ボタンが今離されたかどうか
		virtual bool released(const std::string& key) const = 0;
	};

	// マウスの管理
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

	// キーボードの管理
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

	// ジョイパッドの管理
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


	// 入力系統の統合
	class InputBinder : public Input {
		// バインドの種類
		enum BindType {
			DGS_INPUT_BIND_BUTTON = 0x00,
			DGS_INPUT_BIND_AXIS_TO_BUTTON = 0x01,
			DGS_INPUT_BIND_AXIS = 0x10,
			DGS_INPUT_BIND_BUTTON_TO_AXIS = 0x11,
		};
		// バインド情報
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

			// ボタンかどうか
			bool isButton() const;

			// 軸かどうか
			bool isAxis() const;
		};

		// バインド情報のテーブル
		std::map<std::string, BindInfo> table_;

		// 指定した名前でバインドされている場合は例外を返す
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

		// 指定した名前でバインドされているものがあるかどうか
		bool isBinded(const std::string& key) const;

		// バインドの追加
		void bindButton(const std::string& key, const Input& input, const std::string src_key);
		void bindAxis(const std::string& key, const Input& input, const std::string src_key);
		void bindAxisToButton(const std::string& key, const Input& input, const std::string src_key, long threshold, bool upper_is_down);
		void bindButtonToAxis(const std::string& key, const Input& input, const std::string src_key, long up_val, long down_val);

		// バインドの削除
		void clear();
		void unbind(const std::string& key);
	};
}
