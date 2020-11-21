#include "Window.h"
#include <Windows.h>
#include <iostream>
#include <sstream>
#include "imgui_impl_win32.h"
#include "imgui.h"
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept :
	hInst(GetModuleHandle(nullptr)) 
{
	//register class
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}
Window::WindowClass::~WindowClass() {
	UnregisterClass(wndClassName, GetInstance() );
}

const LPCWSTR Window::WindowClass::GetName() noexcept {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wndClass.hInst;
}

Window::Window(int width, int height, LPCWSTR name) noexcept
	:
	width(width),
	height(height)
{

	RECT windowRectangle;
	windowRectangle.left = 100;
	windowRectangle.right = width + windowRectangle.left;
	windowRectangle.top = 100;
	windowRectangle.bottom = height + windowRectangle.top;
	AdjustWindowRect(&windowRectangle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, false);
	hWnd = CreateWindowEx(
		0, WindowClass::GetName(), name,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRectangle.right - windowRectangle.left,
		windowRectangle.bottom - windowRectangle.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);


	ShowWindow(hWnd, SW_SHOWDEFAULT);
	//GUI:
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hWnd);
	pGfx = std::make_unique<Graphics>(hWnd, width,height);
}
Window::~Window() {
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(hWnd);
}
LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NCCREATE) {
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
		return true;
	}
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
		return 0;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		if (ImGui::GetIO().WantCaptureKeyboard)
		{
			break;
		}
		if (!(lParam & 0x40000000) || kbd.AutorepeadisEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		if (ImGui::GetIO().WantCaptureKeyboard)
		{
			break;
		}
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_CHAR:
	{
		if (ImGui::GetIO().WantCaptureKeyboard)
		{
			break;
		}
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height) {
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWidnow()) {
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else {
			if (wParam & (MK_LBUTTON | MK_RBUTTON)) {
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else {
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftRelease(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightRelease(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		//if()
		break;
	}
	case WM_MBUTTONDOWN:
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnWheelPressed(pt.x, pt.y);
		break;
	}
	case WM_MBUTTONUP:
	{
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnWheelRelease(pt.x, pt.y);
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
bool Window::SetWindowTitle(const std::string& title) {
	if (SetWindowTextA(hWnd, title.c_str()) == 0) {
		throw CWND_LAST_EXCEPT();
		return false;
	}
	return true;
}

bool Window::SetWindowTitleW(const std::wstring& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0) {
		throw CWND_LAST_EXCEPT();
		return false;
	}
	return true;
}

Graphics& Window::Gfx()
{
	return *pGfx;
}


//Windows Exception
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept :
	CustomException(line,file),
	hr(hr)
{};
const char* Window::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
};
const char* Window::Exception::GetType() const noexcept {
	return "Custom Window Exception";
};
std::string Window::Exception::TranslateErrorCode(HRESULT hr) {
	char* pMsgBuff = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,hr,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		reinterpret_cast<LPWSTR>(&pMsgBuff),
		0,nullptr
	);
	if (nMsgLen == 0) {
		return "Unindetified error code";
	}
	std::string errorString = pMsgBuff;
	LocalFree(pMsgBuff);
	return errorString;
};
HRESULT Window::Exception::GetErrorCode() const noexcept {
	return hr;
};
std::string Window::Exception::GetErrorString() const noexcept {
	return TranslateErrorCode(hr);
};