// Context.cpp
#include "Context.h"

#include <cstring>
#include <cmath>
#include <string>

namespace {
	// シェーダ
	const std::string shader_src =
		"Texture2D g_tex : register(t0);"
		"SamplerState g_samp : register(s0);"
		"cbuffer env { float4 t1; }"
		"struct VS_IN { float4 p : POSITION; float4 c : COLOR; float2 t : TEXCOORD; };"
		"struct VS_OUT { float4 p : SV_Position; float4 c : COLOR; float2 t : TEXCOORD; };"
		"VS_OUT vs(VS_IN vi) { VS_OUT vo; vo.p = vi.p * t1 + float4(-1,1,0,0); vo.c = vi.c; vo.t = vi.t; return vo; }"
		"float4 ps(VS_OUT vi) : SV_Target { return vi.c; }"
		"float4 t_ps(VS_OUT vi) : SV_Target { return vi.c * g_tex.Sample(g_samp, vi.t); }"
		;

	// 頂点データの最大数
	const unsigned int CONTEXT2D_MAX_VERTICES = 64;

	// 円と見なす角数
	const unsigned int CONTEXT2D_CIRCLE_EDGES = 64;

	// 数学定数
#ifndef M_PI
	const float M_PI = 3.14159265359f;
#endif
#ifndef M_2PI
	const float M_2PI = 2.0f * M_PI;
#endif
}

// c-tor
DGS::Context2D::Context2D(Graphics& graphics)
	: dev_(&graphics.d3dDevice())
	, dc_(&graphics.d3dContext())
	, vs_(graphics, shader_src, "vs", DGS_SHADER_LOAD_FROM_STRING)
	, ps_(graphics, shader_src, "ps", DGS_SHADER_LOAD_FROM_STRING)
	, t_ps_(graphics, shader_src, "t_ps", DGS_SHADER_LOAD_FROM_STRING)
	, v_(new Context2DVertex[CONTEXT2D_MAX_VERTICES])
{
	// 入力レイアウトの生成
	::D3D11_INPUT_ELEMENT_DESC layout_desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	::ID3D11InputLayout* layout;
	::ID3DBlob* blob_ = vs_.d3dBlob();
	::HRESULT hr = dev_->CreateInputLayout(
		layout_desc,
		sizeof(layout_desc) / sizeof(::D3D11_INPUT_ELEMENT_DESC),
		blob_->GetBufferPointer(),
		blob_->GetBufferSize(),
		&layout);
	
	if (FAILED(hr))
		throw DirectXException(hr);

	layout_.Attach(layout);

	// ラスタライザステートの生成
	::D3D11_RASTERIZER_DESC rs_desc;
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.CullMode = D3D11_CULL_NONE;
	rs_desc.FrontCounterClockwise = true;
	rs_desc.DepthBias = 0;
	rs_desc.DepthBiasClamp = 0.0f;
	rs_desc.SlopeScaledDepthBias = 0.0f;
	rs_desc.DepthClipEnable = false;
	rs_desc.ScissorEnable = false;
	rs_desc.MultisampleEnable = false;
	rs_desc.AntialiasedLineEnable = false;
	::ID3D11RasterizerState* rs;
	hr = dev_->CreateRasterizerState(&rs_desc, &rs);

	if (FAILED(hr))
		throw DirectXException(hr);

	rs_.Attach(rs);

	// ブレンドステートの生成
	::D3D11_BLEND_DESC bs_desc;
	bs_desc.AlphaToCoverageEnable = false;
	bs_desc.IndependentBlendEnable = false;
	bs_desc.RenderTarget[0].BlendEnable = true;
	bs_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bs_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bs_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bs_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bs_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bs_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bs_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	::ID3D11BlendState* bs;
	hr = dev_->CreateBlendState(&bs_desc, &bs);

	if (FAILED(hr))
		throw DirectXException(hr);

	bs_.Attach(bs);

	// サンプラステートの生成
	D3D11_SAMPLER_DESC ss_desc;
	ss_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	ss_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	ss_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	ss_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	ss_desc.MipLODBias = 0.0f;
	ss_desc.MaxAnisotropy = 0;
	ss_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	ss_desc.BorderColor[0] = 0.0f;
	ss_desc.BorderColor[1] = 0.0f;
	ss_desc.BorderColor[2] = 0.0f;
	ss_desc.BorderColor[3] = 0.0f;
	ss_desc.MinLOD = 0;
	ss_desc.MaxLOD = 0;
	::ID3D11SamplerState* ss;
	hr = dev_->CreateSamplerState(&ss_desc, &ss);

	if (FAILED(hr))
		throw DirectXException(hr);

	ss_.Attach(ss);

	// GPU 側バッファの生成
	// 各描画関数でバッファを使い回すので、
	// 書き込みアクセス可能なものを生成する。
	::D3D11_BUFFER_DESC buffer_desc;
	std::memset(&buffer_desc, 0, sizeof(buffer_desc));
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = sizeof(Context2DVertex) * CONTEXT2D_MAX_VERTICES;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	::ID3D11Buffer* buf;
	hr = dev_->CreateBuffer(&buffer_desc, nullptr, &buf);

	if (FAILED(hr))
		throw DirectXException(hr);

	buf_.Attach(buf);

	// 定数バッファの生成
	// 座標変換用のベクトルを計算して格納する。
	// x: [0, W] -> [-1, 1] ... x' = 2x/W - 1
	// y: [0, H] -> [1, -1] ... y' = -2y/H + 1
	// p = (x, y, 0, 1)
	// p' = (x', y', 0, 1)
	// t1 = (2/W, -2/H, 1, 1)
	// t2 = (-1, 1, 0, 0)
	// とすると、
	// p' = p .* t1 + t2
	// t2 は常に同じ値なので、HLSLに直接埋め込む。
	Vector4 t1(2.0f / (float)graphics.width(), -2.0f / (float)graphics.height(), 1.0f, 1.0f);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = sizeof(Vector4);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	::D3D11_SUBRESOURCE_DATA init_data;
	std::memset(&init_data, 0, sizeof(init_data));
	init_data.pSysMem = &t1;
	hr = dev_->CreateBuffer(&buffer_desc, &init_data, &buf);

	if (FAILED(hr))
		throw DirectXException(hr);

	cbuf_.Attach(buf);

	// CPU 側バッファの初期化
	// 変更されない部分だけ先に代入しておく
	for (unsigned int i = 0; i < CONTEXT2D_MAX_VERTICES; ++i) {
		v_[i].p.z = 0.0f;
		v_[i].p.w = 1.0f;
	}
}

