// Input.cpp
#include "Input.h"

#include <cassert>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include "Game.h"


/* class DGS::Mouse */

DGS::Mouse::Mouse(Window& window)
	: window_(window)
	, lb_(false)
	, rb_(false)
	, mb_(false)
	, old_lb_(false)
	, old_rb_(false)
	, old_mb_(false)
{
	p_.x = p_.y = 0L;
	old_p_.x = old_p_.y = 0L;
}

void DGS::Mouse::update()
{
	// 以前の情報の退避
	old_p_ = p_;
	old_lb_ = lb_;
	old_rb_ = rb_;
	old_mb_ = mb_;

	// スクリーン座標でカーソル位置を取得してクライアント座標に変換
	::GetCursorPos(&p_);
	::ScreenToClient(window_.rawHandle(), &p_);

	// マウスが押されているかどうかの更新
	lb_ = window_.mouseDownL();
	rb_ = window_.mouseDownR();
	mb_ = window_.mouseDownM();
}

long DGS::Mouse::axis(const std::string& key) const
{
	if (key == "X") return p_.x - old_p_.x;
	if (key == "Y") return p_.y - old_p_.y;
	if (key == "PX") return p_.x;
	if (key == "PY") return p_.y;
	return 0L;
}

bool  DGS::Mouse::up(const std::string& key) const
{
	if (key == "LEFT") return !lb_;
	if (key == "RIGHT") return !rb_;
	if (key == "MIDDLE") return !mb_;
	return false;
}

bool DGS::Mouse::down(const std::string& key) const
{
	if (key == "LEFT") return lb_;
	if (key == "RIGHT") return rb_;
	if (key == "MIDDLE") return mb_;
	return false;
}

bool DGS::Mouse::pressed(const std::string& key) const
{
	if (key == "LEFT") return (lb_ && !old_lb_);
	if (key == "RIGHT") return (rb_ && !old_rb_);
	if (key == "MIDDLE") return (mb_ && !old_mb_);
	return false;
}

bool DGS::Mouse::released(const std::string& key) const
{
	if (key == "LEFT") return (!lb_ && old_lb_);
	if (key == "RIGHT") return (!rb_ && old_rb_);
	if (key == "MIDDLE") return (!mb_ && old_mb_);
	return false;
}



/* class DGS::Keyboard */

unsigned int DGS::Keyboard::lookup(const std::string& key) const
{
	// 全て大文字にする
	std::string k = key;
	std::transform(k.begin(), k.end(), k.begin(), std::toupper);

	char k0 = k[0];

	// 文字列長が1で[0-9A-Z]である場合はその文字コードを直接返す
	if (k.size() == 1 && (('0' <= k0 && k0 <= '9') || ('A' <= k0 && k0 <= 'Z')))
		return k0;

	// それ以外の場合はテーブルを参照する
	auto it = table_.find(k);
	if (it != table_.end())
		return it->second;

	// 不明なコード
	return 0;
}

