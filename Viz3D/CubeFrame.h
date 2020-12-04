#pragma once

#include "DrawableFrame.h"
#include <cstdlib>
#include <ctime>
#include <memory>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

class CubeFrame : public DrawableFrame<CubeFrame>
{
	//--------------------=Fields=----------------
private:
	static float size;			//4B
	float coords[3];			//6B
	//18b / cell

	//--------------------=Methods=----------------
public:
	CubeFrame(
		unsigned short* meshSize,
		unsigned short x, unsigned short y, unsigned short z,
		float red, float blue, float green,
		Graphics& gfx);
	CubeFrame(
		float* coords,
		float red, float blue, float green,
		Graphics& gfx);
	CubeFrame(CubeFrame& cell) = default;


	std::vector<DirectX::XMVECTOR> GetTriangles() const noexcept;
	bool ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept;


	// Inherited via Cell
	float* getCoords() const;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};