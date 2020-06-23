#pragma once
#include "DrawableCell.h"
#include "Graphics.h"
#include "DataMiner.h"
#include "Surface.h"
#include "GridBase.h"
#include <memory>
#include <DirectXMath.h>
#include <type_traits>

using namespace std;
struct Slices
{
	shared_ptr<Surface> s;
	bool flag;
};

using namespace std;
template<typename T, typename F>
class Grid : public GridBase
{
private:
	shared_ptr<CellView>* cells;
	vector<shared_ptr<T>> visibles;
	
	std::shared_ptr<Cell> picked;
	vector<std::shared_ptr<Cell>> pickedCells;

	vector<shared_ptr<CubeFrame>> frames;
	vector<Slices> slices;
	float* minis;
	float* maxes;
	unsigned short size[3];	// x,y,z
	//unsigned short bounds[6]; // mesh: x,x,y,y,z,z
	//vector<> surfaces;
public:
	//---------------Constructors---------------

	Grid(std::shared_ptr<DataMiner> pDataMiner)
	{
		if (std::is_same<T, ::DrawableCell>::value != true)
		{
			throw "Need DrawableCell<T> extended class";
		}
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
	};
	Grid(std::shared_ptr<DataMiner> pDataMiner,
		shared_ptr<CellView>(*cellSetter)(shared_ptr<Cell> cell, GridBase* base))
	{
		if (std::is_same<T, ::DrawableCell<T>>::value)
		{
			throw "Need DrawableCell<T> extended class";
		}
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
	};

	~Grid()
	{
		delete[] cells;
		delete[] minis;
		delete[] maxes;
	};


	//---------------Methods---------------
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
	void setCellView(shared_ptr<CellView> target) override
	{
		int x = target->cell->getMeshCoords()[0];
		int y = target->cell->getMeshCoords()[1];
		int z = target->cell->getMeshCoords()[2];
		cells[((int)x) + ((int)size[0] * (int)y) + ((int)size[1] * (int)size[0] * (int)z)] = target;
	};
	vector<shared_ptr<T>> getVisableCells()
	{
		return visibles;
	};
	void makeVisableCells(Graphics& gfx, shared_ptr<T>(*cellMaker)(unsigned short* size, std::shared_ptr<Cell>, Graphics& gfx))
	{
		if (this == nullptr)
			return;
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
					visibles.push_back(move(cellMaker(size, cells[i]->cell, gfx)));
				}
			}
		}
	};

	//Inharited via GridBase
	void Slice(shared_ptr<Surface> s, bool side) override
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
			if (tmp < 1.0f && tmp > -1)
			{
				cells[i]->neighbours += 128;
			}
		}
	};
	void deSlice() override
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
			if (tmp < 1.0f && tmp > -1)
			{
				cells[i]->neighbours -= 128;
			}
		}
		slices.erase(slices.end() - 1);
	};
	shared_ptr<Cell> ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction) override
	{
		if (this == nullptr)
		{
			return nullptr;
		}
		shared_ptr<T> returnVale;
		float lastOne = 0.0f;
		for (auto cell : visibles)
		{
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
			return std::dynamic_pointer_cast<Cell>(returnVale);
		return nullptr;
	};

	void showVariable(int valueNo, Graphics& gfx) override
	{
		for (auto cc : visibles)
		{
			vector<float> returned = getColor(maxes[valueNo], minis[valueNo], cc->getDetails().at(valueNo));
			cc->setColor(returned, gfx);
		}
	};
	void resetColors(Graphics& gfx) override
	{
		for (auto cc : visibles)
		{
			cc->resetColor(gfx);
		}
	};

	float* getWorldCoords(Cell& c) const override
	{
		float returnVale[3] = {0,0,0};
		for (auto cell : visibles)
		{
			//TODO
		}
		return returnVale;
	}
	float* getMinis() override
	{
		float* returnVale = new float[sizeof(minis)];
		for (int i = 0; i < sizeof(minis); i++)
		{
			returnVale[i] = minis[i];
		}
		return returnVale;
	};
	float* getMaxes() override
	{
		float* returnVale = new float[sizeof(maxes)];
		for (int i = 0; i < sizeof(maxes); i++)
		{
			returnVale[i] = maxes[i];
		}
		return returnVale;
	};
	void Draw(Graphics& Gfx)override
	{
		for (auto cell : visibles)
		{
			cell->Draw(Gfx);
		}
		for (auto frame : frames)
		{
			frame->Draw(Gfx);
		}
	};
	virtual std::vector<std::shared_ptr<Cell>> getPickedCells() override
	{
		return pickedCells;
	};
	virtual void clearPickedCells() override
	{
		pickedCells.clear();
		frames.clear();
	};
	virtual std::shared_ptr<Cell> pickCell(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, Graphics& gfx) override
	{
		if (this == nullptr)
		{
			return nullptr;
		}
		shared_ptr<T> returnVale;
		float lastOne = 0.0f;
		for (auto cell : visibles)
		{
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
		{
			frames.push_back(returnVale->getFrame(0.0f, 0.0f, 0.0f, gfx));

			pickedCells.push_back(std::dynamic_pointer_cast<Cell>(returnVale));
			return std::dynamic_pointer_cast<Cell>(returnVale);
		}
		return nullptr;
	};


private:
	vector<float> getColor(float max, float min, float val) {
		val = val - min;
		max = max - min;
		if (max == 0)
			max = 1;
		float hue = val / max;
		//hue *= 120;
		vector<float> color = { hue, 1.0f - hue, 0.0f };
		return color;
	};
	bool outOfBounds(shared_ptr<Cell> c) const
	{
		for (auto& slice : slices)
		{
			float f = slice.s->onSurface({
				(float)c->getMeshCoords()[0],
				(float)c->getMeshCoords()[1],
				(float)c->getMeshCoords()[2],
				});
			if (slice.flag == true && f < 0)
			{
				return true;
			}
			if (slice.flag == false && f > 0)
			{
				return true;
			}
		}
		return false;
	};
};