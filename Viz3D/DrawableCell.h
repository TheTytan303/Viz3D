#pragma once

#include "Cell.h"
#include "ConstantBuffer.h"
#include "Graphics.h"
#include "DrawableBase.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>


class DrawableCell : public Cell, public DrawableBase<DrawableCell>
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
	static float surfaceDistanceRendering;

//--------------------=Fields=----------------
protected:
	float coords[3];
	ConstantBuffer<ColorBuffer>* pColorBuffer;

//--------------------=Methods=-----------------
public:
	DrawableCell(Cell cell);
	void setColor(std::vector<float> color, Graphics& gfx);
	void resetColor(Graphics& gfx);
//--------------------=Virtuals=----------------
	virtual float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept = 0;
	virtual float* getCoords() const = 0;
};