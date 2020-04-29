#pragma once
#include "Bindable.h"
#include "macros.h"

class VertexBuffer : public Bindable
{
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
public:
	template<class T>
	VertexBuffer(Graphics& gfx, const std::vector<T>& vertices)
		:
		stride(sizeof(T))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.ByteWidth = UINT(sizeof(T)*vertices.size());
		bufferDesc.StructureByteStride = sizeof(T);
		D3D11_SUBRESOURCE_DATA sResourceData = {};
		sResourceData.pSysMem = vertices.data();
		GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bufferDesc, &sResourceData, &pVertexBuffer));
		//GetDevice(gfx)->CreateBuffer(&bufferDesc, &sResourceData, &pVertexBuffer);
	}
	void Bind(Graphics& gfx) noexcept override;
};