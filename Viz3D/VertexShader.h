#pragma once
#include "Bindable.h"
#include "wrl.h"

class VertexShader : public Bindable
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	std::string path;
	Microsoft::WRL::ComPtr<ID3DBlob> pBytecodeBlob;
public:
	VertexShader(Graphics& gfx, LPCWSTR filename);
	void Bind(Graphics& gfx) noexcept override;
	ID3DBlob* GetBytecode() const noexcept;

};