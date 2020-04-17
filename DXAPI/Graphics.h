#pragma once
#include <exception>
#include <string>
#include "CustomException.h"

#include "CubeCell.h"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3dcompiler.h>
namespace wrl = Microsoft::WRL;
class Graphics
{
private:
	wrl::ComPtr<ID3D11Device> pDevice = nullptr;
	wrl::ComPtr<IDXGISwapChain> pSwap = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	wrl::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;

	
public:

	class Exception : public CustomException
	{
		using CustomException::CustomException;
	};
	class HrException : public Exception {
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	};

	Graphics( HWND hWnd );
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	void DrawTestTriangle(CubeCell* cell, float angleX, float angleY, float x, float y);
	void DrawCell(CubeCell* cell, float angleX, float angleY, float resize);
};

/*

class InfoException : public std::exception {
	public:
		InfoException(int line, const char* file, string);
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info
	};

*/