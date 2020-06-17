#pragma once
#include "ConstantBuffer.h"
#include "Cell.h"
#include "Graphics.h"
#include "DrawableBase.h"

struct ColorBuffer
{
	struct
	{
		float r;
		float g;
		float b;
		float a;
	}face_colors[1];
};

class Hexal : public Cell, public DrawableBase<Hexal>
{
public:
	Hexal(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx);

private:
	short coords[3];
	ConstantBuffer<ColorBuffer>* pColorBuffer;


	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};