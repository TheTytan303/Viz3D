#include "CubeFrame.h"

#include "Line.h"
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
//--------------------=non-Static=----------------
Line::Line(
	std::vector<float> x, std::vector<float> y,
	float red, float blue, float green,
	Graphics& gfx)
{
	if (!isStaticInitialized()) {

		const std::vector<unsigned short> indecies =
		{
			0,1
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
		{x[0], x[1], x[2]}, //LBD - 0
		{ y[0], y[1], y[2]}, //RBD - 1
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

DirectX::XMMATRIX Line::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(1, 1, 1);
};

void Line::Update(float angleX, float angleY, float angleZ, float x, float y, float z) noexcept
{
}