// d-tor
DGS::Context2D::~Context2D()
{
}

// 頂点バッファを転送して描画
void DGS::Context2D::drawInner(unsigned int vertex_num, D3D11_PRIMITIVE_TOPOLOGY topology)
{
	// 頂点バッファを転送
	D3D11_MAPPED_SUBRESOURCE rc;
	::HRESULT hr = dc_->Map(buf_.p, 0, D3D11_MAP_WRITE_DISCARD, 0, &rc);
	if (FAILED(hr))
		return;
	std::memcpy(rc.pData, v_.get(), vertex_num * sizeof(Context2DVertex));
	dc_->Unmap(buf_.p, 0);

	// 設定
	UINT stride = sizeof(Context2DVertex);
	UINT offset = 0;
	dc_->IASetInputLayout(layout_.p);
	dc_->IASetVertexBuffers(0, 1, &buf_.p, &stride, &offset);
	dc_->IASetPrimitiveTopology(topology);
	dc_->VSSetConstantBuffers(0, 1, &cbuf_.p);
	dc_->RSSetState(rs_.p);
	dc_->OMSetBlendState(bs_.p, nullptr, 0xffffffff);
	vs_.apply();
	ps_.apply();

	// 描画
	dc_->Draw(vertex_num, 0);
}

// 頂点バッファを転送してテクスチャを描画
void DGS::Context2D::drawTextureInner(unsigned int vertex_num, D3D11_PRIMITIVE_TOPOLOGY topology, const Texture2D& texture)
{
	// 頂点バッファを転送
	D3D11_MAPPED_SUBRESOURCE rc;
	::HRESULT hr = dc_->Map(buf_.p, 0, D3D11_MAP_WRITE_DISCARD, 0, &rc);
	if (FAILED(hr))
		return;
	std::memcpy(rc.pData, v_.get(), vertex_num * sizeof(Context2DVertex));
	dc_->Unmap(buf_.p, 0);

	// 設定
	UINT stride = sizeof(Context2DVertex);
	UINT offset = 0;
	dc_->IASetInputLayout(layout_.p);
	dc_->IASetVertexBuffers(0, 1, &buf_.p, &stride, &offset);
	dc_->IASetPrimitiveTopology(topology);
	dc_->VSSetConstantBuffers(0, 1, &cbuf_.p);
	::ID3D11ShaderResourceView* rv = &texture.d3dResourceView();
	dc_->PSSetShaderResources(0, 1, &rv);
	dc_->PSSetSamplers(0, 1, &ss_.p);
	dc_->RSSetState(rs_.p);
	dc_->OMSetBlendState(bs_.p, nullptr, 0xffffffff);
	vs_.apply();
	t_ps_.apply();

	// 描画
	dc_->Draw(vertex_num, 0);
}


