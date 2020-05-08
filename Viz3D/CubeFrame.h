#pragma once
#pragma once
#include "Cell.h"
#include "Graphics.h"
#include "DrawableBase.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

class CubeFrame : public DrawableBase<CubeFrame>
{
	//--------------------=Fields=----------------
private:
	static float size;			//4B
	short coords[3];			//6B
	//18b / cell

	//--------------------=Methods=----------------
public:
	CubeFrame(
		unsigned short* meshSize,
		unsigned short x, unsigned short y, unsigned short z,
		float red, float blue, float green,
		Graphics& gfx);
	CubeFrame(CubeFrame& cell) = default;


	std::vector<DirectX::XMVECTOR> GetTriangles() const noexcept;
	bool ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept;


	// Inherited via Cell
	short* getCoords() const;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Update(float angleX, float angleY, float angleZ, float x, float y, float z) noexcept override;
};