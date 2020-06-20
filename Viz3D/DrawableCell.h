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

template<typename T>
class DrawableCell : public Cell, public DrawableBase<T>
{
//--------------------=Static=----------------
protected:
	static std::map<int, std::shared_ptr<std::vector<float>>> colors;
public:
	static std::shared_ptr<std::vector<float>> getColorOf(int grain) noexcept 
	{
		return DrawableCell::colors.at(grain);
	};
	static bool addColor(int grain) noexcept
	{
		if (colors.count(grain)) {
			std::shared_ptr<std::vector<float>> returnVale = std::make_shared<std::vector<float>>();
			for (int i = 0; i < 3; i++) {
				returnVale->push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
			}
			colors[grain] = returnVale;
			return true;
		}
		else
		{
			std::shared_ptr<std::vector<float>> returnVale = std::make_shared<std::vector<float>>();
			for (int i = 0; i < 3; i++) {
				returnVale->push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
			}
			colors[grain] = returnVale;
			return false;
		}
	};
	static bool setColor(int grain, std::vector<float> color) noexcept
	{
		if (colors.count(grain)) {
			colors[grain] = std::make_shared<std::vector<float>>(color);
			return true;
		}
		else
		{
			colors[grain] = std::make_shared<std::vector<float>>(color);
			return false;
		}
	};
	static void initGrain(int id) {
		if (colors.find(id) == colors.end())
		{
			DrawableCell::addColor(id);
		}
	};
	static float size;
	static float surfaceDistanceRendering;

//--------------------=Fields=----------------
protected:
	float coords[3];
	ConstantBuffer<ColorBuffer>* pColorBuffer;

//--------------------=Methods=-----------------
public:
	DrawableCell(Cell cell)
		:
		Cell(cell)
	{
		DrawableCell::initGrain(cell.getGrain());
	};
	void setColor(std::vector<float> color, Graphics& gfx)
	{
		const ColorBuffer cb = {
			{
				{
				color.at(0),
				color.at(1),
				color.at(2),
				1.0f
				}
			}
		};
		pColorBuffer->update(gfx, cb);
	};
	void resetColor(Graphics& gfx)
	{
		const ColorBuffer cb2 = {
			{
				{
				colors.at(grainID)->at(0),
				colors.at(grainID)->at(1),
				colors.at(grainID)->at(2),
				1.0f
				}
			}
		};
		pColorBuffer->update(gfx, cb2);
	};
//--------------------=Virtuals=----------------
	virtual float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept = 0;
	virtual float* getCoords() const = 0;
	//virtual
};

template<typename T>
float DrawableCell<T>::size = 0.5f;

template<typename T>

std::map<int, std::shared_ptr<std::vector<float>>> DrawableCell<T>::colors = {};