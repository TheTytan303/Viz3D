#pragma once
#include "CubeCell.h"
#include "Graphics.h"
#include "DataMiner.h"
#include "Surface.h"

//tmp
#include "CubeFrame.h";

#include <memory>
#include <DirectXMath.h>

using namespace std;
struct CellView
{
	shared_ptr<Cell> cell;
	short neighbours;
};
struct Slices
{
	shared_ptr<Surface> s;
	bool flag;
};



class Grid
{

private:
	shared_ptr<CellView>* cells;	
	vector<shared_ptr<CubeCell>> visibles;	
	vector<Slices> slices;	
	float* minis;
	float* maxes;
	unsigned short size[3];	// x,y,z
	//unsigned short bounds[6]; // mesh: x,x,y,y,z,z
	//vector<> surfaces;
public:
	//---------------Constructors---------------

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

	void Slice(shared_ptr<Surface> s, bool side);
	void deSlice();

	float* getMinis();
	float* getMaxes();

	void showVariable(int valueNo, Graphics& gfx);
	void resetColors(Graphics& gfx);
private:
	vector<float> getColor(float max, float min, float val);
	bool outOfBounds(shared_ptr<Cell> c) const;
};