#pragma once
#include "ConstantBuffer.h"
#include "DrawableFrame.h"


class HexalFrame : public DrawableFrame<HexalFrame>
{


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