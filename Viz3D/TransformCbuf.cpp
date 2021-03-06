#include "TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& gfx, const Drawable& parent)
	:
	parent(parent)
{
	if (!pVcbuf)
	{
		pVcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCbuf::Bind(Graphics& gfx) noexcept
{
	pVcbuf->update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM()* 
			gfx.getCamera() *
			gfx.getProjection()
		)
	);
	pVcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>>TransformCbuf::pVcbuf;