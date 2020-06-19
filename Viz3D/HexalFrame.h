#pragma once
#include "ConstantBuffer.h"
#include "Cell.h"
#include "Graphics.h"
#include "DrawableBase.h"


class HexalFrame : public DrawableBase<HexalFrame>
{

	//--------------------=Fields=----------------
private:
	float size;			//4B
	float coords[3];			//6B
public:
	HexalFrame(
		float size,
		unsigned short* meshSize,
		unsigned short x, unsigned short y, unsigned short z,
		float red, float blue, float green,
		Graphics& gfx);
	HexalFrame(
		float size,
		float coords[3],
		float red, float blue, float green,
		Graphics& gfx);




	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};