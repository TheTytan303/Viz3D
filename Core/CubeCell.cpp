#include "CubeCell.h"
#include "Cell.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "Topology.h"
#include "InputLayout.h"
#include "TransformCbuf.h"
std::vector<float> CubeCell::getColor()
{
	return color;
}

CubeCell::CubeCell(int id, unsigned short* meshSize, unsigned short x, unsigned short y, unsigned short z, std::vector<float> color, std::vector<float> values, Graphics& gfx)
	:
	values(values),
	color(color),
	id(id),
	x(0),
	y(0),
	z(0),
	angleX(0),
	angleZ(0),
	angleY(0),
	size(0.5f)
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
			0,2,1, 2,3,1,	//back
			1,3,5, 3,7,5,	//right
			2,6,3, 3,6,7,	//top
			4,5,7, 4,7,6,	//front
			0,4,2, 2,4,6,	//left
			0,1,4, 1,5,4,	//bottom
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
			{color[0], color[1], color[2], 1.0f}
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));//DONE?
	this->coords[0] = x - (meshSize[0] / 2);
	this->coords[1] = y - (meshSize[1] / 2);
	this->coords[2] = z - (meshSize[2] / 2);
}

int* CubeCell::getCoords()
{
	return coords;
}

int CubeCell::getId()
{
	return id;
}

std::vector<float> CubeCell::getDetails()
{
	return values;
}

DirectX::XMMATRIX CubeCell::GetTransformXM() const noexcept
{
	//float tmpX = (float)(x * 2) / (float)meshSize[0] - 1.0f + (s / 2);
	//float tmpY = (float)(y * 2) / (float)meshSize[1] - 1.0f + (s / 2);
	//float tmpZ = (float)(z * 2) / (float)meshSize[2] - 1.0f + (s / 2);
	return
		DirectX::XMMatrixScaling(size, size, size) *
		DirectX::XMMatrixTranslation(coords[0], coords[1], coords[2]) *
		DirectX::XMMatrixRotationY(angleY) *
		DirectX::XMMatrixRotationX(angleX) *
		DirectX::XMMatrixRotationZ(angleZ) *  
		DirectX::XMMatrixTranslation(x, y, z)
	;
};

void CubeCell::Update(float angleX, float angleY, float angleZ, float x, float y, float z) noexcept
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->angleX = angleX;
	this->angleZ = angleZ;
	this->angleY = angleY;
}


//float s = 2.0f/(float)meshSize[0];
//s *= 0.9f;
//float s = 0.6f;
//float tmpX = (float)(x*2)/ (float)meshSize[0] -1.0f +(s / 2);
//float tmpY = (float)(y * 2) / (float)meshSize[1] - 1.0f + (s / 2);
//float tmpZ = (float)(z * 2) / (float)meshSize[2] - 1.0f + (s / 2);
