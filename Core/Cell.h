#pragma once

class Cell
{
public:
	struct Vertex {
		struct {
			float x;
			float y;
			float z;
		} pos;
	};
public:
	//
	virtual unsigned short* getCoords() = 0;
	virtual int getId() = 0;
	virtual double* getDetails() = 0;

};