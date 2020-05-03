#pragma once
#include "Bindable.h"
#include "wrl.h"
class PixelShader : public Bindable
{
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
public:
	PixelShader(Graphics& gfx, LPCWSTR filename);
	void Bind(Graphics& gfx) noexcept override;
};