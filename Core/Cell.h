#pragma once
#include <vector>
/*
	Reprezentacja komórki :
	struktura "vertex" reprezentuje wierzchołek, potrzebne do rysowania w przestrzeni przez DirectX 
	// oraz przechowuje kolory w tych wierzchołkach
	getCoords - zwraca współrzędne tej komórki w całlej strukturze, zwróci tablicę 3x unsigned short
	getDetails - zwraca tablicę float'ów, które reprezentują parametry komórki
*/
class Cell
{
public:
	struct Vertex {
		struct {
			float x;
			float y;
			float z;
		} pos;
		struct {
			float r;
			float g;
			float b;
		}color;
	};
public:
	//
	virtual unsigned short* getCoords() = 0;
	virtual int getId() = 0;
	virtual  std::vector<float> getDetails() = 0;

};
