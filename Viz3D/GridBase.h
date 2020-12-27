#pragma once
#include "DataMiner.h"
#include "Surface.h"
#include "Graphics.h"
#include "Cell.h"
#include "Surface.h"
#include <DirectXMath.h>
#include <memory>


struct CellView
{
	std::shared_ptr<Cell> cell;
	short neighbours;
};

using namespace std;
class GridBase
{
public:
	shared_ptr<CellView>* cells;
	float* minis;
	float* maxes;
	unsigned short size[3];	// x,y,z

public:
	GridBase(std::shared_ptr<DataMiner> pDataMiner,
		shared_ptr<CellView>(*cellSetter)(shared_ptr<Cell> cell, GridBase* base))
	{

		//TODO null DataMiner
		size[0] = pDataMiner->GetMeshSize()[0];
		size[1] = pDataMiner->GetMeshSize()[1];
		size[2] = pDataMiner->GetMeshSize()[2];

		const int count = ((int)size[0] * (int)size[1] * (int)size[2]);
		this->cells = new shared_ptr<CellView>[count];
		int values = (int)Cell::getNames().size();
		minis = new float[values];
		maxes = new float[values];
		shared_ptr <Cell> tmp = pDataMiner->GetCellAt(0);
		for (int i = 0; i < values; i++)
		{
			minis[i] = tmp->getDetails().at(i);
			maxes[i] = tmp->getDetails().at(i);
		}
		setCellView(cellSetter(tmp, this));
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
			setCellView(cellSetter(tmp, this));
		}
	}

	void setCell(shared_ptr<Cell> cell)
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
		tmp = getCellView(x - 1, y, z);
		if (tmp != nullptr)
		{
			target->neighbours -= 64;
			tmp->neighbours -= 32;
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
		tmp = getCellView(x, y - 1, z);
		if (tmp != nullptr)
		{
			target->neighbours -= 16;
			tmp->neighbours -= 8;
		}
		//TOP
		tmp = getCellView(x, y + 1, z);
		if (tmp != nullptr)
		{
			target->neighbours -= 8;
			tmp->neighbours -= 16;
		}

		// Z
		//FRONT
		tmp = getCellView(x, y, z - 1);
		if (tmp != nullptr)
		{
			target->neighbours -= 4;
			tmp->neighbours -= 2;
		}
		//BACK
		tmp = getCellView(x, y, z + 1);
		if (tmp != nullptr)
		{
			target->neighbours -= 2;
			tmp->neighbours -= 4;
		}
		setCellView(target);
	};
	void setCellView(shared_ptr<CellView> target) 
	{
		int x = target->cell->getMeshCoords()[0];
		int y = target->cell->getMeshCoords()[1];
		int z = target->cell->getMeshCoords()[2];
		cells[((int)x) + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)] = target;
	};
	unsigned short* getSize()
	{
		return size;
	}
	shared_ptr<CellView> getCellView(unsigned short x, unsigned short y, unsigned short z)
	{
		if (x >= size[0] || y >= size[1] || z >= size[2] || x < 0 || y < 0 || z < 0)
		{
			return nullptr;
		}
		return cells[(int)x + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)];
	};
	shared_ptr<Cell> getCell(unsigned short x, unsigned short y, unsigned short z)
	{
		if (x >= size[0] || y >= size[1] || z >= size[2] || x < 0 || y < 0 || z < 0)
		{
			return nullptr;
		}
		return cells[(int)x + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)]->cell;
	};


	virtual void Slice(std::shared_ptr<Surface> s, bool side) = 0;
	virtual void deSlice() = 0;

	virtual void addPickedGrain(int grainID) = 0;
	virtual void removePickedGrain(int grainID) = 0;
	virtual void earsePickedGrains() = 0;

	virtual std::shared_ptr<Cell> ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction) = 0;
	
	//virtual std::shared_ptr<CellView> getCellView(unsigned short x, unsigned short y, unsigned short z) = 0;
	//virtual std::shared_ptr<Cell> getCell(unsigned short x, unsigned short y, unsigned short z) = 0;
	//virtual void setCellView(std::shared_ptr<CellView> target) = 0;

	virtual std::vector<std::shared_ptr<Cell>> getPickedCells() =0;
	virtual void clearPickedCells() = 0;
	virtual std::shared_ptr<Cell> pickCell(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, Graphics& gfx) = 0;



	virtual void Draw(Graphics& gfx) =0;
	virtual void showVariable(int i, Graphics& gfx) =0;

	virtual float* getWorldCoords(Cell& c) const = 0;//TODO


	//virtual float* getMinis() = 0;
	//virtual float* getMaxes() = 0;
	float* getMinis()
	{
		return minis;
	};
	float* getMaxes()
	{
		return maxes;
	};
	virtual void resetColors(Graphics& gfx) = 0;
};