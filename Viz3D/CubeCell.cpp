#include "CubeCell.h"
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
//--------------------=Static=----------------
std::shared_ptr<std::vector<float>> CubeCell::getColorOf(int grain) noexcept
{
	return CubeCell::colors.at(grain);
}
bool CubeCell::addColor(int grain) noexcept
{
	if (colors.count(grain)) {
		std::shared_ptr<std::vector<float>> returnVale = std::make_shared<std::vector<float>>();
		for (int i = 0; i < 3; i++) {
			returnVale->push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		}
		colors[grain] = returnVale;
		return true;
	}
	else
	{
		std::shared_ptr<std::vector<float>> returnVale = std::make_shared<std::vector<float>>();
		for (int i = 0; i < 3; i++) {
			returnVale->push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		}
		colors[grain] = returnVale;
		return false;
	}
}
bool CubeCell::setColor(int grain,const std::vector<float> color) noexcept
{
	if (colors.count(grain)) {
		colors[grain] = std::make_shared<std::vector<float>>(color);
		return true;
	}
	else
	{
		colors[grain] = std::make_shared<std::vector<float>>(color);
		return false;
	}
}
void CubeCell::initGrain(int id){
	if (colors.find(id) == colors.end())
	{
		CubeCell::addColor(id);
	}
};
//inits:
float CubeCell::size = 0.5f;
std::map<int, std::shared_ptr<std::vector<float>>> CubeCell::colors = {};



//--------------------=non-Static=----------------

CubeCell::CubeCell(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx)
	:
	Cell(cell)
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
			{-1.0f,-1.0f,-1.0f},
			{ 1.0f,-1.0f,-1.0f},
			{-1.0f, 1.0f,-1.0f},
			{ 1.0f, 1.0f,-1.0f},
			{-1.0f,-1.0f, 1.0f},
			{ 1.0f,-1.0f, 1.0f},
			{-1.0f, 1.0f, 1.0f},
			{ 1.0f, 1.0f, 1.0f},
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
		const std::vector<unsigned short> indecies =
		{
			2,3,
			0,1,
			5,3,
			7,2,
			6,0,
			4,5,
			6,7
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
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP));
	}
	else
	{
		SetIndexFromStatic();
	}
	initGrain(grainID);
	const ColorBuffer cb2 = {
		{
			{
			colors.at(grainID)->at(0),
			colors.at(grainID)->at(1),
			colors.at(grainID)->at(2),
			1.0f
			}
		}
	};
	std::unique_ptr<PixelConstantBuffer<ColorBuffer>> tmp = std::make_unique<PixelConstantBuffer<ColorBuffer>>(gfx, cb2);
	pColorBuffer = tmp.get();
	//pColorBuffer = std::make_unique<PixelConstantBuffer<ColorBuffer>>(gfx, cb2);
	AddBind(move(tmp));


	AddBind(std::make_unique<TransformCbuf>(gfx, *this));

	this->coords[0] = meshCoords[0] - (meshSize[0] / 2);
	this->coords[1] = meshCoords[1] - (meshSize[1] / 2);
	this->coords[2] = meshCoords[2] - (meshSize[2] / 2);
}

std::shared_ptr<std::vector<float>> CubeCell::getColor()
{
	return colors.at(grainID);
}
/*
std::vector<DirectX::XMVECTOR> CubeCell::GetTriangles() const noexcept
{
	struct Vertex
	{
		float x;
		float y;
		float z;
	};
	std::vector<DirectX::XMVECTOR> returnVale;
	std::vector<Vertex> vertices =
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
*/
float CubeCell::ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept
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
	float returnVale = 0.0f;
	float distance = returnVale;
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
		
		distance = 0;
		DirectX::TriangleTests::Intersects(
			origin, direction, 
			vertices[indecies[i]],
			vertices[indecies[i+1]],
			vertices[indecies[i+2]],
			distance
		);
		if (distance != 0)
		{
			if (returnVale == 0)
			{
				returnVale = distance;
			}
			else
			{
				if (distance < returnVale)
				{
					returnVale = distance;
				}
			}
		}
	}
	//DirectX::TriangleTests::Intersects();
	return returnVale;
}

void CubeCell::setColor(std::vector<float> color, Graphics& gfx)
{
	const ColorBuffer cb = {
		{
			{
			color.at(0),
			color.at(1),
			color.at(2),
			1.0f
			}
		}
	};
	pColorBuffer->update(gfx, cb);
}
void CubeCell::resetColor(Graphics& gfx)
{
	const ColorBuffer cb2 = {
		{
			{
			colors.at(grainID)->at(0),
			colors.at(grainID)->at(1),
			colors.at(grainID)->at(2),
			1.0f
			}
		}
	};
	pColorBuffer->update(gfx, cb2);
}



short* CubeCell::getCoords() const
{
	short* returnVale = new short[3];
	returnVale[0] = coords[0];
	returnVale[1] = coords[1];
	returnVale[2] = coords[2];
	return returnVale;
}

DirectX::XMMATRIX CubeCell::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(size, size, size) *
		DirectX::XMMatrixTranslation((float)coords[0], (float)coords[1], (float)coords[2])
	;
};