DGS::Keyboard::Keyboard()
{
	// ルックアップテーブルのキーの型
	typedef std::map<std::string, int>::value_type NODE;

	// バッファの初期化
	std::fill(buf_, buf_ + BUFFER_SIZE, 0);
	std::fill(oldbuf_, oldbuf_ + BUFFER_SIZE, 0);

	// ルックアップテーブルの初期化
	table_.insert(NODE("UP", VK_UP));
	table_.insert(NODE("DOWN", VK_DOWN));
	table_.insert(NODE("LEFT", VK_LEFT));
	table_.insert(NODE("RIGHT", VK_RIGHT));

	table_.insert(NODE("SHIFT", VK_SHIFT));
	table_.insert(NODE("LSHIFT", VK_LSHIFT));
	table_.insert(NODE("RSHIFT", VK_RSHIFT));
	table_.insert(NODE("CONTROL", VK_CONTROL));
	table_.insert(NODE("LCONTROL", VK_LCONTROL));
	table_.insert(NODE("RCONTROL", VK_RCONTROL));
	table_.insert(NODE("CTRL", VK_CONTROL));
	table_.insert(NODE("LCTRL", VK_LCONTROL));
	table_.insert(NODE("RCTRL", VK_RCONTROL));
	table_.insert(NODE("ALT", VK_MENU));

	table_.insert(NODE("ENTER", VK_RETURN));
	table_.insert(NODE("RETURN", VK_RETURN));
	table_.insert(NODE("ESCAPE", VK_ESCAPE));
	table_.insert(NODE("ESC", VK_ESCAPE));
	table_.insert(NODE("INSERT", VK_INSERT));
	table_.insert(NODE("DELETE", VK_DELETE));
	table_.insert(NODE("HOME", VK_HOME));
	table_.insert(NODE("END", VK_END));
	table_.insert(NODE("BACK", VK_BACK));
	table_.insert(NODE("BACKSPACE", VK_BACK));
	table_.insert(NODE("TAB", VK_TAB));
	table_.insert(NODE("SPACE", VK_SPACE));
	table_.insert(NODE(" ", VK_SPACE));

	table_.insert(NODE("!", '1'));
	table_.insert(NODE("\"", '2'));
	table_.insert(NODE("#", '3'));
	table_.insert(NODE("$", '4'));
	table_.insert(NODE("%", '5'));
	table_.insert(NODE("&", '6'));
	table_.insert(NODE("'", '7'));
	table_.insert(NODE("(", '8'));
	table_.insert(NODE(")", '9'));

	table_.insert(NODE(":", VK_OEM_1));
	table_.insert(NODE("*", VK_OEM_1));
	table_.insert(NODE(";", VK_OEM_PLUS));
	table_.insert(NODE("+", VK_OEM_PLUS));
	table_.insert(NODE(",", VK_OEM_COMMA));
	table_.insert(NODE("<", VK_OEM_COMMA));
	table_.insert(NODE("-", VK_OEM_MINUS));
	table_.insert(NODE("=", VK_OEM_MINUS));
	table_.insert(NODE(".", VK_OEM_PERIOD));
	table_.insert(NODE(">", VK_OEM_PERIOD));
	table_.insert(NODE("/", VK_OEM_2));
	table_.insert(NODE("?", VK_OEM_2));
	table_.insert(NODE("@", VK_OEM_3));
	table_.insert(NODE("`", VK_OEM_3));
	table_.insert(NODE("[", VK_OEM_4));
	table_.insert(NODE("{", VK_OEM_4));
	table_.insert(NODE("\\", VK_OEM_5));
	table_.insert(NODE("|", VK_OEM_5));
	table_.insert(NODE("]", VK_OEM_6));
	table_.insert(NODE("}", VK_OEM_6));
	table_.insert(NODE("^", VK_OEM_7));
	table_.insert(NODE("~", VK_OEM_7));
	table_.insert(NODE("_", VK_OEM_102));

	table_.insert(NODE("NUM0", VK_NUMPAD0));
	table_.insert(NODE("NUM1", VK_NUMPAD1));
	table_.insert(NODE("NUM2", VK_NUMPAD2));
	table_.insert(NODE("NUM3", VK_NUMPAD3));
	table_.insert(NODE("NUM4", VK_NUMPAD4));
	table_.insert(NODE("NUM5", VK_NUMPAD5));
	table_.insert(NODE("NUM6", VK_NUMPAD6));
	table_.insert(NODE("NUM7", VK_NUMPAD7));
	table_.insert(NODE("NUM8", VK_NUMPAD8));
	table_.insert(NODE("NUM9", VK_NUMPAD9));
	table_.insert(NODE("NUM*", VK_MULTIPLY));
	table_.insert(NODE("NUM+", VK_ADD));
	table_.insert(NODE("NUM,", VK_SEPARATOR));
	table_.insert(NODE("NUM-", VK_SUBTRACT));
	table_.insert(NODE("NUM.", VK_DECIMAL));
	table_.insert(NODE("NUM/", VK_DIVIDE));

	table_.insert(NODE("F1", VK_F1));
	table_.insert(NODE("F2", VK_F2));
	table_.insert(NODE("F3", VK_F3));
	table_.insert(NODE("F4", VK_F4));
	table_.insert(NODE("F5", VK_F5));
	table_.insert(NODE("F6", VK_F6));
	table_.insert(NODE("F7", VK_F7));
	table_.insert(NODE("F8", VK_F8));
	table_.insert(NODE("F9", VK_F9));
	table_.insert(NODE("F10", VK_F10));
	table_.insert(NODE("F11", VK_F11));
	table_.insert(NODE("F12", VK_F12));
	table_.insert(NODE("F13", VK_F13));
	table_.insert(NODE("F14", VK_F14));
	table_.insert(NODE("F15", VK_F15));
	table_.insert(NODE("F16", VK_F16));
	table_.insert(NODE("F17", VK_F17));
	table_.insert(NODE("F18", VK_F18));
	table_.insert(NODE("F19", VK_F19));
	table_.insert(NODE("F20", VK_F20));
	table_.insert(NODE("F21", VK_F21));
	table_.insert(NODE("F22", VK_F22));
	table_.insert(NODE("F23", VK_F23));
	table_.insert(NODE("F24", VK_F24));

	table_.insert(NODE("LBUTTON", VK_LBUTTON));
	table_.insert(NODE("RBUTTON", VK_RBUTTON));
	table_.insert(NODE("MBUTTON", VK_MBUTTON));
}

