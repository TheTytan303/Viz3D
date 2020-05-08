#pragma once
#include "Graphics.h"
#include "DrawableBase.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

class Line : public DrawableBase<Line>
{
	//--------------------=Fields=----------------
private:
	short coordsX[3];			//6B
	short coordsY[3];			//6B
	//18b / cell

	//--------------------=Methods=----------------
public:
	Line(
		std::vector<float> x, std::vector<float> y,
		float red, float blue, float green,
		Graphics& gfx);
	Line(Line& cell) = default;


	// Inherited via Cell

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Update(float angleX, float angleY, float angleZ, float x, float y, float z) noexcept override;
};