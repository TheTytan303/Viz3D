#include "CubeFrame.h"

#include "Cell.h"
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

float CubeFrame::size = 0.5f;
//--------------------=non-Static=----------------
std::vector<DirectX::XMVECTOR> CubeFrame::GetTriangles() const noexcept
{
	std::vector<DirectX::XMVECTOR> returnVale;
	std::vector<Cell::Vertex> vertices =
	{
		{-1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},
		{-1.0f, 1.0f,-1.0f},
		{ 1.0f, 1.0f,-1.0f},
		{-1.0f,-1.0f, 1.0f},
		{ 1.0f,-1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{ 1.0f, 1.0f, 1.0f},
	};
	const std::vector<unsigned short> indecies =
	{
		0,2,1, 2,3,1,	//back
		1,3,5, 3,7,5,	//right
		2,6,3, 3,6,7,	//top
		4,5,7, 4,7,6,	//front
		0,4,2, 2,4,6,	//left
		0,1,4, 1,5,4,	//bottom
	};

	return returnVale;
}
bool CubeFrame::ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept
{
	std::vector<DirectX::XMVECTOR> vertices =
	{
		DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f,-1.0f,-1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(1.0f,-1.0f,-1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f, 1.0f,-1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(1.0f, 1.0f,-1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f,-1.0f, 1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(1.0f,-1.0f, 1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f), GetTransformXM())
	};
	vertices[0] = vertices[0];
	//DirectX::XMVectorMul

	const std::vector<unsigned short> indecies =
	{
		0,2,1, 2,3,1,	//back
		1,3,5, 3,7,5,	//right
		2,6,3, 3,6,7,	//top
		4,5,7, 4,7,6,	//front
		0,4,2, 2,4,6,	//left
		0,1,4, 1,5,4,	//bottom
	};
	for (int i = 0; i < indecies.size(); i += 3)
	{
		//DirectX::XMVECTOR v1 = DirectX::XMVectorSet
		//(
		//	vertices.at(),
		//);
		//DirectX::XMVECTOR v2 = DirectX::XMVectorSet
		//(
		//
		//);
		//DirectX::XMVECTOR v2 = DirectX::XMVectorSet
		//(
		//
		//);
		bool returnVale = DirectX::TriangleTests::Intersects(
			origin, direction,
			vertices[indecies[i]],
			vertices[indecies[i + 1]],
			vertices[indecies[i + 2]],
			dist
		);
		if (returnVale)
			return true;
	}

	//DirectX::TriangleTests::Intersects();
	return false;
}
CubeFrame::CubeFrame(
	unsigned short* meshSize,
	unsigned short x, unsigned short y, unsigned short z,
	float red, float blue, float green,
	 Graphics& gfx)
{
	if (!isStaticInitialized()) {
		struct Vertex {
			struct {
				float x;
				float y;
				float z;
			} pos;
		};
		std::vector<Vertex> vertices =
		{
			{-1.0f,-1.0f,-1.0f}, //LBD - 0
			{ 1.0f,-1.0f,-1.0f}, //RBD - 1
			{-1.0f, 1.0f,-1.0f}, //LBT - 2
			{ 1.0f, 1.0f,-1.0f}, //RBT - 3
			{-1.0f,-1.0f, 1.0f}, //LFD - 4
			{ 1.0f,-1.0f, 1.0f}, //RFD - 5
			{-1.0f, 1.0f, 1.0f}, //LFT - 6
			{ 1.0f, 1.0f, 1.0f}, //RFT - 7
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));

		const std::vector<unsigned short> indecies =
		{
			0,2, 3,1, 2,3, 1,0,	//back
			1,5, 3,7, 7,5,	//right
			2,6, 6,7,	//top
			6,4, 4,5,	//front
			0,4			//left
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
	this->coords[0] = x - (meshSize[0] / 2);
	this->coords[1] = y - (meshSize[1] / 2);
	this->coords[2] = z - (meshSize[1] / 2);
}

short* CubeFrame::getCoords() const
{
	return (short*)coords;
}
DirectX::XMMATRIX CubeFrame::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(size, size, size) *
		DirectX::XMMatrixTranslation((float)coords[0], (float)coords[1], (float)coords[2])
		;
};

void CubeFrame::Update(float angleX, float angleY, float angleZ, float x, float y, float z) noexcept
{
}