// Shader.h
// プログラマブルシェーダ関連の処理
#pragma once

#include <utility>
#include <string>
#include <atlbase.h>
#include <d3d11.h>
#include "Graphics.h"
#include "utility.h"

namespace DGS {

	// シェーダの読み込みオプション
	enum ShaderLoadingFlags {
		DGS_SHADER_LOAD_FROM_FILE,
		DGS_SHADER_LOAD_FROM_STRING,
	};

	// 頂点シェーダ
	class VertexShader : private Uncopyable {
		::CComPtr<::ID3D11Device> dev_;
		::CComPtr<::ID3D11DeviceContext> dc_;
		::CComPtr<::ID3DBlob> asm_blob_;
		::CComPtr<::ID3D11VertexShader> shader_;

	public:
		VertexShader(
			Graphics& graphics,
			const std::string& src,
			const std::string& entry_point,
			ShaderLoadingFlags flag,
			const std::string& profile = "vs_4_0");
		~VertexShader() {}

		void apply();

		::ID3DBlob* d3dBlob() const { return asm_blob_.p; }
	};

	// ピクセルシェーダ
	class PixelShader : private Uncopyable {
		::CComPtr<::ID3D11Device> dev_;
		::CComPtr<::ID3D11DeviceContext> dc_;
		::CComPtr<::ID3DBlob> asm_blob_;
		::CComPtr<::ID3D11PixelShader> shader_;

	public:
		PixelShader(
			Graphics& graphics,
			const std::string& file_name,
			const std::string& entry_point,
			ShaderLoadingFlags flag,
			const std::string& profile = "ps_4_0");
		~PixelShader() {}

		void apply();

		::ID3DBlob* d3dBlob() const { return asm_blob_.p; }
	};

}