// 三角形を描画
void DGS::Context2D::drawTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Color& color)
{
	v_[0].p.x = p1.x, v_[0].p.y = p1.y;
	v_[1].p.x = p2.x, v_[1].p.y = p2.y;
	v_[2].p.x = p3.x, v_[2].p.y = p3.y;

	v_[0].c = v_[1].c = v_[2].c = color;

	drawInner(3, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

// 矩形を描画
void DGS::Context2D::drawRectangle(const Vector2& lefttop, const Vector2& size, const Color& color)
{
	v_[0].p.x = v_[2].p.x = lefttop.x;
	v_[0].p.y = v_[1].p.y = lefttop.y;
	v_[1].p.x = v_[3].p.x = lefttop.x + size.x;
	v_[2].p.y = v_[3].p.y = lefttop.y + size.y;

	v_[0].c = v_[1].c = v_[2].c = v_[3].c = color;

	drawInner(4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

// 正多角形を描画
void DGS::Context2D::drawRegular(unsigned int n, const Vector2& center, float radius, float rot, const Color& color)
{
	// 角数を制限
	if (n < 3)
		return;
	if (n > CONTEXT2D_MAX_VERTICES)
		n = CONTEXT2D_MAX_VERTICES;

	// トライアングルストリップを生成

	// 最初の点
	v_[0].p.x = center.x + radius * std::cos(rot);
	v_[0].p.y = center.y + radius * std::sin(rot);

	// 最後の点 (偶数角形のみ)
	if ((n & 1) == 0) {
		v_[n - 1].p.x = center.x - radius * std::cos(rot);
		v_[n - 1].p.y = center.y - radius * std::sin(rot);
	}

	// 中間の点
	for (unsigned int i = 2; i < n; i += 2) {
		float t = M_2PI * static_cast<float>(i >> 1) / static_cast<float>(n);
		v_[i - 1].p.x = center.x + radius * std::cos(rot + t);
		v_[i - 1].p.y = center.y + radius * std::sin(rot + t);
		v_[i].p.x = center.x + radius * std::cos(rot - t);
		v_[i].p.y = center.y + radius * std::sin(rot - t);
	}

	// 色
	for (unsigned int i = 0; i < n; ++i)
		v_[i].c = color;

	drawInner(n, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

// 円を描画
void DGS::Context2D::drawCircle(const Vector2& center, float radius, const Color& color)
{
	// 正 CONTEXT2D_CIRCLE_EDGES 角形を描画
	drawRegular(CONTEXT2D_CIRCLE_EDGES, center, radius, 0.0f, color);
}

// 楕円を描画
void DGS::Context2D::drawEllipse(const Vector2& center, float a, float b, float rot, const Color& color)
{
	float cos_t = std::cos(rot);
	float sin_t = std::sin(rot);

	// トライアングルストリップを生成
	// CONTEXT2D_CIRCLE_EDGES 角形とする

	// 最初の点
	v_[0].p.x = center.x + a * cos_t;
	v_[0].p.y = center.y + a * sin_t;
	
	// 最後の点
	v_[CONTEXT2D_CIRCLE_EDGES - 1].p.x = center.x - a * cos_t;
	v_[CONTEXT2D_CIRCLE_EDGES - 1].p.y = center.y - a * sin_t;

	// 中間の点
	for (unsigned int i = 2; i < CONTEXT2D_CIRCLE_EDGES; i += 2) {
		float p = M_2PI * static_cast<float>(i >> 1) / static_cast<float>(CONTEXT2D_CIRCLE_EDGES);
		float a_cos_p = a * std::cos(p);
		float b_sin_p = b * std::sin(p);
		v_[i - 1].p.x = center.x + a_cos_p * cos_t - b_sin_p * sin_t;
		v_[i - 1].p.y = center.y + a_cos_p * sin_t + b_sin_p * cos_t;
		v_[i].p.x = center.x + a_cos_p * cos_t + b_sin_p * sin_t;
		v_[i].p.y = center.y + a_cos_p * sin_t - b_sin_p * cos_t;
	}

	// 色
	for (unsigned int i = 0; i < CONTEXT2D_CIRCLE_EDGES; ++i)
		v_[i].c = color;

	drawInner(CONTEXT2D_CIRCLE_EDGES, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

// テクスチャを描画
void DGS::Context2D::drawTexture(
	const Vector2& lefttop,
	const Vector2& size,
	const Texture2D& texture,
	const Color& color)
{
	drawTexture(lefttop, size, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), texture, color);
}

// テクスチャを描画
// テクスチャ座標を指定
void DGS::Context2D::drawTexture(
	const Vector2& lefttop,
	const Vector2& size,
	const Vector2& t1,
	const Vector2& t2,
	const Texture2D& texture,
	const Color& color)
{
	v_[0].p.x = v_[2].p.x = lefttop.x;
	v_[0].p.y = v_[1].p.y = lefttop.y;
	v_[1].p.x = v_[3].p.x = lefttop.x + size.x;
	v_[2].p.y = v_[3].p.y = lefttop.y + size.y;

	v_[0].t.x = v_[2].t.x = t1.x;
	v_[0].t.y = v_[1].t.y = t1.y;
	v_[1].t.x = v_[3].t.x = t2.x;
	v_[2].t.y = v_[3].t.y = t2.y;

	v_[0].c = v_[1].c = v_[2].c = v_[3].c = color;

	drawTextureInner(4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, texture);
}

// テクスチャを描画
// 中心点からのサイズと回転角を指定
// テクスチャ座標を指定
void DGS::Context2D::drawTexture(
	const Vector2& center,
	const Vector2& scale,
	float rot,
	const Texture2D& texture,
	const Color& color)
{
	drawTexture(center, scale, rot, Vector2(0.0f, 0.0f), Vector2(1.0f, 1.0f), texture, color);
}

// テクスチャを描画
// 中心点からのサイズと回転角を指定
// テクスチャ座標を指定
void DGS::Context2D::drawTexture(
	const Vector2& center,
	const Vector2& scale,
	float rot,
	const Vector2& t1,
	const Vector2& t2,
	const Texture2D& texture,
	const Color& color)
{
	float cos_t = std::cos(rot);
	float sin_t = std::sin(rot);
	float sx_cos_t = scale.x * cos_t;
	float sx_sin_t = scale.x * sin_t;
	float sy_cos_t = scale.y * cos_t;
	float sy_sin_t = scale.y * sin_t;

	v_[0].p.x = center.x - sx_cos_t + sy_sin_t;
	v_[0].p.y = center.y - sx_sin_t - sy_cos_t;
	v_[1].p.x = center.x + sx_cos_t + sy_sin_t;
	v_[1].p.y = center.y + sx_sin_t - sy_cos_t;
	v_[2].p.x = center.x - sx_cos_t - sy_sin_t;
	v_[2].p.y = center.y - sx_sin_t + sy_cos_t;
	v_[3].p.x = center.x + sx_cos_t - sy_sin_t;
	v_[3].p.y = center.y + sx_sin_t + sy_cos_t;

	v_[0].t.x = v_[2].t.x = t1.x;
	v_[0].t.y = v_[1].t.y = t1.y;
	v_[1].t.x = v_[3].t.x = t2.x;
	v_[2].t.y = v_[3].t.y = t2.y;

	v_[0].c = v_[1].c = v_[2].c = v_[3].c = color;

	drawTextureInner(4, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, texture);
}
