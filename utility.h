// utility.h
// ���𗧂��c�[������
#pragma once

#include <string>
#include <stdexcept>
#include <windows.h> // for ::HRESULT

namespace DGS {
	// �R�s�[�}�~�̂��߂̃N���X
	class Uncopyable {
		Uncopyable(const Uncopyable&);
		Uncopyable& operator=(const Uncopyable&);
	public:
		Uncopyable() {}
		virtual ~Uncopyable() {}
	};

	// 2 �����x�N�g��
	struct Vector2 {
		float x, y;
		Vector2() : x(0.0f), y(0.0f) {}
		Vector2(const Vector2& src) : x(src.x), y(src.y) {}
		Vector2(float x_, float y_) : x(x_), y(y_) {}
		Vector2& operator=(const Vector2& src) { x = src.x, y = src.y; return *this; }
	};

	// 3 �����x�N�g��
	struct Vector3 {
		float x, y, z;
		Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3(const Vector3& src) : x(src.x), y(src.y), z(src.z) {}
		Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
		Vector3& operator=(const Vector3& src) { x = src.x, y = src.y, z = src.z; return *this; }
	};

	// 4 �����x�N�g��
	struct Vector4 {
		float x, y, z, w;
		Vector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
		Vector4(const Vector4& src) : x(src.x), y(src.y), z(src.z), w(src.w) {}
		Vector4(float x_, float y_, float z_) : x(x_), y(y_), z(z_), w(1.0f) {}
		Vector4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
		Vector4& operator=(const Vector4& src) { x = src.x, y = src.y, z = src.z, w = src.w; return *this; }
	};


	// RGBA �F
	struct Color {
		float r, g, b, a;
		Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
		Color(const Color& src) : r(src.r), g(src.g), b(src.b), a(src.a) {}
		Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_), a(1.0f) {}
		Color(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
		Color& operator=(const Color& src) { r = src.r, g = src.g, b = src.b, a = src.a; return *this; }
	};

	// Win32�G���[
	// �R���X�g���N�^�Ɉ�����^�����ꍇ�͂��̃G���[�R�[�h
	// �^���Ȃ������ꍇ�� ::GetLastError() �̒l�ƂȂ�.
	class Win32ErrorCode {
		unsigned int code_;
	public:
		Win32ErrorCode() : code_(::GetLastError()) {}
		Win32ErrorCode(const Win32ErrorCode& src) : code_(src.code_) {}
		Win32ErrorCode(unsigned int code) : code_(code) {}

		Win32ErrorCode& operator=(const Win32ErrorCode& src) { return operator=(src.code_); }
		Win32ErrorCode& operator=(unsigned int code) { code_ = code; return *this; }

		bool operator==(const Win32ErrorCode& src) const { return (code_ == src.code_); }
		bool operator==(unsigned int code) const { return (code_ == code); }
		bool operator!=(const Win32ErrorCode& src) const { return !operator==(src); }
		bool operator!=(unsigned int code) const { return !operator==(code); }

		unsigned int code() const { return code_; }
		std::string toString() const;
	};

	// DirectX�̃G���[�R�[�h(HRESULT�̃��b�p)
	class DirectXResult {
		::HRESULT code_;
	public:
		DirectXResult(const DirectXResult& src) : code_(src.code_) {}
		DirectXResult(::HRESULT code) : code_(code) {}

		DirectXResult& operator=(const DirectXResult& src) { return operator=(src.code_); }
		DirectXResult& operator=(::HRESULT code) { code_ = code; return *this; }

		bool operator==(const DirectXResult& src) const { return (code_ == src.code_); }
		bool operator==(::HRESULT code) const { return (code_ == code); }
		bool operator!=(const DirectXResult& src) const { return !operator==(src); }
		bool operator!=(::HRESULT code) const { return !operator==(code); }

		unsigned int code() const { return code_; }
		std::string toString() const;
	};

	// ��O
	class Exception : public std::domain_error {
	public:
		Exception(const std::string& what)
			: std::domain_error("DGS ERROR: " + what) {}
	};

	// ��O: Win32 �G���[
	class Win32Exception : public Exception {
	public:
		Win32Exception()
			: Exception("Win32: " + Win32ErrorCode().toString()) {}
		Win32Exception(const Win32ErrorCode& code)
			: Exception("Win32: " + code.toString()) {}
	};

	// ��O: DirectX �G���[
	class DirectXException : public Exception {
	public:
		DirectXException(const DirectXResult& code)
			: Exception("DirectX: " + code.toString()) {}
	};

	// ���W�A���Ɠx�̕ϊ�
	inline float degToRad(float deg) { return deg * (3.1415926536f / 180.0f); }
	inline float radToDeg(float deg) { return deg * (180.0f / 3.1415926536f); }
}
