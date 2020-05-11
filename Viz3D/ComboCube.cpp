#include "ComboCube.h"
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

ComboCube::ComboCube(unsigned short* meshSize, std::vector<std::shared_ptr<CubeCell>> subCells, Graphics& gfx)
	:
	subCells(subCells),
	cell(-1)
{
	if (subCells.size() < 8)
	{
		throw " not enough cells";
	}


	if (!isStaticInitialized()) {

		//struct Vertex {
		//	struct {
		//		float x;
		//		float y;
		//		float z;
		//	} pos;
		//};
		//std::vector<Vertex> vertices =
		//{
		//	{-1.0f,-1.0f,-1.0f},
		//	{ 1.0f,-1.0f,-1.0f},
		//	{-1.0f, 1.0f,-1.0f},
		//	{ 1.0f, 1.0f,-1.0f},
		//	{-1.0f,-1.0f, 1.0f},
		//	{ 1.0f,-1.0f, 1.0f},
		//	{-1.0f, 1.0f, 1.0f},
		//	{ 1.0f, 1.0f, 1.0f},
		//};
		//AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));


		const std::vector<unsigned short> indecies =
		{
			0,2,1, 2,3,1,	//back
			1,3,5, 3,7,5,	//right
			2,6,3, 3,6,7,	//top
			4,5,7, 4,7,6,	//front
			0,4,2, 2,4,6,	//left
			0,1,4, 1,5,4,	//bottom
		};
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indecies));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"ComboCubePixelShader.cso"));
		//AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
		//auto pvs = std::make_unique<VertexShader>(gfx, L"ComboCubeVertexShader.cso");
		auto pvs = std::make_unique<VertexShader>(gfx, L"ComboCubeVertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"Color",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0}
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}
	//struct ConstantBuffer2
	//{
	//	struct
	//	{
	//		float r;
	//		float g;
	//		float b;
	//	}vertex_colors[8];
	//};
	/*
	const ConstantBuffer2 cb2 =
	{
		{
			{
			subCells[0]->getColor()->at(0),
			subCells[0]->getColor()->at(1),
			subCells[0]->getColor()->at(2),
			},
			{
			subCells[1]->getColor()->at(0),
			subCells[1]->getColor()->at(1),
			subCells[1]->getColor()->at(2),
			},
			{
			subCells[2]->getColor()->at(0),
			subCells[2]->getColor()->at(1),
			subCells[2]->getColor()->at(2),
			},
			{
			subCells[3]->getColor()->at(0),
			subCells[3]->getColor()->at(1),
			subCells[3]->getColor()->at(2),
			},
			{
			subCells[4]->getColor()->at(0),
			subCells[4]->getColor()->at(1),
			subCells[4]->getColor()->at(2),
			},
			{
			subCells[5]->getColor()->at(0),
			subCells[5]->getColor()->at(1),
			subCells[5]->getColor()->at(2),
			},
			{
			subCells[6]->getColor()->at(0),
			subCells[6]->getColor()->at(1),
			subCells[6]->getColor()->at(2),
			},
			{
			subCells[7]->getColor()->at(0),
			subCells[7]->getColor()->at(1),
			subCells[7]->getColor()->at(2),
			},
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));
	*/
	///*
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
		struct
		{
			float r;
			float g;
			float b;
		} color;
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
	for (int i = 0; i < vertices.size(); i++)
	{
		if (subCells[i] != nullptr)
		{
			vertices[i].color.r = subCells[i]->getColor()->at(0);
			vertices[i].color.g = subCells[i]->getColor()->at(1);
			vertices[i].color.b = subCells[i]->getColor()->at(2);

			if(cell==-1)
				cell = i;
		}else
		{
			vertices[i].color.r = 1.0f;
			vertices[i].color.g = 1.0f;
			vertices[i].color.b = 1.0f;
		}
		vertices[i].pos.x += 0.5f;
		vertices[i].pos.y += 0.5f;
		vertices[i].pos.z += 0.5f;
	}
	//short x = subCells[cell]->meshCoords[0];
	//short y = subCells[cell]->meshCoords[1];
	//short z = subCells[cell]->meshCoords[2];
	//this->coords[0] = x - (meshSize[0] / 2);
	//this->coords[1] = y - (meshSize[1] / 2);
	//this->coords[2] = z - (meshSize[1] / 2);
	//this->meshCoords[0] = x;
	//this->meshCoords[1] = y;
	//this->meshCoords[2] = z;
	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	//*/
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));







}

bool ComboCube::ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept
{
	std::vector<DirectX::XMVECTOR> vertices =
	{
		DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f,-1.0f,-1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet( 1.0f,-1.0f,-1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f, 1.0f,-1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet( 1.0f, 1.0f,-1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f,-1.0f, 1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet( 1.0f,-1.0f, 1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f), GetTransformXM()),
		DirectX::XMVector3Transform(DirectX::XMVectorSet( 1.0f, 1.0f, 1.0f, 0.0f), GetTransformXM())
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
	for (int i = 0; i < indecies.size(); i += 3)
	{
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
	return false;
}

std::vector<float> ComboCube::getDetails() const
{
	return std::vector<float>();
}

DirectX::XMMATRIX ComboCube::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(2.0f, 2.0f, 2.0f) *
		subCells[cell]->GetTransformXM();

	/*
	return
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *
		DirectX::XMMatrixTranslation((float)coords[0], (float)coords[1], (float)coords[2])
		;
	//*/
}
