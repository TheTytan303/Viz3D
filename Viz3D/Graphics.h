#pragma once
#include <exception>
#include <string>
#include "CustomException.h"

#include "DxgiInfoManager.h"
#include "Camera.h"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
namespace wrl = Microsoft::WRL;
class Graphics
{
	friend class Bindable;
private:
	wrl::ComPtr<ID3D11Device> pDevice = nullptr;
	wrl::ComPtr<IDXGISwapChain> pSwap = nullptr;
	wrl::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	wrl::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	wrl::ComPtr<ID3D11DepthStencilView> pDSV = nullptr;
	bool imguiEnabled = true;

	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

//#ifndef NDEBUG
	DxgiInfoManager infoManager;
//#endif
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

	Graphics(HWND hWnd, unsigned int windowWidth, unsigned int windowHeight);
	//Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	DirectX::XMMATRIX getProjection();
	void setProjection(DirectX::XMMATRIX projection);
	void SetCamera(DirectX::FXMMATRIX cam) noexcept;
	DirectX::XMMATRIX getCamera() const noexcept;
	
	void BeginFrame(float red, float green, float blue);
	void EnableImgui()noexcept;
	void DisableImGui() noexcept;
	bool IsImGuiEnabled() noexcept;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
	//void DrawTestTriangle(float angleX, float angleY, float x, float y);
	//void DrawCell(CubeCell* cell, float angleX, float angleY, float resize);
	void DrawIndexed(UINT count) noexcept;
};