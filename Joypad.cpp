// Joypad.cpp
#include "Joypad.h"

#include <cstring>
#include "Game.h"

namespace {
	// �f�o�C�X�̎擾�Ԋu
	unsigned int DEVICE_RETRIEVE_INTERVAL = 4;

	// �{�^���̍ő吔
	unsigned int BUTTON_NUM_MAX = 64;
}

// DirectInput �I�u�W�F�N�g
::CComPtr<::IDirectInput8A> DGS::Joypad::s_di_;

// ���Ɏ擾�ς݂̃f�o�C�X�� GUID
std::list<GUID> DGS::Joypad::s_guids_;

// �W���C�p�b�h�̗�
BOOL CALLBACK DGS::Joypad::joypadEnum(const DIDEVICEINSTANCE* ddi, void* ref)
{
	Joypad* joypad = reinterpret_cast<Joypad*>(ref);
		
	// �����W���C�p�b�h�̃C���X�^���X�͐������Ȃ�
	for (auto it = s_guids_.begin(); it != s_guids_.end(); ++it) {
		if (ddi->guidInstance == *it)
			return DIENUM_CONTINUE;
	}

	// �C���X�^���X�̐���
	::IDirectInputDevice8A* dev;
	HRESULT hr = s_di_->CreateDevice(ddi->guidInstance, &dev, nullptr);

	if (FAILED(hr))
		return DIENUM_CONTINUE;

	joypad->dev_.Attach(dev);

	// GUID�̃R�s�[
	s_guids_.push_back(ddi->guidInstance);
	joypad->guid_ = ddi->guidInstance;

	return DIENUM_STOP;
}

// �f�o�C�X�̎擾
bool DGS::Joypad::getDevice()
{
	// �f�o�C�X��񋓂��Ď擾
	s_di_->EnumDevices(DI8DEVCLASS_GAMECTRL, joypadEnum, reinterpret_cast<void *>(this), DIEDFL_ATTACHEDONLY);
	if (!dev_.p)
		return false;

	// �f�[�^�t�H�[�}�b�g�̐ݒ�
	::HRESULT hr = dev_->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
		goto ERR;

	// �������x���̐ݒ�
	hr = dev_->SetCooperativeLevel(hwnd_, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hr))
		goto ERR;

	// �e���̐ݒ�
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

	// ���擾�̊J�n
	hr = dev_->Acquire();
	if (FAILED(hr))
		goto ERR;

	return true;

	// �G���[����
ERR:
	dev_.Release();
	return false;
}

// �f�o�C�X�̔j��
void DGS::Joypad::destroyDevice()
{
	if (dev_.p) {
		dev_->Unacquire();
		dev_.Release();

		// �o�^���Ă��� GUID �̍폜
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
	// DirectInput �I�u�W�F�N�g�̐���
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

// ���̍X�V
void DGS::Joypad::update()
{
	// ����̍X�V�Ńf�o�C�X�̍Ď擾���������ǂ���
	// 4�b���ƂɎ������Ƃɂ���
	bool reset = (frame_ % (Game::getCurrentObject().frameRate() * DEVICE_RETRIEVE_INTERVAL) == 0);
	++frame_;

	// �O�̏����R�s�[
	old_state_ = state_;

	bool enabled;
	if (dev_.p) {
		// ���݂̏����擾
		enabled = true;
		::HRESULT hr = dev_->Poll();
		if (FAILED(hr)) {
			// �����炭�f�o�C�X���X�g
			// �擾�ҋ@�ɂ���
			destroyDevice();
			enabled = false;
		}
	} else {
		// �f�o�C�X�̍Ď擾������
		if (reset)
			enabled = getDevice();
		else
			enabled = false;
	}

	// ���̍X�V
	if (enabled)
		dev_->GetDeviceState(sizeof(state_), &state_);
	else
		std::memset(&state_, 0, sizeof(state_));
}

// �{�^�����オ���Ă��邩�ǂ���
bool DGS::Joypad::up(unsigned int index) const
{
	if (index >= BUTTON_NUM_MAX)
		return false;
	return !(state_.rgbButtons[index] & 0x80);
}

// �{�^�����������Ă��邩�ǂ���
bool DGS::Joypad::down(unsigned int index) const
{
	if (index >= BUTTON_NUM_MAX)
		return false;
	return !!(state_.rgbButtons[index] & 0x80);
}

// �{�^�����������ꂽ���ǂ���
bool DGS::Joypad::pressed(unsigned int index) const
{
	if (index >= BUTTON_NUM_MAX)
		return false;
	return (!!(state_.rgbButtons[index] & 0x80) && !(old_state_.rgbButtons[index] & 0x80));
}

// �{�^�����������ꂽ���ǂ���
bool DGS::Joypad::released(unsigned int index) const
{
	if (index >= BUTTON_NUM_MAX)
		return false;
	return (!(state_.rgbButtons[index] & 0x80) && !!(old_state_.rgbButtons[index] & 0x80));
}
