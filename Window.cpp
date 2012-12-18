// Window.cpp
#include "Window.h"

#include "Game.h"
#include "utility.h"

/* NOTE:
 *   WN_CLOSE のハンドラでプロパティリストの削除を行うので、
 *   外部から直接 ::DestroyWindow() を呼び出して
 *   ウィンドウハンドルを削除するのは禁忌である。
 *   必ず ::SendMessage(hwnd, WM_CLOSE, 0, 0) にすること。
 */

namespace {
	const char* WINDOWCLASS_NAME = "DGS_WINDOWCLASS";
}

// Windowオブジェクトの数
unsigned int DGS::Window::s_object_num_ = 0;

// ウィンドウクラスの登録
bool DGS::Window::registerWindowClass()
{
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEXA);

	wcex.style         = CS_CLASSDC;
	wcex.lpfnWndProc   = DGS::Window::windowProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = Game::getCurrentObject().instanceHandle();
	wcex.hIcon         = NULL;
	wcex.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = WINDOWCLASS_NAME;
	wcex.hIconSm       = NULL;

	ATOM ret = ::RegisterClassExA(&wcex);
	return !!ret;
}

// ウィンドウプロシージャ
LRESULT CALLBACK DGS::Window::windowProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	const char* PROP_NAME = "DGS_WINDOW";

	if (WM_CREATE == message) {
		// プロパティリストへキーを追加

		// prop[PROP_NAME] : Window オブジェクトへのポインタ
		::CREATESTRUCT* cs = reinterpret_cast<::CREATESTRUCT*>(lp);
		BOOL ret = ::SetPropA(hwnd, PROP_NAME, reinterpret_cast<::HANDLE>(cs->lpCreateParams));
		if (!ret) return -1;

		// prop[ゲーム識別子] : ウィンドウハンドルがこのゲームのものかどうかの判別用
		ret = ::SetPropA(hwnd, Game::getCurrentObject().identifier().c_str(), reinterpret_cast<::HANDLE>(1));
		if (!ret) {
			::RemovePropA(hwnd, PROP_NAME);
			return -1;
		}

		// マウスボタンの押下判定を初期化
		Window* window = reinterpret_cast<Window*>(cs->lpCreateParams);
		window->lb_down_ = false;
		window->rb_down_ = false;
		window->mb_down_ = false;
		return 0;
	}
	
	// プロパティリストから Window オブジェクトのポインタを取得。
	// NOTE:
	//   WM_CREATE より前や WM_CLOSE より後の処理では NULL になる
	Window* window = reinterpret_cast<Window*>(::GetPropA(hwnd, PROP_NAME));

	switch (message) {
	case WM_SYSCOMMAND:
		switch (wp & 0xfff0) {
		//case SC_CLOSE: return 0;        // WM_CLOSEを 発生させる操作 (×ボタンなど) の抑制
		case SC_SCREENSAVE: return 1;   // スクリーンセーバーの抑制
		case SC_MONITORPOWER: return 1; // モニタの自動パワーオフの抑制
		default:
			return ::DefWindowProcA(hwnd, message, wp, lp);
		}

	case WM_CLOSE:
		// Window オブジェクトからウィンドウハンドルを消す
		// SC_CLOSE からの WM_CLOSE イベントは Window::Close() を経由しないので
		// ここで関連付けの解除を行う。
		window->hwnd_ = NULL;

		// プロパティリストからキーを削除
		// (WM_DESTROY より前に作業しなければならない)
		::RemovePropA(hwnd, PROP_NAME);
		::RemovePropA(hwnd, Game::getCurrentObject().identifier().c_str());

		// ウィンドウ削除
		::DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		// メッセージループを抜ける
		::PostQuitMessage(0);
		break;

		// マウスボタンが押されたときの処理
	case WM_LBUTTONDOWN:
		window->lb_down_ = true;
		::SetCapture(hwnd);
		break;
	case WM_RBUTTONDOWN:
		window->rb_down_ = true;
		::SetCapture(hwnd);
		break;
	case WM_MBUTTONDOWN:
		window->mb_down_ = true;
		::SetCapture(hwnd);
		break;

		// マウスボタンが離されたときの処理
	case WM_LBUTTONUP:
		window->lb_down_ = false;
		::ReleaseCapture();
		break;
	case WM_RBUTTONUP:
		window->rb_down_ = false;
		::ReleaseCapture();
		break;
	case WM_MBUTTONUP:
		window->mb_down_ = false;
		::ReleaseCapture();
		break;

#if defined(DEBUG) || defined(_DEBUG)
		// 画面サイズの表示
	case WM_SIZE:
		{
			char s[64];
			::wsprintfA(s, "WM_SIZE: %d, %d\n", LOWORD(lp), HIWORD(lp));
			::OutputDebugStringA(s);
		}
		break;
#endif

	default:
		return ::DefWindowProcA(hwnd, message, wp, lp);
	}

	return 0;
}

