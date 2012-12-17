// Joypad.cpp
#include "Joypad.h"

#include <cstring>
#include "Game.h"

namespace {
	// デバイスの取得間隔
	unsigned int DEVICE_RETRIEVE_INTERVAL = 4;

	// ボタンの最大数
	unsigned int BUTTON_NUM_MAX = 64;
}

// DirectInput オブジェクト
::CComPtr<::IDirectInput8A> DGS::Joypad::s_di_;

// 既に取得済みのデバイスの GUID
std::list<GUID> DGS::Joypad::s_guids_;

// ジョイパッドの列挙
BOOL CALLBACK DGS::Joypad::joypadEnum(const DIDEVICEINSTANCE* ddi, void* ref)
{
	Joypad* joypad = reinterpret_cast<Joypad*>(ref);
		
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
bool DGS::Joypad::getDevice()
{
	// デバイスを列挙して取得
	s_di_->EnumDevices(DI8DEVCLASS_GAMECTRL, joypadEnum, reinterpret_cast<void *>(this), DIEDFL_ATTACHEDONLY);
	if (!dev_.p)
		return false;

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

	return true;

	// エラー処理
ERR:
	dev_.Release();
	return false;
}

// デバイスの破棄
void DGS::Joypad::destroyDevice()
{
	if (dev_.p) {
		dev_->Unacquire();
		dev_.Release();

		// 登録していた GUID の削除
		auto it = s_guids_.begin();
		while (*it != guid_)
			++it;
		s_guids_.erase(it);
	}
}

// c-tor
DGS::Joypad::Joypad(Window& window, long min_value, long max_value)
	: hwnd_(window.rawHandle())
	, min_value_(min_value)
	, max_value_(max_value)
	, frame_(0)
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

	getDevice();
}

// d-tor
DGS::Joypad::~Joypad()
{
	destroyDevice();
}

// 情報の更新
void DGS::Joypad::update()
{
	// 今回の更新でデバイスの再取得を試すかどうか
	// 4秒ごとに試すことにする
	bool reset = (frame_ % (Game::getCurrentObject().frameRate() * DEVICE_RETRIEVE_INTERVAL) == 0);
	++frame_;

	// 前の情報をコピー
	old_state_ = state_;

	bool enabled;
	if (dev_.p) {
		// 現在の情報を取得
		enabled = true;
		::HRESULT hr = dev_->Poll();
		if (FAILED(hr)) {
			// おそらくデバイスロスト
			// 取得待機にする
			destroyDevice();
			enabled = false;
		}
	} else {
		// デバイスの再取得を試す
		if (reset)
			enabled = getDevice();
		else
			enabled = false;
	}

	// 情報の更新
	if (enabled)
		dev_->GetDeviceState(sizeof(state_), &state_);
	else
		std::memset(&state_, 0, sizeof(state_));
}

// ボタンが上がっているかどうか
bool DGS::Joypad::up(unsigned int index) const
{
	if (index >= BUTTON_NUM_MAX)
		return false;
	return !(state_.rgbButtons[index] & 0x80);
}

// ボタンが下がっているかどうか
bool DGS::Joypad::down(unsigned int index) const
{
	if (index >= BUTTON_NUM_MAX)
		return false;
	return !!(state_.rgbButtons[index] & 0x80);
}

// ボタンが今押されたかどうか
bool DGS::Joypad::pressed(unsigned int index) const
{
	if (index >= BUTTON_NUM_MAX)
		return false;
	return (!!(state_.rgbButtons[index] & 0x80) && !(old_state_.rgbButtons[index] & 0x80));
}

// ボタンが今離されたかどうか
bool DGS::Joypad::released(unsigned int index) const
{
	if (index >= BUTTON_NUM_MAX)
		return false;
	return (!(state_.rgbButtons[index] & 0x80) && !!(old_state_.rgbButtons[index] & 0x80));
}