void DGS::Keyboard::update()
{
	// 直前の情報の移動
	std::copy(buf_, buf_ + BUFFER_SIZE, oldbuf_);

	// 新しい情報の取得
	::GetKeyboardState(buf_);
}

long DGS::Keyboard::axis(const std::string& /*key*/) const
{
	// 対応する軸はない
	return 0L;
}

bool  DGS::Keyboard::up(const std::string& key) const
{
	return !(buf_[lookup(key)] & 0x80);
}

bool DGS::Keyboard::down(const std::string& key) const
{
	return !!(buf_[lookup(key)] & 0x80);
}

bool DGS::Keyboard::pressed(const std::string& key) const
{
	auto code = lookup(key);
	return (!!(buf_[code] & 0x80) && !(oldbuf_[code] & 0x80));
}

bool DGS::Keyboard::released(const std::string& key) const
{
	auto code = lookup(key);
	return (!(buf_[code] & 0x80) && !!(oldbuf_[code] & 0x80));
}



/* class DGS::Joypad */

namespace {
	// ボタンの最大数
	unsigned int BUTTON_NUM_MAX = 64;
}

// DirectInput オブジェクト
::CComPtr<::IDirectInput8A> DGS::Joypad::s_di_;

// 既に取得済みのデバイスの GUID
std::list<GUID> DGS::Joypad::s_guids_;

// クリティカルセクション
DGS::CriticalSection DGS::Joypad::s_cs_;

// ジョイパッドの取得を行うスレッド
unsigned int DGS::Joypad::joypadThread(void* data)
{
	Joypad* joypad = static_cast<Joypad*>(data);

	while (joypad->continue_thread_) {
		// デバイスがなければ取得を試みる
		if (!joypad->enabled_) {
			s_cs_.enter();
			joypad->getDevice();
			s_cs_.leave();
		}

		// 待機
		Thread::sleep(1000);
	}

	return 0;
}

// ジョイパッドの列挙
BOOL CALLBACK DGS::Joypad::joypadEnum(const DIDEVICEINSTANCE* ddi, void* ref)
{
	Joypad* joypad = static_cast<Joypad*>(ref);
		
	// 同じジョイパッドのインスタンスは生成しない
	for (auto it = s_guids_.begin(); it != s_guids_.end(); ++it) {
		if (ddi->guidInstance == *it)
			return DIENUM_CONTINUE;
	}

	// インスタンスの生成
	::IDirectInputDevice8A* dev;
	HRESULT hr = s_di_->CreateDevice(ddi->guidInstance, &dev, nullptr);

	if (FAILED(hr))
		return DIENUM_CONTINUE;

	joypad->dev_.Attach(dev);

	// GUIDのコピー
	s_guids_.push_back(ddi->guidInstance);
	joypad->guid_ = ddi->guidInstance;

	return DIENUM_STOP;
}

// デバイスの取得
void DGS::Joypad::getDevice()
{
	assert(!enabled_);

	// デバイスを列挙して取得
	s_di_->EnumDevices(DI8DEVCLASS_GAMECTRL, joypadEnum, static_cast<void *>(this), DIEDFL_ATTACHEDONLY);
	if (!dev_.p)
		return;

	// データフォーマットの設定
	::HRESULT hr = dev_->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
		goto ERR;

	// 協調レベルの設定
	hr = dev_->SetCooperativeLevel(hwnd_, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr))
		goto ERR;

	// 各軸の設定
	::DIPROPRANGE dipr;
	dipr.diph.dwSize = sizeof(dipr);
	dipr.diph.dwHeaderSize = sizeof(dipr.diph);
	dipr.diph.dwHow = DIPH_BYOFFSET;
	dipr.lMin = min_value_;
	dipr.lMax = max_value_;

	dipr.diph.dwObj = DIJOFS_X;
	hr = dev_->SetProperty(DIPROP_RANGE, &dipr.diph);
	if (FAILED(hr))
		goto ERR;

	dipr.diph.dwObj = DIJOFS_Y;
	hr = dev_->SetProperty(DIPROP_RANGE, &dipr.diph);
	if (FAILED(hr))
		goto ERR;

	// 情報取得の開始
	hr = dev_->Acquire();
	if (FAILED(hr))
		goto ERR;

	enabled_ = true;
	return;

	// エラー処理
