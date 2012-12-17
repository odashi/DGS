// Graphics.h
// 画面デバイスの管理
#pragma once

#include <atlbase.h>
#include <d3d11.h>
#include "Window.h"
#include "utility.h"

namespace DGS {
	// Direct3D 関連の処理
	class Graphics : private Uncopyable {
		Window& window_;
		unsigned int width_;
		unsigned int height_;
		bool full_screen_;
		::CComPtr<::ID3D11Device> dev_;
		::CComPtr<::IDXGISwapChain> swap_;
		::CComPtr<::ID3D11DeviceContext> dc_;
		::CComPtr<::ID3D11RenderTargetView> target_;
	public:
		Graphics(
			Window& window,
			unsigned int width,
			unsigned int height,
			bool full_screen);
		~Graphics();

		void clear(const Color& color = Color());

		void present();

		unsigned int width() const { return width_; }
		unsigned int height() const { return height_; }
		::ID3D11Device& d3dDevice() const { return *dev_; }
		::ID3D11DeviceContext& d3dContext() const { return *dc_; }
	};
}
