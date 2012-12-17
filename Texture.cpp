// Texture.cpp
#include "Texture.h"

#include <cstring>
#include <d3dx11.h>

DGS::Texture2D::Texture2D(Graphics& graphics, const std::string& file_name)
	: dev_(&graphics.d3dDevice())
{
	// テクスチャの読み込み
	::D3DX11_IMAGE_LOAD_INFO load_info;
	load_info.Width = D3DX11_DEFAULT;
	load_info.Height = D3DX11_DEFAULT;
	load_info.Depth = D3DX11_DEFAULT;
	load_info.FirstMipLevel = D3DX11_DEFAULT;
	load_info.MipLevels = 1;
	load_info.Usage = D3D11_USAGE_DEFAULT;
	load_info.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	load_info.CpuAccessFlags = 0;
	load_info.MiscFlags = 0;
	load_info.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	load_info.Filter = D3DX11_DEFAULT;
	load_info.MipFilter = D3DX11_DEFAULT;
	load_info.pSrcInfo = nullptr;
	::ID3D11Texture2D* tex;
	::HRESULT hr = ::D3DX11CreateTextureFromFileA(
		dev_.p,
		file_name.c_str(),
		&load_info, nullptr,
		reinterpret_cast<ID3D11Resource**>(&tex),
		nullptr);

	if (FAILED(hr))
		throw DirectXException(hr);

	tex_.Attach(tex);

	D3D11_TEXTURE2D_DESC d;
	tex_->GetDesc(&d);

	// シェーダリソースビューの生成
	D3D11_SHADER_RESOURCE_VIEW_DESC rv_desc;
	std::memset(&rv_desc, 0, sizeof(rv_desc));
	rv_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	rv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rv_desc.Texture2D.MipLevels = 1;
	::ID3D11ShaderResourceView* rv;
	hr = dev_->CreateShaderResourceView(tex_, &rv_desc, &rv);

	if (FAILED(hr))
		throw DirectXException(hr);

	rv_.Attach(rv);
}