// c-tor
DGS::Window::Window(const std::string& title)
	: hwnd_(NULL)
	, title_(title)
{
	// Game の初期化が必要
	Game::getCurrentObject();

	// 1個目の Window オブジェクトであれば
	// ウィンドウクラスの登録を行う。
	if (0 == s_object_num_)
		if (!registerWindowClass())
			throw Win32Exception();

	++s_object_num_;
}

// d-tor
DGS::Window::~Window()
{
	// ウィンドウハンドルが残っていれば削除する
	if (isOpen())
		close();

	--s_object_num_;

	// 最後の Window オブジェクトであれば
	// ウィンドウクラスの破棄を行う。
	if (0 == s_object_num_)
		::UnregisterClassA(WINDOWCLASS_NAME, Game::getCurrentObject().instanceHandle());
}

// ウィンドウを生成して表示する
void DGS::Window::open()
{
	if (isOpen())
		throw Exception("既にウィンドウが開かれています。");

	// ウィンドウハンドルの生成
	hwnd_ = ::CreateWindowA(
		WINDOWCLASS_NAME,
		title_.c_str(),
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL,
		NULL,
		NULL, // Game::getInstanceHandle(),
		this);

	if (!hwnd_)
		throw Win32Exception();

	try {
		// 表示
		::ShowWindow(hwnd_, SW_SHOW);
		if (!::UpdateWindow(hwnd_))
			throw Win32Exception();

		// Win32 APIによる再描画を発生させなくする。
		if (!::ValidateRect(hwnd_, NULL))
			throw Win32Exception();
	} catch (...) {
		// 処理に失敗した場合、ウィンドウを閉じて例外を上へ投げる。
		close();
		throw;
	}
}

// ウィンドウを閉じて破棄する
void DGS::Window::close()
{
	assertOpen();

	// WM_CLOSE をウィンドウプロシージャに送る。
	// 関連付けの解除はウィンドウプロシージャで行われる。
	::SendMessageA(hwnd_, WM_CLOSE, 0, 0);
}

// ウィンドウが開かれているかどうかを調べる
void DGS::Window::assertOpen() const
{
	if (!isOpen())
		throw Exception("ウィンドウが開かれていません。");
}

// クライアント領域のサイズを設定する
void DGS::Window::setClientSize(unsigned int width, unsigned int height)
{
	assertOpen();

	// ウィンドウ全体のサイズを計算する
	::RECT rc, rw;
	::GetClientRect(hwnd_, &rc);
	::GetWindowRect(hwnd_, &rw);
	unsigned int w = (rw.right - rw.left) - (rc.right - rc.left) + width;
	unsigned int h = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

	// 適用
	if (!::SetWindowPos(hwnd_, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER))
		throw Win32Exception();
}

// ウィンドウ全体のサイズを設定する
void DGS::Window::setWindowSize(unsigned int width, unsigned int height)
{
	assertOpen();

	// 適用
	if (!::SetWindowPos(hwnd_, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER))
		throw Win32Exception();
}
