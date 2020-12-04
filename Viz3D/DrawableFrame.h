#pragma once
#include "DrawableBase.h"

template<typename T>
class DrawableFrame : public DrawableBase<T>
{
protected:
	float size;
	float coords[3];
public:
	DrawableFrame(
		float size, float* coords,
		Graphics& gfx)
	{
		this->size = size;
		this->coords[0] = coords[0];
		this->coords[1] = coords[1];
		this->coords[2] = coords[2];
	};

	short* getCoords() const
	{
		//return new short[3] = { coords[0],coords[1],coords[2] };
		return coords;
	}
	short* getsize() const
	{
		return size;
	}
};