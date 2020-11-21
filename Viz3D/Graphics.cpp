#include "Graphics.h"
#include <d3dcompiler.h>
#include "dxerr.h"
#include "CubeCell.h"
#include "Macros.h"

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <comdef.h> 
#include <sstream>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")


namespace dx = DirectX;

Graphics::Graphics(HWND hWnd, unsigned int windowWidth, unsigned int windowHeight)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = 0;
		sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	//need for macros
	HRESULT hr;

	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	));

	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GFX_THROW_FAILED(pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	));


	D3D11_DEPTH_STENCIL_DESC sdesc = {};
	sdesc.DepthEnable = true;
	sdesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sdesc.DepthFunc = D3D11_COMPARISON_LESS;

	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	GFX_THROW_FAILED(pDevice->CreateDepthStencilState(&sdesc, &pDSState));
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = windowWidth;
	descDepth.Height = windowHeight;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_FAILED(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	GFX_THROW_FAILED(pDevice->CreateDepthStencilView(pDepthStencil.Get(),&descDSV,&pDSV));

	pContext->OMSetRenderTargets(1u,pTarget.GetAddressOf(),pDSV.Get());

	D3D11_VIEWPORT vp;
	vp.Width = windowWidth;
	vp.Height = windowHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

	//ImGui init
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());
}

DirectX::XMMATRIX Graphics::getProjection()
{
	return projection;
}

void Graphics::setProjection(DirectX::XMMATRIX projection)
{
	this->projection = projection;
}

void Graphics::SetCamera(DirectX::FXMMATRIX cam) noexcept
{
	camera = cam;
}

DirectX::XMMATRIX Graphics::getCamera() const noexcept
{
	return camera;
}

void Graphics::BeginFrame(float red, float green, float blue)
{
	if (imguiEnabled)
	{
		//ImGui:
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	const float color[] = { red,green,blue, 1.0F };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1, 0);
}

void Graphics::EnableImgui() noexcept
{
	imguiEnabled = true;
}

void Graphics::DisableImGui() noexcept
{
	imguiEnabled = false;
}

bool Graphics::IsImGuiEnabled() noexcept
{
	return imguiEnabled;
}

void Graphics::EndFrame()
{
	if (imguiEnabled) 
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	HRESULT hr;
	if ( FAILED ( hr = pSwap->Present(1u, 0u) ) ) {
		if ( hr == DXGI_ERROR_DEVICE_REMOVED ) {
			throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
		}
		else {
			GFX_THROW_FAILED(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue) noexcept
{
	const float color[] = { red,green,blue, 1.0F };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1, 0);
}

void Graphics::DrawIndexed(UINT count) noexcept
{
	pContext->DrawIndexed(count, 0u, 0u);
}


//error handling

Graphics::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}
const char* Graphics::HrException::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error code: 0x" << std::hex << GetErrorCode() << std::dec << std::endl
		<< "Error String: " << GetErrorString() << std::endl
		<< "Description: " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
};

const char* Graphics::HrException::GetType() const noexcept {
	return "GraphicsException";
};
HRESULT Graphics::HrException::GetErrorCode() const noexcept {
	return hr;
};
std::string Graphics::HrException::GetErrorString() const noexcept {
	
	const WCHAR* wc = DXGetErrorStringW(_In_  hr);
	_bstr_t b(wc);
	const char* output = b;
	std::string returnVale(output);

	return returnVale;
};
std::string Graphics::HrException::GetErrorDescription() const noexcept {
	char buff[32];
	DXGetErrorDescriptionA(hr, buff, std::size(buff));
	return buff;
};

const char* Graphics::DeviceRemovedException::GetType() const noexcept {
	return "Graphics exception - Device removed";
}

