#include "Hexal.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "Topology.h"
#include "InputLayout.h"
#include "TransformCbuf.h"


Hexal::Hexal(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx)
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
		float bok = 0.66; // d³ugoœæ boku
		float a = bok/2.0f; // po³owa d³ugoœci boku
		float b = sqrt(2) * bok; // przek¹tna (kwadratów)
		float c = b/2.0f; // po³owa przek¹tnej (kwadratów)
		float d = sqrt(bok / 1.5208f) ;
		std::vector<Vertex> vertices =
		{
			//{X, Y, Z}
			//punkty górne (0-3) k
			{-a , 1 ,  -a},
			{ a , 1 ,  -a},
			{ a , 1 ,   a},
			{-a,  1 ,   a}, 

			//punkty œrodkowo-górne (4-7) 
			{-d , c ,  -d},
			{ d , c ,  -d},
			{ d , c ,   d},
			{-d,  c ,   d},
			
			//punkty œrodkowe (8-15) k
			{-1 , 0 ,  -a},
			{-a , 0 ,  -1},
			{ a , 0 ,  -1}, // 10
			{ 1,  0 ,  -a}, // 11
			{ 1 , 0 ,   a}, // 12
			{ a , 0 ,   1},
			{-a , 0 ,   1},
			{-1,  0 ,   a},

			//punkty œrodkowo-dolne (16-19)
			{-d , -c ,  -d}, // 4 -> 16
			{ d , -c ,  -d}, // 5 -> 17
			{ d , -c ,   d}, // 6 -> 18
			{-d,  -c ,   d}, // 7 -> 19

			//punkty dolne (20-23) k
			{-a , -1 ,  -a}, // 0 -> 20
			{ a , -1 ,  -a}, // 1 -> 21
			{ a , -1 ,   a}, // 2 -> 22
			{-a,  -1 ,   a}, // 3 -> 23
		};
		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
		//triangle List
		//*
		const std::vector<unsigned short> indecies =
		{
			//góra
			2,1,0,
			3,2,0,

			//góra-œrodek
			0,9,4,
			0,10,9,
			1,10,0,
			10,1,5,

			1,11,5,
			1,2,11,
			2,12,11,
			6,12,2,

			7,14,3,
			3,14,2,
			2,14,13,
			2,13,6,

			4,8,0,
			3,0,8,
			8,15,3,
			3,15,7,

			//œrodek
			4,9,8,
			16,8,9,

			5,11,10,
			17,10,11,

			6,13,12,
			18,12,13,

			7,15,14,
			19,14,15,

			//dó³-œrodek
			20,16,9 ,
			20,9 ,10,
			21,20,10,
			10,17,21,

			21,17,11,
			21,11,22,
			22,11,12,
			18,22,12,

			19,23,14,
			23,22,14,
			22,13,14,
			22,18,13,

			16,20,8 ,
			23,8 ,20,
			8 ,23,15,
			23,19,15,

			//dó³
			20,21,22,
			20,22,23,
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
		//AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_LINELIST));
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
			{1.0f, 1.0f, 0.0f, 1.0f}
		}
	};
	AddBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));
	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

DirectX::XMMATRIX Hexal::GetTransformXM() const noexcept
{
	float size = 1.0f;
	return
		DirectX::XMMatrixScaling(size, size, size) 
		;
}