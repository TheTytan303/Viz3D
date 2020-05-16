#include "Grid.h"

Grid::Grid(unsigned short* size, vector<shared_ptr<CubeCell>> cells)
{
	this->size[0] = size[0];
	this->size[1] = size[1];
	this->size[2] = size[2];
	const int count = ((int)size[0] * (int)size[1] * (int)size[2]);
	this->cells = new shared_ptr<CellView>[count];
	for (int i = 0; i < cells.size(); i++)
	{
		setCell(cells[i]);
	}
}

shared_ptr<CubeCell> Grid::getCell(unsigned short x, unsigned short y, unsigned short z)
{
	if (x >= size[0] || y >= size[1] || z >= size[2] || x <0 || y<0 || z<0)
	{
		return nullptr;
	}
	return cells[(int)x + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)]->cell;
}
shared_ptr<CubeCell> Grid::ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction)
{
	shared_ptr<CubeCell> returnVale;


	vector<shared_ptr<CubeCell>> visables = getVisableCells();
	float lastOne = 0.0f;
	for (auto cell: visables)
	{
		//cell->GetTransformXM();
		float hitDistance = cell->ifHit(origin, direction, 0);
		if (hitDistance != 0)
		{
			if (lastOne == 0.0)
			{
				returnVale = cell;
				lastOne = hitDistance;
			}
			else
			{
				if (lastOne > hitDistance)
				{
					returnVale = cell;
					lastOne = hitDistance;
				}
			}
		}
	}
	if (lastOne != 0.0f)
		return returnVale;
	return nullptr;
}
shared_ptr<CellView> Grid::getCellView(unsigned short x, unsigned short y, unsigned short z)
{
	if (x >= size[0] || y >= size[1] || z >= size[2] || x <0 || y<0 || z<0)
	{
		return nullptr;
	}
	return cells[(int)x + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)];
}
void Grid::setCell(unsigned short x, unsigned short y, unsigned short z, shared_ptr<CubeCell> cell)
{
	shared_ptr<CellView> tmp = make_shared<CellView>();
	tmp->cell = cell;
	tmp->neighbours = 64u; // 6 œcian = 2^6 = 64
	// TODO
	cells[((int)x )+ ((int)size[0] * (int)y) +((int)size[1] * (int)size[0] * (int)z)] = tmp;
}
void Grid::setCell(shared_ptr<CubeCell> cell)
{
	int x = cell.get()->meshCoords[0];
	int y = cell.get()->meshCoords[1];
	int z = cell.get()->meshCoords[2];
	shared_ptr<CellView> target = make_shared<CellView>();
	target->cell = cell;
	target->neighbours = 126u; // 6 œcian = 2^6 = 64
	
	//Left Right Bottom Top Front Back
	//LRTBFB
		
	//1111110
	//64 32 16 8 4 2 0

	shared_ptr<CellView> tmp;

	// X
	//LEFT
	tmp = getCellView(x-1,y,z);
	if (tmp != nullptr)
	{
		target->neighbours -= 64;
		tmp->neighbours -=32;
	}
	//RIGHT
	tmp = getCellView(x + 1, y, z);
	if (tmp != nullptr)
	{
		target->neighbours -= 32;
		tmp->neighbours -= 64;
	}

	// Y
	//BOTTOM
	tmp = getCellView(x, y-1, z);
	if (tmp != nullptr)
	{
		target->neighbours -= 16;
		tmp->neighbours -= 8;
	}
	//TOP
	tmp = getCellView(x, y+1, z);
	if (tmp != nullptr)
	{
		target->neighbours -= 8;
		tmp->neighbours -= 16;
	}

	// Z
	//FRONT
	tmp = getCellView(x, y, z-1);
	if (tmp != nullptr)
	{
		target->neighbours -= 4;
		tmp->neighbours -= 2;
	}
	//BACK
	tmp = getCellView(x , y, z+1);
	if (tmp != nullptr)
	{
		target->neighbours -= 2;
		tmp->neighbours -= 4;
	}

	cells[((int)x) + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)] = target;
}
void Grid::Draw(Graphics& Gfx)
{
	vector<shared_ptr<CubeCell>> tmp = getVisableCells();
	for (auto cell : tmp)
	{
		cell->Draw(Gfx);
	}
}
vector<shared_ptr<CubeCell>> Grid::getVisableCells()
{
	vector<shared_ptr<CubeCell>> returnVale;


	if (this == nullptr)
		return vector<shared_ptr<CubeCell>>();


	int count = ((int)size[0] * (int)size[1] * (int)size[2]);
	for (int i = 0; i < count; i++)
	{
		if (cells[i] != nullptr)
		{
			/*
			unsigned short x = cells[i]->meshCoords[0];
			unsigned short y = cells[i]->meshCoords[1];
			unsigned short z = cells[i]->meshCoords[2];
			if (x == 0 || y == 0 || z == 0 ||
				x == this->size[0] - 1 || y == this->size[1] - 1 ||
				z == this->size[2] - 1)
			{
				returnVale.push_back(cells[i]);
			};
			*/
			if (cells[i]->neighbours != 0)
			{
				returnVale.push_back(cells[i]->cell);
			}
		}
	}
	return returnVale;
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