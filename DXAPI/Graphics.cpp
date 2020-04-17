#include "Graphics.h"
#include <d3dcompiler.h>
#include "dxerr.h"
#include "CubeCell.h"
#include <sstream>
#include <DirectXMath.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

#define GFX_THROW_FAILED(hrcall) if(FAILED(hr=(hrcall))) throw Graphics::HrException(__LINE__,__FILE__,hr)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException(__LINE__,__FILE__,(hr))
//#define COUT_HRFAIL(hrcall) if(FAILED(hr=(hrcall))) std::count<<__LINE__<<" | " <<__FILE__<<std::endl;
#define GFX_EXCEPT(hr) Graphics::HrException(__LINE__, __FILE__, (hr))
#define GFX_THROW_NOINFO(hrcall) if(FAILED (hr = (hrcall)) htrow Graphic

//#define GFX_EXCEPT(ht) Graphics::HrException(__LINE__, __FILE__, (hr),infoManager.GetMessages() )
#define GFX_THROW_INFO(hrcall) infoManager.Set(); if (FAILED(hr = (hrcall))) throw GFX_EXCEPT(hr)

namespace dx = DirectX;

Graphics::Graphics(HWND hWnd)
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
	descDepth.Width = 800u;
	descDepth.Height = 600u;
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
}

void Graphics::EndFrame()
{
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
	const float color[] = { red,green,blue, 1.0F};
	pContext->ClearRenderTargetView( pTarget.Get(), color);
	pContext->ClearDepthStencilView( pDSV.Get(),D3D11_CLEAR_DEPTH,1,0);
}

