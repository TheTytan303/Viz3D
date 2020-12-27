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
	vector<shared_ptr<T>> visibles;

	std::shared_ptr<Cell> picked;
	vector<std::shared_ptr<Cell>> pickedCells;

	vector<shared_ptr<CubeFrame>> frames;
	vector<Slices> slices;
	vector<int> pickedGrains;
	//unsigned short bounds[6]; // mesh: x,x,y,y,z,z
	//vector<> surfaces;
public:
	//---------------Constructors---------------

	Grid(std::shared_ptr<DataMiner> pDataMiner)
		:
		Grid(pDataMiner,CubeCellSetter)
	{
	};
	Grid(std::shared_ptr<DataMiner> pDataMiner,
		shared_ptr<CellView>(*cellSetter)(shared_ptr<Cell> cell, GridBase* base))
		:
		GridBase(pDataMiner, cellSetter)
	{
		if (std::is_same<T, ::DrawableCell<T>>::value)
		{
			throw "Need DrawableCell<T> extended class";
		}
	};
	~Grid()
	{
		//delete[] cells;
		const int count = ((int)size[0] * (int)size[1] * (int)size[2]);
		for (int i = 0; i < count; i++)
		{
			cells[i] = nullptr;
		}
		delete[] minis;
		delete[] maxes;
	};


	//---------------Methods---------------

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
				if (pickedGrains.size() != 0)
				{
					if (!pickedGrain(cells[i]->cell->getGrain()))
					{
						continue;
					}
				}
				if (cells[i]->neighbours != 0)
				{
					visibles.push_back(move(cellMaker(size, cells[i]->cell, gfx)));
				}
			}
		}
	};
	void makeVisableCells(Graphics& gfx)
	{
		this->makeVisableCells(gfx, makeCubeCell);
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
					(float)this->cells[i]->cell->getMeshCoords()[0],
					(float)this->cells[i]->cell->getMeshCoords()[1],
					(float)this->cells[i]->cell->getMeshCoords()[2],
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

	virtual void addPickedGrain(int grainID)
	{
		pickedGrains.push_back(grainID);
		int count = ((int)size[0] * (int)size[1] * (int)size[2]);
		for (int i = 0; i < count; i++)
		{
			if (cells[i] == nullptr)
			{
				continue;
			}
			if ( cells[i]->cell->getGrain() == grainID)
			{
				cells[i]->neighbours += 128;
			}
		}
	};
	virtual void removePickedGrain(int grainID)
	{
		///... todo
	};
	virtual void earsePickedGrains()
	{
		pickedGrains.clear();
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
				float returnVale[3] = { 0,0,0 };
				for (auto cell : visibles)
				{
					//TODO
				}
				return returnVale;
			}
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

	bool pickedGrain(int grainID) 
	{
		for (auto& pickedGrainID : pickedGrains)
		{
			if (pickedGrainID == grainID)
			{
				return true;
			}
		}
		return false;
	}

	// Public static - Cube default impl.
public:
	//cell setter
	static shared_ptr<CellView> CubeCellSetter(shared_ptr<Cell> cell, GridBase* base)
	{
		int x = cell.get()->getMeshCoords()[0];
		int y = cell.get()->getMeshCoords()[1];
		int z = cell.get()->getMeshCoords()[2];
		shared_ptr<CellView> target = make_shared<CellView>();
		target->cell = cell;
		target->neighbours = 126u;

		shared_ptr<CellView> tmp;

		// X
		//LEFT
		tmp = base->getCellView(x - 1, y, z);
		if (tmp != nullptr)
		{
			target->neighbours -= 64;
			tmp->neighbours -= 32;
		}
		//RIGHT
		tmp = base->getCellView(x + 1, y, z);
		if (tmp != nullptr)
		{
			target->neighbours -= 32;
			tmp->neighbours -= 64;
		}

		// Y
		//BOTTOM
		tmp = base->getCellView(x, y - 1, z);
		if (tmp != nullptr)
		{
			target->neighbours -= 16;
			tmp->neighbours -= 8;
		}
		//TOP
		tmp = base->getCellView(x, y + 1, z);
		if (tmp != nullptr)
		{
			target->neighbours -= 8;
			tmp->neighbours -= 16;
		}

		// Z
		//FRONT
		tmp = base->getCellView(x, y, z - 1);
		if (tmp != nullptr)
		{
			target->neighbours -= 4;
			tmp->neighbours -= 2;
		}
		//BACK
		tmp = base->getCellView(x, y, z + 1);
		if (tmp != nullptr)
		{
			target->neighbours -= 2;
			tmp->neighbours -= 4;
		}
		return target;
	}
	//cell maker
	static shared_ptr<CubeCell> makeCubeCell(unsigned short* size, std::shared_ptr<Cell> c, Graphics& gfx)
	{
		return std::make_shared<CubeCell>(size, c, gfx);
	}
};