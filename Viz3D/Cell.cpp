#include "Cell.h"



//--------------------=Static=----------------
std::vector<std::string> Cell::getNames() noexcept
{
	return valuesNames;
}
void Cell::addName(std::string name) noexcept
{
	Cell::valuesNames.push_back(name);
}
void Cell::setNames(std::vector<std::string> names) noexcept
{
	Cell::valuesNames = names;
}
// ------- INITS ------
std::vector<std::string> Cell::valuesNames = {};

unsigned short* Cell::getMeshCoords() const
{
	int n = sizeof(meshCoords);
	unsigned short* returnVale = new unsigned short[n];
	for (int i = 0; i < n; i++)
	{
		returnVale[i] = meshCoords[i];
	}
	return returnVale;
}

int Cell::getGrain() const
{
	return grainID;
}

std::vector<float> Cell::getDetails() const
{
	return values;
}

Cell::Cell(unsigned short x, unsigned short y, unsigned short z, int grainID, std::vector<float> values)
	:
	grainID(grainID),
	values(values)
{
	meshCoords[0] = x;
	meshCoords[1] = y;
	meshCoords[2] = z;
}

Cell::Cell(Cell& cell)
	:
	grainID(cell.grainID),
	values(cell.values)
{
	meshCoords[0] = cell.meshCoords[0];
	meshCoords[1] = cell.meshCoords[1];
	meshCoords[2] = cell.meshCoords[2];
}

Cell::Cell(std::shared_ptr<Cell> cell)
	:
	grainID(cell->grainID),
	values(cell->values)
{
	meshCoords[0] = cell->meshCoords[0];
	meshCoords[1] = cell->meshCoords[1];
	meshCoords[2] = cell->meshCoords[2];
}
