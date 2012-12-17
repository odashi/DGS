// Shader.cpp
#include "Shader.h"

#include <utility>
#include <d3dx11.h>
#include <d3dcompiler.h>

namespace {
	// シェーダのコンパイルオプション
#if defined(DEBUG) || defined(_DEBUG)
	const DWORD s_shader_flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
	const DWORD s_shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#endif

}



/* class DGS::VertexShader */

DGS::VertexShader::VertexShader(
	Graphics& graphics,
	const std::string& src,
	const std::string& entry_point,
	ShaderLoadingFlags flag,
	const std::string& profile)
	: dev_(&graphics.d3dDevice())
	, dc_(&graphics.d3dContext())
{
	// コンパイル
	::ID3DBlob* asm_blob;
	::ID3DBlob* error_blob;
	::HRESULT hr;
	switch (flag) {
	case DGS_SHADER_LOAD_FROM_FILE:
		// ファイルから
		hr = ::D3DX11CompileFromFileA(
			src.c_str(),
			nullptr, nullptr,
			entry_point.c_str(),
			profile.c_str(), s_shader_flags, 0,
			nullptr, &asm_blob, &error_blob, nullptr);
		break;
	case DGS_SHADER_LOAD_FROM_STRING:
		// 文字列から
		hr = ::D3DX11CompileFromMemory(
			src.c_str(), src.size(),
			nullptr, nullptr, nullptr,
			entry_point.c_str(),
			profile.c_str(), s_shader_flags, 0,
			nullptr, &asm_blob, &error_blob, nullptr);
		break;
	default:
		throw Exception("不明なパラメータが渡されました。");
	}

	if (FAILED(hr)) {
		if (error_blob) {
			std::string message = reinterpret_cast<char*>(error_blob->GetBufferPointer());
			error_blob->Release();
			throw Exception(message);
		}
		throw DirectXException(hr);
	}

	if (error_blob)
		error_blob->Release();

	asm_blob_.Attach(asm_blob);

	// シェーダオブジェクトの生成
	::ID3D11VertexShader* shader;
	hr = dev_->CreateVertexShader(
		asm_blob_->GetBufferPointer(),
		asm_blob_->GetBufferSize(),
		nullptr, &shader);
	
	if (FAILED(hr))
		throw DirectXException(hr);

	shader_.Attach(shader);
}

// シェーダの適用
void DGS::VertexShader::apply()
{
	dc_->VSSetShader(shader_.p, nullptr, 0);
}



/* class DGS::PixelShader */

DGS::PixelShader::PixelShader(
	Graphics& graphics,
	const std::string& src,
	const std::string& entry_point,
	ShaderLoadingFlags flag,
	const std::string& profile)
	: dev_(&graphics.d3dDevice())
	, dc_(&graphics.d3dContext())
{
	// コンパイル
	::ID3DBlob* asm_blob;
	::ID3DBlob* error_blob;
	::HRESULT hr;
	switch (flag) {
	case DGS_SHADER_LOAD_FROM_FILE:
		// ファイルから
		hr = ::D3DX11CompileFromFileA(
			src.c_str(),
			nullptr, nullptr,
			entry_point.c_str(),
			profile.c_str(), s_shader_flags, 0,
			nullptr, &asm_blob, &error_blob, nullptr);
		break;
	case DGS_SHADER_LOAD_FROM_STRING:
		// 文字列から
		hr = ::D3DX11CompileFromMemory(
			src.c_str(), src.size(),
			nullptr, nullptr, nullptr,
			entry_point.c_str(),
			profile.c_str(), s_shader_flags, 0,
			nullptr, &asm_blob, &error_blob, nullptr);
		break;
	default:
		throw Exception("不明なパラメータが渡されました。");
	}

	if (FAILED(hr)) {
		if (error_blob) {
			std::string message = reinterpret_cast<char*>(error_blob->GetBufferPointer());
			error_blob->Release();
			throw Exception(message);
		}
		throw DirectXException(hr);
	}

	if (error_blob)
		error_blob->Release();

	asm_blob_.Attach(asm_blob);

	// シェーダオブジェクトの生成
	::ID3D11PixelShader* shader;
	hr = dev_->CreatePixelShader(
		asm_blob_->GetBufferPointer(),
		asm_blob_->GetBufferSize(),
		nullptr, &shader);
	
	if (FAILED(hr))
		throw DirectXException(hr);

	shader_.Attach(shader);
}

// シェーダの適用
void DGS::PixelShader::apply()
{
	dc_->PSSetShader(shader_.p, nullptr, 0);
}
