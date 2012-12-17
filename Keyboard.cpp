// Keyboard.cpp
#include "Keyboard.h"

#include <algorithm>
#include <cctype>
#include <windows.h>

// キー名に対応するバッファの位置を求める
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

// c-tor
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

// 情報の更新
void DGS::Keyboard::update()
{
	// 直前の情報の移動
	std::copy(buf_, buf_ + BUFFER_SIZE, oldbuf_);

	// 新しい情報の取得
	::GetKeyboardState(buf_);
}

// キーが上がっているかどうか
bool  DGS::Keyboard::up(const std::string& key) const
{
	return !(buf_[lookup(key)] & 0x80);
}

// キーが下がっているかどうか
bool DGS::Keyboard::down(const std::string& key) const
{
	return !!(buf_[lookup(key)] & 0x80);
}

// キーがこのフレームで押されたかどうか
bool DGS::Keyboard::pressed(const std::string& key) const
{
	auto code = lookup(key);
	return (!!(buf_[code] & 0x80) && !(oldbuf_[code] & 0x80));
}

// キーがこのフレームで離されたかどうか
bool DGS::Keyboard::released(const std::string& key) const
{
	auto code = lookup(key);
	return (!(buf_[code] & 0x80) && !!(oldbuf_[code] & 0x80));
}
