#include "VertexShader.h"
#include "macros.h"

VertexShader::VertexShader(Graphics& gfx, LPCWSTR filename)
{
	INFOMAN(gfx);
	//Microsoft::WRL::ComPtr<ID3DBlob>  pBlob;
	D3DReadFileToBlob(filename, &pBytecodeBlob);
	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(), pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader));
	//GetDevice(gfx)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(), pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader);
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBytecodeBlob.Get();
}
