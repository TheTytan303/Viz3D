#pragma once
#include "DrawableBase.h"

class Star : public DrawableBase<Star>
{
	//--------------------=Fields=----------------
private:
	short coordsX[3];			//6B
	short coordsY[3];			//6B

	//--------------------=Methods=----------------
public:
	Star(
		std::vector<float> coords,
		float red, float blue, float green,
		Graphics& gfx);
	Star(Star& cell) = default;


	// Inherited via Cell

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};