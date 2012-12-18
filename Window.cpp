// Window.cpp
#include "Window.h"

#include "Game.h"
#include "utility.h"

/* NOTE:
 *   WN_CLOSE �̃n���h���Ńv���p�e�B���X�g�̍폜���s���̂ŁA
 *   �O�����璼�� ::DestroyWindow() ���Ăяo����
 *   �E�B���h�E�n���h�����폜����̂͋֊��ł���B
 *   �K�� ::SendMessage(hwnd, WM_CLOSE, 0, 0) �ɂ��邱�ƁB
 */

namespace {
	const char* WINDOWCLASS_NAME = "DGS_WINDOWCLASS";
}

// Window�I�u�W�F�N�g�̐�
unsigned int DGS::Window::s_object_num_ = 0;

// �E�B���h�E�N���X�̓o�^
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

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK DGS::Window::windowProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	const char* PROP_NAME = "DGS_WINDOW";

	if (WM_CREATE == message) {
		// �v���p�e�B���X�g�փL�[��ǉ�

		// prop[PROP_NAME] : Window �I�u�W�F�N�g�ւ̃|�C���^
		::CREATESTRUCT* cs = reinterpret_cast<::CREATESTRUCT*>(lp);
		BOOL ret = ::SetPropA(hwnd, PROP_NAME, reinterpret_cast<::HANDLE>(cs->lpCreateParams));
		if (!ret) return -1;

		// prop[�Q�[�����ʎq] : �E�B���h�E�n���h�������̃Q�[���̂��̂��ǂ����̔��ʗp
		ret = ::SetPropA(hwnd, Game::getCurrentObject().identifier().c_str(), reinterpret_cast<::HANDLE>(1));
		if (!ret) {
			::RemovePropA(hwnd, PROP_NAME);
			return -1;
		}

		// �}�E�X�{�^���̉��������������
		Window* window = reinterpret_cast<Window*>(cs->lpCreateParams);
		window->lb_down_ = false;
		window->rb_down_ = false;
		window->mb_down_ = false;
		return 0;
	}
	
	// �v���p�e�B���X�g���� Window �I�u�W�F�N�g�̃|�C���^���擾�B
	// NOTE:
	//   WM_CREATE ���O�� WM_CLOSE ����̏����ł� NULL �ɂȂ�
	Window* window = reinterpret_cast<Window*>(::GetPropA(hwnd, PROP_NAME));

	switch (message) {
	case WM_SYSCOMMAND:
		switch (wp & 0xfff0) {
		//case SC_CLOSE: return 0;        // WM_CLOSE�� ���������鑀�� (�~�{�^���Ȃ�) �̗}��
		case SC_SCREENSAVE: return 1;   // �X�N���[���Z�[�o�[�̗}��
		case SC_MONITORPOWER: return 1; // ���j�^�̎����p���[�I�t�̗}��
		default:
			return ::DefWindowProcA(hwnd, message, wp, lp);
		}

	case WM_CLOSE:
		// Window �I�u�W�F�N�g����E�B���h�E�n���h��������
		// SC_CLOSE ����� WM_CLOSE �C�x���g�� Window::Close() ���o�R���Ȃ��̂�
		// �����Ŋ֘A�t���̉������s���B
		window->hwnd_ = NULL;

		// �v���p�e�B���X�g����L�[���폜
		// (WM_DESTROY ���O�ɍ�Ƃ��Ȃ���΂Ȃ�Ȃ�)
		::RemovePropA(hwnd, PROP_NAME);
		::RemovePropA(hwnd, Game::getCurrentObject().identifier().c_str());

		// �E�B���h�E�폜
		::DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		// ���b�Z�[�W���[�v�𔲂���
		::PostQuitMessage(0);
		break;

		// �}�E�X�{�^���������ꂽ�Ƃ��̏���
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

		// �}�E�X�{�^���������ꂽ�Ƃ��̏���
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
		// ��ʃT�C�Y�̕\��
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
	// Game �̏��������K�v
	Game::getCurrentObject();

	// 1�ڂ� Window �I�u�W�F�N�g�ł����
	// �E�B���h�E�N���X�̓o�^���s���B
	if (0 == s_object_num_)
		if (!registerWindowClass())
			throw Win32Exception();

	++s_object_num_;
}

// d-tor
DGS::Window::~Window()
{
	// �E�B���h�E�n���h�����c���Ă���΍폜����
	if (isOpen())
		close();

	--s_object_num_;

	// �Ō�� Window �I�u�W�F�N�g�ł����
	// �E�B���h�E�N���X�̔j�����s���B
	if (0 == s_object_num_)
		::UnregisterClassA(WINDOWCLASS_NAME, Game::getCurrentObject().instanceHandle());
}

// �E�B���h�E�𐶐����ĕ\������
void DGS::Window::open()
{
	if (isOpen())
		throw Exception("���ɃE�B���h�E���J����Ă��܂��B");

	// �E�B���h�E�n���h���̐���
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
		// �\��
		::ShowWindow(hwnd_, SW_SHOW);
		if (!::UpdateWindow(hwnd_))
			throw Win32Exception();

		// Win32 API�ɂ��ĕ`��𔭐������Ȃ�����B
		if (!::ValidateRect(hwnd_, NULL))
			throw Win32Exception();
	} catch (...) {
		// �����Ɏ��s�����ꍇ�A�E�B���h�E����ė�O����֓�����B
		close();
		throw;
	}
}

// �E�B���h�E����Ĕj������
void DGS::Window::close()
{
	assertOpen();

	// WM_CLOSE ���E�B���h�E�v���V�[�W���ɑ���B
	// �֘A�t���̉����̓E�B���h�E�v���V�[�W���ōs����B
	::SendMessageA(hwnd_, WM_CLOSE, 0, 0);
}

// �E�B���h�E���J����Ă��邩�ǂ����𒲂ׂ�
void DGS::Window::assertOpen() const
{
	if (!isOpen())
		throw Exception("�E�B���h�E���J����Ă��܂���B");
}

// �N���C�A���g�̈�̃T�C�Y��ݒ肷��
void DGS::Window::setClientSize(unsigned int width, unsigned int height)
{
	assertOpen();

	// �E�B���h�E�S�̂̃T�C�Y���v�Z����
	::RECT rc, rw;
	::GetClientRect(hwnd_, &rc);
	::GetWindowRect(hwnd_, &rw);
	unsigned int w = (rw.right - rw.left) - (rc.right - rc.left) + width;
	unsigned int h = (rw.bottom - rw.top) - (rc.bottom - rc.top) + height;

	// �K�p
	if (!::SetWindowPos(hwnd_, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER))
		throw Win32Exception();
}

// �E�B���h�E�S�̂̃T�C�Y��ݒ肷��
void DGS::Window::setWindowSize(unsigned int width, unsigned int height)
{
	assertOpen();

	// �K�p
	if (!::SetWindowPos(hwnd_, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER))
		throw Win32Exception();
}
