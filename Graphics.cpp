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

	// ウィンドウのサイズを調整
	// デバイス生成時のウィンドウサイズを覚えているらしく、
	// このタイミングで設定しておくと、
	// フルスクリーンから復帰したときにサイズがおかしくなることを防げる。
	window_.setClientSize(width_, height_);

	// スワップチェイン情報の設定
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

	// デバイス、スワップチェイン、イミディエイトコンテキストの生成
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

	// レンダーターゲットを生成
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

	// ビューポートの設定
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
	// スワップチェインをウィンドウモードにしておかないと
	// d3dswap_->Release() で例外が出る
	// D3D11 の仕様
	swap_->SetFullscreenState(FALSE, nullptr);
}

// 画面のクリア
void DGS::Graphics::clear(const Color& color)
{
	// Color を float[4] だと思ってそのままポインタを渡す
	dc_->ClearRenderTargetView(target_.p, &color.r);
}

// 画面の転送
void DGS::Graphics::present()
{
	swap_->Present(0, 0);
}
