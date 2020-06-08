#include "Grid.h"
Grid::Grid(std::shared_ptr<DataMiner> pDataMiner)
{
	size[0] = pDataMiner->GetMeshSize()[0];
	size[1] = pDataMiner->GetMeshSize()[1];
	size[2] = pDataMiner->GetMeshSize()[2];
	const int count = ((int)size[0] * (int)size[1] * (int)size[2]);
	this->cells = new shared_ptr<CellView>[count];
	int values = (int)Cell::getNames().size();
	minis = new float[values];
	maxes = new float[values];
	shared_ptr <Cell> tmp = pDataMiner->GetNextCell();
	for (int i = 0; i < values; i++)
	{
		minis[i] = tmp->getDetails().at(i);
		maxes[i] = tmp->getDetails().at(i);
	}
	setCell(tmp);
	for (int i = 1; i < count; i++)
	{
		tmp = pDataMiner->GetNextCell();
		for (int i = 0; i < values; i++)
		{
			if (tmp->getDetails().at(i) > maxes[i])
			{
				maxes[i] = tmp->getDetails().at(i);
			}
			else if (tmp->getDetails().at(i) < minis[i])
			{
				minis[i] = tmp->getDetails().at(i);
			}
		}
		setCell(tmp);
	}
}

Grid::~Grid()
{
	delete[] cells;
	delete[] minis;
	delete[] maxes;
}

shared_ptr<Cell> Grid::getCell(unsigned short x, unsigned short y, unsigned short z)
{
	if (x >= size[0] || y >= size[1] || z >= size[2] || x <0 || y<0 || z<0)
	{
		return nullptr;
	}
	return cells[(int)x + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)]->cell;
}
shared_ptr<CubeCell> Grid::ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction)
{
	if (this == nullptr)
	{
		return nullptr;
	}
	shared_ptr<CubeCell> returnVale;
	float lastOne = 0.0f;
	for (auto cell: visibles)
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
void Grid::setCell(shared_ptr<Cell> cell)
{
	int x = cell.get()->getMeshCoords()[0];
	int y = cell.get()->getMeshCoords()[1];
	int z = cell.get()->getMeshCoords()[2];
	shared_ptr<CellView> target = make_shared<CellView>();
	target->cell = cell;
	target->neighbours = 126u;
	
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
	//vector<shared_ptr<CubeCell>> tmp = getVisableCells();
	for (auto cell : visibles)
	{
		cell->Draw(Gfx);
	}
}

vector<shared_ptr<CubeCell>> Grid::getVisableCells()
{
	return visibles;
}
vector<shared_ptr<CubeCell>> Grid::makeVisableCells(Graphics& gfx)
{
	if (this == nullptr)
		return vector<shared_ptr<CubeCell>>();

	visibles.clear();
	int count = ((int)size[0] * (int)size[1] * (int)size[2]);
	for (int i = 0; i < count; i++)
	{
		if (cells[i] != nullptr)
		{
			if (outOfBounds(cells[i]->cell))
			{
				continue;
			}
			if (cells[i]->neighbours != 0)
			{
				visibles.push_back( std::make_shared<CubeCell>(size, cells[i]->cell, gfx) );
			}
		}
	}
	return visibles;
}

void Grid::Slice(shared_ptr<Surface> s, bool side)
{
	slices.push_back(
		{
			s,
			side
		}
	);
	int count = ((int)size[0] * (int)size[1] * (int)size[2]);
	for (int i = 0; i < count; i++)
	{
		if (cells[i] == nullptr)
		{
			continue;
		}
		float tmp = s->onSurface(
			{
				(float)cells[i]->cell->getMeshCoords()[0],
				(float)cells[i]->cell->getMeshCoords()[1],
				(float)cells[i]->cell->getMeshCoords()[2],
			}
		);
		if (!side) tmp *= -1;
		if (tmp < 2) 
		{
			cells[i]->neighbours += 128;
		}
	}
}

void Grid::deSlice()
{
	if (slices.size() == 0)
	{
		return;
	}
	int count = ((int)size[0] * (int)size[1] * (int)size[2]);
	for (int i = 0; i < count; i++)
	{
		if (cells[i] == nullptr)
		{
			continue;
		}
		float tmp = slices.at(slices.size() - 1).s->onSurface(
			{
				(float)cells[i]->cell->getMeshCoords()[0],
				(float)cells[i]->cell->getMeshCoords()[1],
				(float)cells[i]->cell->getMeshCoords()[2],
			}
		);
		if (!slices.at(slices.size() - 1).flag) tmp *= -1;
		if (tmp < 2)
		{
			cells[i]->neighbours -= 128;
		}
	}
	slices.erase(slices.end()-1);
}

float* Grid::getMinis()
{
	float* returnVale = new float[sizeof(minis)];
	for (int i = 0; i < sizeof(minis); i++) 
	{
		returnVale[i] = minis[i];
	}
	return returnVale;
}
float* Grid::getMaxes()
{
	float* returnVale = new float[sizeof(maxes)];
	for (int i = 0; i < sizeof(maxes); i++)
	{
		returnVale[i] = maxes[i];
	}
	return returnVale;
}

void Grid::showVariable(int valueNo, Graphics& gfx)
{
	for (auto cc : visibles)
	{
		vector<float> returned = getColor(maxes[valueNo], minis[valueNo], cc->getDetails().at(valueNo));
		cc->setColor(returned, gfx);
	}
}
void Grid::resetColors(Graphics& gfx)
{
	for (auto cc : visibles)
	{
		cc->resetColor(gfx);
	}
}

vector<float> Grid::getColor(float max, float min, float val) {
	val = val - min;
	max = max - min;
	if (max == 0)
		max = 1;
	float hue = val / max;
	//hue *= 120;
	vector<float> color = { hue, 1.0f-hue, 0.0f };
	return color;
}

bool Grid::outOfBounds(shared_ptr<Cell> c) const
{
	for (auto& slice : slices)
	{
		float f = slice.s->onSurface({
			(float)c->getMeshCoords()[0],
			(float)c->getMeshCoords()[1],
			(float)c->getMeshCoords()[2],
			});
		if (slice.flag == true && f < 0 )
		{
			return true;
		}
	}
	return false;
}