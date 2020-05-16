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
std::vector<std::string> CubeCell::getNames() noexcept
{
	return valuesNames;
}
void CubeCell::addName(std::string name) noexcept
{
	CubeCell::valuesNames.push_back(name);
}
void CubeCell::setNames(std::vector<std::string> names) noexcept
{
	CubeCell::valuesNames = names;
}
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
//inits:
float CubeCell::size = 0.5f;
std::map<int, std::shared_ptr<std::vector<float>>> CubeCell::colors = {};
std::vector<std::string> CubeCell::valuesNames = {};


//--------------------=non-Static=----------------
std::shared_ptr<std::vector<float>> CubeCell::getColor()
{
	return colors.at(grain);
}
std::vector<DirectX::XMVECTOR> CubeCell::GetTriangles() const noexcept
{
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
CubeCell::CubeCell(
	unsigned short* meshSize, 
	unsigned short x, unsigned short y, unsigned short z, 
	int grain, std::vector<float> values, Graphics& gfx)
	:
	values(values),
	grain(grain)
	//color(color),
	//id()
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
		/*
		const std::vector<unsigned short> indecies =
		{
			0,2,1, 2,3,1,	//back
			1,3,5, 3,7,5,	//right
			2,6,3, 3,6,7,	//top
			4,5,7, 4,7,6,	//front
			0,4,2, 2,4,6,	//left
			0,1,4, 1,5,4,	//bottom
		};
		//*/
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
		//*/
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indecies));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));
		auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			//{"SV_InstanceID",0,DXGI_FORMAT_R32_UINT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		//AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP));
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
			{colors.at(grain)->at(0), colors.at(grain)->at(1), colors.at(grain)->at(2), 1.0f}
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));



	this->coords[0] = x - (meshSize[0] / 2);
	this->coords[1] = y - (meshSize[1] / 2);
	this->coords[2] = z - (meshSize[1] / 2);

	this->meshCoords[0] = x;
	this->meshCoords[1] = y;
	this->meshCoords[2] = z;
}

short* CubeCell::getCoords() const
{
	return (short*)coords;
}
int CubeCell::getId() const
{
	return 1;
}
int CubeCell::getGrain() const
{
	return grain;
}

std::vector<float> CubeCell::getDetails() const
{
	return values;
}

DirectX::XMMATRIX CubeCell::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(size, size, size) *
		DirectX::XMMatrixTranslation((float)coords[0], (float)coords[1], (float)coords[2])
	;
};
