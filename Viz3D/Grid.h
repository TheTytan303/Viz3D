#pragma once
#include "CubeCell.h"
#include "Graphics.h"

#include <memory>

using namespace std;

class Grid
{
private:
	shared_ptr<CubeCell>* cells;
	unsigned short size[3];	// x,y,z
public:
	Grid(unsigned short* size, vector<shared_ptr<CubeCell>> cells);

	unsigned short* getSize()
	{
		return size;
	}
	shared_ptr<CubeCell> getCell(unsigned short x, unsigned short y, unsigned short z);
	void setCell(unsigned short x, unsigned short y, unsigned short z, shared_ptr<CubeCell> cell);
	void setCell(shared_ptr<CubeCell> cell);
	void Draw(Graphics& Gfx);

};