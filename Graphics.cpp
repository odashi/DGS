// Graphics.cpp
#include "Graphics.h"

#include <cstring>
#include "Game.h"

DGS::Graphics::Graphics(Window& window, unsigned int width, unsigned int height, bool full_screen)
	: window_(window)
	, width_(width)
	, height_(height)
	, full_screen_(full_screen)
{
	window_.assertOpen();

	// �E�B���h�E�̃T�C�Y�𒲐�
	// �f�o�C�X�������̃E�B���h�E�T�C�Y���o���Ă���炵���A
	// ���̃^�C�~���O�Őݒ肵�Ă����ƁA
	// �t���X�N���[�����畜�A�����Ƃ��ɃT�C�Y�����������Ȃ邱�Ƃ�h����B
	window_.setClientSize(width_, height_);

	// �X���b�v�`�F�C�����̐ݒ�
	DXGI_SWAP_CHAIN_DESC sd;
	std::memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width_;
	sd.BufferDesc.Height = height_;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = Game::getCurrentObject().frameRate();
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = window_.rawHandle();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = !full_screen_;

	// �f�o�C�X�A�X���b�v�`�F�C���A�C�~�f�B�G�C�g�R���e�L�X�g�̐���
	::ID3D11Device* dev;
	::IDXGISwapChain* swap;
	::ID3D11DeviceContext* dc;
	::HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
		nullptr, 0, D3D11_SDK_VERSION, &sd,
		&swap, &dev, nullptr, &dc);
	if (FAILED(hr))
		throw DirectXException(hr);

	dev_.Attach(dev);
	swap_.Attach(swap);
	dc_.Attach(dc);

	// �����_�[�^�[�Q�b�g�𐶐�
	::ID3D11Texture2D* back_buffer;
	::ID3D11RenderTargetView* d3dtarget;
	hr = swap_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	if (FAILED(hr))
		throw DirectXException(hr);

	hr = dev_->CreateRenderTargetView(back_buffer, NULL, &d3dtarget);
	back_buffer->Release();
	if (FAILED(hr))
		throw DirectXException(hr);

	dc_->OMSetRenderTargets(1, &d3dtarget, NULL);

	target_.Attach(d3dtarget);

	// �r���[�|�[�g�̐ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<float>(width_);
	vp.Height = static_cast<float>(height_);
	vp.MinDepth = 0.0f;
	vp.MaxDepth= 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	dc_->RSSetViewports(1, &vp);
}

DGS::Graphics::~Graphics()
{
	// �X���b�v�`�F�C�����E�B���h�E���[�h�ɂ��Ă����Ȃ���
	// d3dswap_->Release() �ŗ�O���o��
	// D3D11 �̎d�l
	swap_->SetFullscreenState(FALSE, nullptr);
}

// ��ʂ̃N���A
void DGS::Graphics::clear(const Color& color)
{
	// Color �� float[4] ���Ǝv���Ă��̂܂܃|�C���^��n��
	dc_->ClearRenderTargetView(target_.p, &color.r);
}

// ��ʂ̓]��
void DGS::Graphics::present()
{
	swap_->Present(0, 0);
}
