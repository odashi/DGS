// Texture.h
// テクスチャ関連の処理
#pragma once

#include <string>
#include <d3d11.h>
#include <atlbase.h>
#include "Graphics.h"
#include "utility.h"

namespace DGS {
	// 2D テクスチャ
	class Texture2D : private Uncopyable {
		::CComPtr<::ID3D11Device> dev_;
		::CComPtr<::ID3D11Texture2D> tex_;
		::CComPtr<::ID3D11ShaderResourceView> rv_;

	public:
		Texture2D(Graphics& graphics, const std::string& file_name);

		ID3D11ShaderResourceView& d3dResourceView() const { return *rv_; };
	};
}