void Graphics::DrawTestTriangle(CubeCell* cell, float angleX, float angleY, float x, float z)
{

	Cell::Vertex* vertices = cell->getVertices();
	namespace wrl = Microsoft::WRL;
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(vertices)*sizeof(Cell::Vertex);
	bufferDesc.StructureByteStride = sizeof(Cell::Vertex);

	const unsigned short indecies[] =
	{
		0,2,1, 2,3,1,	//back
		1,3,5, 3,7,5,	//right
		2,6,3, 3,6,7,	//top
		4,5,7, 4,7,6,	//front
		0,4,2, 2,4,6,	//left
		0,1,4, 1,5,4,	//bottom
	};

	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = sizeof(indecies);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexsd = {};
	indexsd.pSysMem = indecies;
	pDevice->CreateBuffer(&indexBufferDesc, &indexsd, &pIndexBuffer);

	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);



	D3D11_SUBRESOURCE_DATA sResourceData = {};
	sResourceData.pSysMem = vertices;

	struct ConstantBuffer 
	{
		dx::XMMATRIX transform;
	};

	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationX(angleX) *
				dx::XMMatrixRotationY(angleY) *
				dx::XMMatrixTranslation(x,0.0f,z) *
				dx::XMMatrixPerspectiveLH(1.0f,3.0f/4.0f,0.5f,10.0f)
			)
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	pDevice->CreateBuffer(&cbd,&csd,&pConstantBuffer);

	//bund
	pContext->VSSetConstantBuffers(0,1,pConstantBuffer.GetAddressOf());

	pDevice->CreateBuffer(&bufferDesc, &sResourceData, &pVertexBuffer);

	//Bind vertex Buffer to pipeline
	const UINT stride = sizeof(Cell::Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(
		0u, 1u,
		pVertexBuffer.GetAddressOf(),
		&stride, &offset
	);

	wrl::ComPtr<ID3DBlob>  pBlob;


	struct ConstantBuffer2
	{
		struct 
		{
			float r;
			float g;
			float b;
			float a;
		}face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f, 1.0f},
		}
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {}; 
	csd2.pSysMem = &cb2;
	pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2);

	//bund
	pContext->PSSetConstantBuffers(0, 1, pConstantBuffer2.GetAddressOf());



	//create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	//bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	//Bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);

	// input layout (2d position only)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		//{"Color",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};


	pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	//bind vertex lauot
	pContext->IASetInputLayout(pInputLayout.Get());


	//Set primitive topology to triangle list (vertices)
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Configure ViewPort
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);


	//index buffer:
	//const unsigned short triangles[] = {
	//	0,1,2,
	//	3,0,
	//
	//
	//};

	//pContext->Draw((UINT)std::size(vertices), 0u);
	//pContext->DrawIndexed(triangles,,0u)
	pContext->DrawIndexed((UINT)std::size(indecies), 0u,0u);
}
void Graphics::DrawCell(CubeCell* cell, float angleX, float angleY, float resize) 
{
	HRESULT hr;
	const Cell::Vertex* vertices = cell->getVertices();

	namespace wrl = Microsoft::WRL;
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0u;
	bufferDesc.MiscFlags = 0u;
	bufferDesc.ByteWidth = sizeof(vertices) * sizeof(Cell::Vertex);
	bufferDesc.StructureByteStride = sizeof(Cell::Vertex);
	
	const unsigned short indecies[] =
	{
		0,2,1, 2,3,1,	//back
		1,3,5, 3,7,5,	//right
		2,6,3, 3,6,7,	//top
		4,5,7, 4,7,6,	//front
		0,4,2, 2,4,6,	//left
		0,1,4, 1,5,4,	//bottom
	};
	
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	
	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = sizeof(indecies);
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexsd = {};
	indexsd.pSysMem = indecies;
	
	GFX_THROW_FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexsd, &pIndexBuffer));
	
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	
	
	
	D3D11_SUBRESOURCE_DATA sResourceData = {};
	sResourceData.pSysMem = vertices;
	
	struct ConstantBuffer
	{
		dx::XMMATRIX transform;
	};
	
	const ConstantBuffer cb =
	{
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationY(angleY)
				* dx::XMMatrixRotationX(angleX)
				* 0.75f
				//* dx::XMMatrixTranslation(x,0.0f,z + 4.0f) 
				//* dx::XMMatrixPerspectiveLH(1.0f,3.0f / 4.0f,0.5f,10.0f)
			)
		}
	};
	
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	GFX_THROW_FAILED(pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer));
	
	//bund
	pContext->VSSetConstantBuffers(0, 1, pConstantBuffer.GetAddressOf());
	
	GFX_THROW_FAILED(pDevice->CreateBuffer(&bufferDesc, &sResourceData, &pVertexBuffer));
	
	//Bind vertex Buffer to pipeline
	const UINT stride = sizeof(Cell::Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(
		0u, 1u,
		pVertexBuffer.GetAddressOf(),
		&stride, &offset
	);
	
	wrl::ComPtr<ID3DBlob>  pBlob;
	
	
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		}face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f, 1.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 0.0f, 1.0f},
			{0.0f, 1.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f, 1.0f},
			{1.0f, 0.0f, 1.0f, 1.0f},
		}
	};
	
	wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	GFX_THROW_FAILED(pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2));
	
	//bund
	pContext->PSSetConstantBuffers(0, 1, pConstantBuffer2.GetAddressOf());
	
	
	
	//create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	GFX_THROW_FAILED(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	
	//bind pixel shader
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	
	//create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	GFX_THROW_FAILED(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	
	//Bind vertex shader
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0);
	
	// input layout (2d position only)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		//{"Color",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	
	
	GFX_THROW_FAILED(pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));
	
	//bind vertex lauot
	pContext->IASetInputLayout(pInputLayout.Get());
	
	//bind render target
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
	
	//Set primitive topology to triangle list (vertices)
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//Configure ViewPort
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);
	
	
	//index buffer:
	//const unsigned short triangles[] = {
	//	0,1,2,
	//	3,0,
	//
	//
	//};
	
	//pContext->Draw((UINT)std::size(vertices), 0u);
	//pContext->DrawIndexed(triangles,,0u)
	pContext->DrawIndexed((UINT)std::size(indecies), 0u, 0u);
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
	return DXGetErrorStringA(hr);
};
std::string Graphics::HrException::GetErrorDescription() const noexcept {
	char buff[32];
	DXGetErrorDescriptionA(hr, buff, std::size(buff));
	return buff;
};

const char* Graphics::DeviceRemovedException::GetType() const noexcept {
	return "Graphics exception - Device removed";
}
