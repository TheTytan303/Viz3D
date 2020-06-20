#pragma once
#include "Surface.h"
#include "Graphics.h"
#include "Cell.h"
#include "Surface.h"
#include <DirectXMath.h>
#include <memory>

class GridBase
{
public:
	virtual void Slice(std::shared_ptr<Surface> s, bool side) = 0;
	virtual void deSlice() = 0;
	virtual std::shared_ptr<Cell> ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction) = 0;
	
	//virtual void calculateNeighbours()=0;

	virtual void Draw(Graphics& gfx) =0;
	virtual void showVariable(int i, Graphics& gfx) =0;

	//virtual void makeVisableCells(Graphics& gfx, std::shared_ptr<DrawableCell<Cell>>(*cellMaker)(unsigned short* size, std::shared_ptr<Cell>, Graphics& gfx)) = 0;


	virtual float* getMinis() = 0;
	virtual float* getMaxes() = 0;
	virtual void resetColors(Graphics& gfx) = 0;
};