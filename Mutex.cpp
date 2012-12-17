// Mutex.cpp
#include "Mutex.h"

#include "Game.h"

// 共通の初期化処理
void DGS::Mutex::open()
{
	// ミューテックス生成
	mutex_ = ::CreateMutexA(NULL, TRUE, name_.c_str());
	Win32ErrorCode err;

	if (!mutex_)
		throw Win32Exception(err);

	// 既に取得されているかどうか
	duplicated_ = (err == ERROR_ALREADY_EXISTS);
}

// c-tor
// ゲーム識別子でミューテックスを生成
DGS::Mutex::Mutex()
	: name_(Game::getCurrentObject().identifier())
{
	open();
}

// c-tor
// 名前を指定してミューテックスを生成
DGS::Mutex::Mutex(const std::string& name)
	: name_(name)
{
	// ゲーム識別子を使用しない場合でも Game の初期化が必要なようにする。
	Game::getCurrentObject();

	open();
}

// d-tor
DGS::Mutex::~Mutex()
{
	if (mutex_)
		::ReleaseMutex(mutex_);
}