ERR:
	dev_.Release();
	return;
}

// デバイスの破棄
void DGS::Joypad::destroyDevice()
{
	if (dev_.p) {
		dev_->Unacquire();
		dev_.Release();

		// 登録していた GUID の削除
		s_cs_.enter();
		auto it = s_guids_.begin();
		while (*it != guid_)
			++it;
		s_guids_.erase(it);
		s_cs_.leave();

		enabled_ = false;
	}
}

// c-tor
DGS::Joypad::Joypad(Window& window, long min_value, long max_value)
	: hwnd_(window.rawHandle())
	, min_value_(min_value)
	, max_value_(max_value)
	, th_(joypadThread, this, false)
	, continue_thread_(true)
	, enabled_(false)
{
	// DirectInput オブジェクトの生成
	if (!s_di_.p) {
		::IDirectInput8A* di;
		::HRESULT hr = ::DirectInput8Create(
			Game::getCurrentObject().instanceHandle(),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			reinterpret_cast<void**>(&di),
			nullptr);
		if (FAILED(hr))
			throw DirectXException(hr);
		s_di_.Attach(di);
	}

	// デバイス取得スレッドの起動
	th_.resume();
}

// d-tor
DGS::Joypad::~Joypad()
{
	// デバイス取得スレッドの停止
	continue_thread_ = false;
	th_.join();

	// デバイスの破棄
	destroyDevice();
}

// 情報の更新
void DGS::Joypad::update()
{
	// 前の情報をコピー
	old_state_ = state_;

	if (enabled_) {
		// 現在の情報を取得
		::HRESULT hr = dev_->Poll();
		if (FAILED(hr)) {
			// おそらくデバイスロスト
			// 取得待機にする
			destroyDevice();
		}
	}

	// 情報の更新
	if (enabled_)
		dev_->GetDeviceState(sizeof(state_), &state_);
	else
		std::memset(&state_, 0, sizeof(state_));
}

long DGS::Joypad::axis(const std::string& key) const
{
	if (key == "X") return state_.lX;
	if (key == "Y") return state_.lY;
	return 0L;
}

/* NOTE:
 * 以下の関数では、文字列から数値に変換する際に std::atoi() を用いる。
 * std::string を使っている限りバッファオーバーフローは(あまり)考えられず、
 * また正しくない数値は直後の条件で弾かれるので、
 * 実用上はとりあえず安全である。
 */

bool DGS::Joypad::up(const std::string& key) const
{
	unsigned int index = static_cast<unsigned int>(std::atoi(key.c_str()));
	if (index >= BUTTON_NUM_MAX)
		return false;
	return !(state_.rgbButtons[index] & 0x80);
}

bool DGS::Joypad::down(const std::string& key) const
{
	unsigned int index = static_cast<unsigned int>(std::atoi(key.c_str()));
	if (index >= BUTTON_NUM_MAX)
		return false;
	return !!(state_.rgbButtons[index] & 0x80);
}

bool DGS::Joypad::pressed(const std::string& key) const
{
	unsigned int index = static_cast<unsigned int>(std::atoi(key.c_str()));
	if (index >= BUTTON_NUM_MAX)
		return false;
	return (!!(state_.rgbButtons[index] & 0x80) && !(old_state_.rgbButtons[index] & 0x80));
}

bool DGS::Joypad::released(const std::string& key) const
{
	unsigned int index = static_cast<unsigned int>(std::atoi(key.c_str()));
	if (index >= BUTTON_NUM_MAX)
		return false;
	return (!(state_.rgbButtons[index] & 0x80) && !!(old_state_.rgbButtons[index] & 0x80));
}
