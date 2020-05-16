#pragma once
#include <Windows.h>
#include <memory>
#include "CustomException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "macros.h"

/*
	Reprezentacja okna
*/

class Window
{
private:
	class WindowClass {
	public:
		static const LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const LPCWSTR wndClassName = L"Direct 3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	class Exception : CustomException {
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override {
			return "No graphics exception";
		};
	};
	Window(int width, int height, LPCWSTR name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	bool SetWindowTitle(const std::string& title);
	bool SetWindowTitleW(const std::wstring& title);
	Graphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd;
	Mouse mouse;
private:
	int width, height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
};
