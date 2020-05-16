#pragma once
#include "CubeCell.h"
#include "Graphics.h"

#include <memory>
#include <DirectXMath.h>

using namespace std;
struct CellView
{
	shared_ptr<CubeCell> cell;
	short neighbours;
};



class Grid
{
private:
	shared_ptr<CellView>* cells;
	unsigned short size[3];	// x,y,z
public:
	Grid(unsigned short* size, vector<shared_ptr<CubeCell>> cells);

	unsigned short* getSize()
	{
		return size;
	}
	shared_ptr<CellView> getCellView(unsigned short x, unsigned short y, unsigned short z);
	shared_ptr<CubeCell> getCell(unsigned short x, unsigned short y, unsigned short z);
	shared_ptr<CubeCell> ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction);
	void setCell(unsigned short x, unsigned short y, unsigned short z, shared_ptr<CubeCell> cell);
	void setCell(shared_ptr<CubeCell> cell);
	void Draw(Graphics& Gfx);
	vector<shared_ptr<CubeCell>> getVisableCells();

};