// Context.h
// •`‰æŠÖ˜A‚Ìƒ‰ƒbƒp
#pragma once

#include <memory>
#include <atlbase.h>
#include <d3d11.h>
#include "Graphics.h"
#include "Shader.h"
#include "Texture.h"
#include "utility.h"

namespace DGS {
	// 2D’¸“_ƒf[ƒ^
	struct Context2DVertex {
		Vector4 p;
		Color c;
		Vector2 t;
	};

	// 2D •`‰æŠÂ‹«
	class Context2D : private Uncopyable {
		::CComPtr<::ID3D11Device> dev_;
		::CComPtr<::ID3D11DeviceContext> dc_;
		::CComPtr<::ID3D11InputLayout> layout_;
		::CComPtr<::ID3D11RasterizerState> rs_;
		::CComPtr<::ID3D11BlendState> bs_;
		::CComPtr<::ID3D11SamplerState> ss_;
		::CComPtr<::ID3D11Buffer> buf_, tbuf_;
		::CComPtr<::ID3D11Buffer> cbuf_;
		std::unique_ptr<Context2DVertex[]> v_;
		VertexShader vs_;
		PixelShader ps_, t_ps_;

		void drawInner(unsigned int vertex_num, D3D11_PRIMITIVE_TOPOLOGY topology);
		void drawTextureInner(unsigned int vertex_num, D3D11_PRIMITIVE_TOPOLOGY topology, const Texture2D& texture);

	public:
		Context2D(Graphics& graphics);
		~Context2D();

		// OŠpŒ`‚ğ•`‰æ
		void drawTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Color& color);

		// ‹éŒ`‚ğ•`‰æ
		void drawRectangle(const Vector2& lefttop, const Vector2& size, const Color& color);

		// ³‘½ŠpŒ`‚ğ•`‰æ
		void drawRegular(unsigned int n, const Vector2& center, float radius, float rot, const Color& color);

		// ‰~‚ğ•`‰æ
		void drawCircle(const Vector2& center, float radius, const Color& color);

		// ‘È‰~‚ğ•`‰æ
		void drawEllipse(const Vector2& center, float a, float b, float rot, const Color& color);

		// ƒeƒNƒXƒ`ƒƒ‚ğ•`‰æ
		void drawTexture(
			const Vector2& lefttop,
			const Vector2& size,
			const Texture2D& texture,
			const Color& color = Color(1.0f, 1.0f, 1.0f, 1.0f));
		void drawTexture(
			const Vector2& lefttop,
			const Vector2& size,
			const Vector2& t1,
			const Vector2& t2,
			const Texture2D& texture,
			const Color& color = Color(1.0f, 1.0f, 1.0f, 1.0f));
		void drawTexture(
			const Vector2& center,
			const Vector2& scale,
			float rot,
			const Texture2D& texture,
			const Color& color = Color(1.0f, 1.0f, 1.0f, 1.0f));
		void drawTexture(
			const Vector2& center,
			const Vector2& scale,
			float rot,
			const Vector2& t1,
			const Vector2& t2,
			const Texture2D& texture,
			const Color& color = Color(1.0f, 1.0f, 1.0f, 1.0f));
	};
}
