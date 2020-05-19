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

class CubeCell : public Cell, public DrawableBase<CubeCell>
{
//--------------------=Static=----------------
protected:
	static std::map<int, std::shared_ptr<std::vector<float>>> colors;
public:
	static std::shared_ptr<std::vector<float>> getColorOf(int grain) noexcept;
	static bool addColor(int grain) noexcept;
	static bool setColor(int grain, std::vector<float> color) noexcept;
	static void initGrain(int id);
	static float size;			
//--------------------=Fields=----------------
private:
	short coords[3];

//--------------------=Methods=----------------
public:
	CubeCell(unsigned short* meshSize,
		unsigned short x, unsigned short y, unsigned short z,
		int grain, std::vector<float> values,
		Graphics& gfx);
	CubeCell(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx);


	CubeCell(CubeCell& cell) = default;
	std::shared_ptr<std::vector<float>> getColor();

	//std::vector<DirectX::XMVECTOR> GetTriangles() const noexcept;
	float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept;

	short* getCoords() const;


	// Inherited via Cell:
	//unsigned short* getMeshCoords() const;
	//int getGrain() const override;
	//std::vector<float> getDetails() const override;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};
