#include "Surface.h"
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
Surface::Surface(std::vector<Point> points,
	float red, float green, float blue,
	float alpha, Graphics& gfx)
{
	if (!isStaticInitialized()) {

		const std::vector<unsigned short> indecies =
		{
			0,1,2,
			0,2,1,
			1,2,3,
			1,3,2
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
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}
	float a = points[1].x - points[0].x;
	float b = points[1].y - points[0].y;
	float c = points[1].z - points[0].z;

	float d = points[2].x - points[0].x;
	float e = points[2].y - points[0].y;
	float f = points[2].z - points[0].z;
	A = (b * f) - (c * e);
	B = (c*d) - (a*f);
	C = (a*e) - (b*d);
	D = ((A*points[0].x)+ (B * points[0].y)+ (C * points[0].z))*(-1);

	struct Vertex {
		struct {
			float x;
			float y;
			float z;
		} pos;
	};
	Vertex p1 = {  1, 1, 0 };
	Vertex p2 = { -1, 1, 0 };
	Vertex p3 = {  1,-1, 0 };
	Vertex p4 = { -1,-1, 0 };
	p1.pos.z = (-1)*((A*p1.pos.x) + (B*p1.pos.y) + D)/(C);
	p2.pos.z = (-1)*((A*p2.pos.x) + (B*p2.pos.y) + D)/(C);
	p3.pos.z = (-1)*((A*p3.pos.x) + (B*p3.pos.y) + D)/(C);
	p4.pos.z = (-1)*((A*p4.pos.x) + (B*p4.pos.y) + D)/(C);
	std::vector<Vertex> vertices ={	p1,p2,p3,p4	};
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
void Surface::SetSize(float size)
{
	this->size = size;
}
float Surface::onSurface(Point p)
{
	return (p.x*A) + (p.y*B) + (p.z*C) + D;
}
DirectX::XMMATRIX Surface::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(size, size, size);
};
