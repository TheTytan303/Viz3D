#include "HexalFrame.h"


HexalFrame::HexalFrame(
	float size,
	unsigned short* meshSize,
	unsigned short x, unsigned short y, unsigned short z,
	float red, float blue, float green,
	Graphics& gfx)
	:
	DrawableFrame(size, coords, gfx)
{
	if (!isStaticInitialized()) {
		struct Vertex {
			struct {
				float x;
				float y;
				float z;
			} pos;
		};
		float bok = 0.66f; // d³ugoœæ boku
		float a = bok / 2.0f; // po³owa d³ugoœci boku
		float b = (float)sqrt(2) * bok; // przek¹tna (kwadratów)
		float c = b / 2.0f; // po³owa przek¹tnej (kwadratów)
		float d = (float)sqrt(bok / 1.5208f);
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
		
		//line LIST
		const std::vector<unsigned short> indecies =
		{
			//góra
			0,1,
			1,2,
			2,3,
			3,0,

			//góra, góra - œrodek
			0,4,
			1,5,
			2,6,
			3,7,

			//góra-œrodek, œrodek
			4,8,
			4,9,
			5,10,
			5,11,
			6,12,
			6,13,
			7,14,
			7,15,

			//œrodek
			9,10,
			11,12,
			13,14,
			15,8,

			//dó³-œrodek, œrodek
			16,8,
			16,9,
			17,10,
			17,11,
			18,12,
			18,13,
			19,14,
			19,15,

			//dó³, dó³ - œrodek
			20,16,
			21,17,
			22,18,
			23,19,

			//dó³
			20,21,
			21,22,
			22,23,
			23,20,
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
	int xx = (x - (meshSize[0] / 2));
	int yy = (y - (meshSize[1] / 2));
	int zz = (z - (meshSize[2] / 2));
	this->coords[0] = (float)xx;
	this->coords[1] = (float)yy;
	this->coords[2] = (float)zz;
	if (xx % 2 != 0)
	{
		this->coords[2] += 0.5f;
		this->coords[1] += 0.5f;
	}
	if (yy % 2 != 0)
	{
		this->coords[0] += 0.5f;
		this->coords[2] += 0.5f;
	}
	if (zz % 2 != 0)
	{
		this->coords[1] += 0.5f;
		this->coords[0] += 0.5f;
	}
	this->coords[1] /= 2;
	this->size = size;
}
HexalFrame::HexalFrame(
	float size,
	float coords[3],
	float red, float blue, float green,
	Graphics& gfx)
	:
	DrawableFrame(size, coords, gfx)
{
	if (!isStaticInitialized()) {
		struct Vertex {
			struct {
				float x;
				float y;
				float z;
			} pos;
		};
		float bok = 0.66f; // d³ugoœæ boku
		float a = bok / 2.0f; // po³owa d³ugoœci boku
		float b = (float)sqrt(2) * bok; // przek¹tna (kwadratów)
		float c = b / 2.0f; // po³owa przek¹tnej (kwadratów)
		float d = (float)sqrt(bok / 1.5208f);
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

		//line LIST
		const std::vector<unsigned short> indecies =
		{
			//góra
			0,1,
			1,2,
			2,3,
			3,0,

			//góra, góra - œrodek
			0,4,
			1,5,
			2,6,
			3,7,

			//góra-œrodek, œrodek
			4,8,
			4,9,
			5,10,
			5,11,
			6,12,
			6,13,
			7,14,
			7,15,

			//œrodek
			9,10,
			11,12,
			13,14,
			15,8,

			//dó³-œrodek, œrodek
			16,8,
			16,9,
			17,10,
			17,11,
			18,12,
			18,13,
			19,14,
			19,15,

			//dó³, dó³ - œrodek
			20,16,
			21,17,
			22,18,
			23,19,

			//dó³
			20,21,
			21,22,
			22,23,
			23,20,
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
	this->coords[0] = coords[0];
	this->coords[1] = coords[1];
	this->coords[2] = coords[2];
	this->size = size;
}

DirectX::XMMATRIX HexalFrame::GetTransformXM() const noexcept
{
	return
		DirectX::XMMatrixScaling(size, size, size) *
		DirectX::XMMatrixRotationY(0.7853f) *
		DirectX::XMMatrixTranslation((float)coords[0], (float)coords[1], (float)coords[2])
		;
}