#include "Star.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "Topology.h"
#include "InputLayout.h"
#include "TransformCbuf.h"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <DirectXMath.h>
#include <DirectXCollision.h>
Star::Star(std::vector<float> coords, float red, float blue, float green, Graphics& gfx)

{
	if (!isStaticInitialized()) {

		const std::vector<unsigned short> indecies =
		{
			0,1,
			2,3,
			4,5
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indecies));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
		auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	struct Vertex {
		struct {
			float x;
			float y;
			float z;
		} pos;
	};
	std::vector<Vertex> vertices =
	{
		{coords[0]-0.1f, coords[1], coords[2]},
		{coords[0]+0.1f, coords[1], coords[2]},
		{coords[0], coords[1] - 0.1f, coords[2]},
		{coords[0], coords[1] + 0.1f, coords[2]},
		{coords[0], coords[1], coords[2] - 0.1f},
		{coords[0], coords[1], coords[2] + 0.1f},
	};
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		}face_colors[1];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{red, blue, green, 1.0f}
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}
DirectX::XMMATRIX Star::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(1, 1, 1);
};
