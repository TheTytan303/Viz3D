#pragma once
#include "CubeCell.h"
#include "Graphics.h"
#include "DataMiner.h"

#include <memory>
#include <DirectXMath.h>

using namespace std;
struct CellView
{
	shared_ptr<Cell> cell;
	short neighbours;
};


class Grid
{

private:
	shared_ptr<CellView>* cells;	//invisibles
	vector<shared_ptr<CubeCell>> visibles;	//<----
	float* minis;
	float* maxes;
	unsigned short size[3];	// x,y,z
public:
	//---------------Constructors---------------

	Grid(unsigned short* size, vector<shared_ptr<CubeCell>> cells);
	Grid(std::shared_ptr<DataMiner> pDataMiner);

	~Grid();


	//---------------Methods---------------
	unsigned short* getSize()
	{
		return size;
	}
	shared_ptr<CellView> getCellView(unsigned short x, unsigned short y, unsigned short z);
	shared_ptr<Cell> getCell(unsigned short x, unsigned short y, unsigned short z);
	shared_ptr<CubeCell> ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction);
	void setCell(unsigned short x, unsigned short y, unsigned short z, shared_ptr<CubeCell> cell);
	void setCell(shared_ptr<Cell> cell);
	void Draw(Graphics& Gfx);
	vector<shared_ptr<CubeCell>> getVisableCells();
	vector<shared_ptr<CubeCell>> makeVisableCells(Graphics& gfx);

	float* getMinis();
	float* getMaxes();

	void showVariable(int valueNo, Graphics& gfx);
	void resetColors(Graphics& gfx);
private:
	vector<float> getColor(float max, float min, float val);
};