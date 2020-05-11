#include "Grid.h"

Grid::Grid(unsigned short* size, vector<shared_ptr<CubeCell>> cells)
{
	this->size[0] = size[0];
	this->size[1] = size[1];
	this->size[2] = size[2];
	const int count = ((int)size[0] * (int)size[1] * (int)size[2]);
	this->cells = new shared_ptr<CubeCell>[count];
	for (int i = 0; i < cells.size(); i++)
	{
		setCell(cells[i]);
	}
}

shared_ptr<CubeCell> Grid::getCell(unsigned short x, unsigned short y, unsigned short z)
{
	if (x >= size[0] || y >= size[1] || z >= size[2])
	{
		throw "out of bonds";
	}
	return cells[(int)x + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)];
}
void Grid::setCell(unsigned short x, unsigned short y, unsigned short z, shared_ptr<CubeCell> cell)
{
	cells[((int)x )+ ((int)size[0] * (int)y) +((int)size[1] * (int)size[0] * (int)z)] = cell;
}
void Grid::setCell(shared_ptr<CubeCell> cell)
{
	int x = cell.get()->meshCoords[0];
	int y = cell.get()->meshCoords[1];
	int z = cell.get()->meshCoords[2];
	cells[x + ((int)size[0] * y )+ ((int)size[1]* (int)size[0] * z)] = cell;
}
void Grid::Draw(Graphics& Gfx)
{
	int count = ((int)size[0] * (int)size[1] * (int)size[2]);
	for (int i = 0; i < count; i++)
	{
		if (cells[i] != nullptr)
		{
			cells[i]->Draw(Gfx);
		}
	}
}
/*
size:
x = 10
y = 10
z = 10

count = 1000

(0,0,0) -> cells(0)
(1,0,0) -> cells(1)
(0,1,0) -> cells(10)
(0,0,1) -> cells(100)

(x,y,z) -> cells( x + 10*y + 100*z );

*/