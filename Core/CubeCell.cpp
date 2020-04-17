#include "CubeCell.h"
#include "Cell.h"
std::vector<float> CubeCell::getColor()
{
	return color;
}

CubeCell::CubeCell(unsigned short* meshSize, unsigned short x, unsigned short y, unsigned short z, std::vector<float> color, std::vector<float> values)
	:
	values(values),
	color(color)
	//id(CubeCell::ids++)
{
	this->coords[0] = x;
	this->coords[1] = y;
	this->coords[2] = z;
	this->id = CubeCell::ids++;
	//float tmpX = 2* (float)x / ((float)meshSize[0]) -1;
	//float tmpY = 2* (float)y / ((float)meshSize[1]) -1;
	//float tmpZ = 2* (float)z / ((float)meshSize[2]) -1;

	float s = 2.0f/(float)meshSize[0];
	s *= 0.9f;
	//float s = 0.6f;
	float tmpX = (float)(x*2)/ (float)meshSize[0] -1.0f +(s / 2);
	float tmpY = (float)(y * 2) / (float)meshSize[1] - 1.0f + (s / 2);
	float tmpZ = (float)(z * 2) / (float)meshSize[2] - 1.0f + (s / 2);


	this->vertices[0] = {tmpX - s, tmpY - s, tmpZ - s	, color[0], color[1], color[2]},//LBB - 0
	this->vertices[1] =	{tmpX,   tmpY-s, tmpZ-s			, color[0], color[1], color[2]};	//RBB - 1
	this->vertices[2] = {tmpX-s, tmpY,   tmpZ-s			, color[0], color[1], color[2]};	//LBT - 2
	this->vertices[3] = {tmpX,   tmpY,   tmpZ-s			, color[0], color[1], color[2]};	//RBT - 3
	this->vertices[4] = {tmpX-s, tmpY-s, tmpZ			, color[0], color[1], color[2]};	//LFB - 4
	this->vertices[5] = {tmpX,   tmpY-s, tmpZ			, color[0], color[1], color[2]};	//RFB - 5
	this->vertices[6] = {tmpX-s, tmpY,   tmpZ			, color[0], color[1], color[2]};	//LFT - 6
	this->vertices[7] = {tmpX,   tmpY,   tmpZ			, color[0], color[1], color[2]};	//RFT - 7
}

Cell::Vertex* CubeCell::getVertices()
{
	return vertices;
}

unsigned short* CubeCell::getCoords()
{
	return coords;
}

int CubeCell::getId()
{
	return id;
}

std::vector<float> CubeCell::getDetails()
{
	return values;
}
