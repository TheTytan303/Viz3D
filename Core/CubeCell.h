#pragma once
#include "Cell.h"
#include <vector>
class CubeCell : public Cell
{
private:
	static int ids;
public:
	unsigned short coords[3];
	int id;
	Vertex vertices[8];
	std::vector<float> color;
	std::vector<float> values;
public:
	CubeCell(unsigned short* meshSize, unsigned short x, unsigned short y, unsigned short z, std::vector<float> color, std::vector<float> values);
	//CubeCell(const CubeCell&) = delete;
	CubeCell& operator=(const CubeCell&) = delete;
	Vertex* getVertices();
	std::vector<float> getColor();

	// Inherited via Cell
	virtual unsigned short* getCoords() override;

	virtual int getId() override;

	virtual double* getDetails() override;

};