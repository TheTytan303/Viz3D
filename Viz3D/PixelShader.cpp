#include "PixelShader.h"
#include "macros.h"

PixelShader::PixelShader(Graphics& gfx, LPCWSTR filename)
{
	INFOMAN(gfx);
	Microsoft::WRL::ComPtr<ID3DBlob>  pBlob;
	D3DReadFileToBlob(filename, &pBlob);
	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	//GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
