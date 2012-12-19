// Input.cpp
#include "Input.h"

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
	// �ȑO�̏��̑ޔ�
	old_p_ = p_;
	old_lb_ = lb_;
	old_rb_ = rb_;
	old_mb_ = mb_;

	// �X�N���[�����W�ŃJ�[�\���ʒu���擾���ăN���C�A���g���W�ɕϊ�
	::GetCursorPos(&p_);
	::ScreenToClient(window_.rawHandle(), &p_);

	// �}�E�X��������Ă��邩�ǂ����̍X�V
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
	// �S�đ啶���ɂ���
	std::string k = key;
	std::transform(k.begin(), k.end(), k.begin(), std::toupper);

	char k0 = k[0];

	// �����񒷂�1��[0-9A-Z]�ł���ꍇ�͂��̕����R�[�h�𒼐ڕԂ�
	if (k.size() == 1 && (('0' <= k0 && k0 <= '9') || ('A' <= k0 && k0 <= 'Z')))
		return k0;

	// ����ȊO�̏ꍇ�̓e�[�u�����Q�Ƃ���
	auto it = table_.find(k);
	if (it != table_.end())
		return it->second;

	// �s���ȃR�[�h
	return 0;
}

DGS::Keyboard::Keyboard()
{
	// ���b�N�A�b�v�e�[�u���̃L�[�̌^
	typedef std::map<std::string, int>::value_type NODE;

	// �o�b�t�@�̏�����
	std::fill(buf_, buf_ + BUFFER_SIZE, 0);
	std::fill(oldbuf_, oldbuf_ + BUFFER_SIZE, 0);

	// ���b�N�A�b�v�e�[�u���̏�����
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
	// ���O�̏��̈ړ�
	std::copy(buf_, buf_ + BUFFER_SIZE, oldbuf_);

	// �V�������̎擾
	::GetKeyboardState(buf_);
}

long DGS::Keyboard::axis(const std::string& /*key*/) const
{
	// �Ή����鎲�͂Ȃ�
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
	Joypad* joypad = static_cast<Joypad*>(ref);
		
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
	s_di_->EnumDevices(DI8DEVCLASS_GAMECTRL, joypadEnum, static_cast<void *>(this), DIEDFL_ATTACHEDONLY);
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

long DGS::Joypad::axis(const std::string& key) const
{
	if (key == "X") return state_.lX;
	if (key == "Y") return state_.lY;
	return 0L;
}

/* NOTE:
 * �ȉ��̊֐��ł́A�����񂩂琔�l�ɕϊ�����ۂ� std::atoi() ��p����B
 * std::string ���g���Ă������o�b�t�@�I�[�o�[�t���[��(���܂�)�l����ꂸ�A
 * �܂��������Ȃ����l�͒���̏����Œe�����̂ŁA
 * ���p��͂Ƃ肠�������S�ł���B
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
