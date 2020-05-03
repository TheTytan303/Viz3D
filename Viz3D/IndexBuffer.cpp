#include "IndexBuffer.h"
#include "macros.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indecies)
	:
	count ((UINT)indecies.size())
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC indexBufferDesc = {};
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = UINT(count*sizeof(unsigned short));
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA indexsd = {};
	indexsd.pSysMem = indecies.data();
	GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&indexBufferDesc, &indexsd, &pIndexBuffer));
	//GetDevice(gfx)->CreateBuffer(&indexBufferDesc, &indexsd, &pIndexBuffer);

}
void IndexBuffer::Bind(Graphics& gfx) noexcept 
{	
	GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
