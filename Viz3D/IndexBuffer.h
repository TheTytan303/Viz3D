#pragma once
#include "Bindable.h"
#include <wrl.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>
#include <d3dcompiler.h>

class IndexBuffer : public Bindable
{
protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indecies);
	void Bind(Graphics& gfx) noexcept override;
	UINT GetCount() const noexcept;
};