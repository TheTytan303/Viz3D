#pragma once
#include <vector>
/*
	Reprezentacja komórki :
	struktura "vertex" reprezentuje wierzcho³ek, potrzebne do rysowania w przestrzeni przez DirectX 
	// oraz przechowuje kolory w tych wierzcho³kach
	getCoords - zwraca wspó³rzêdne tej komórki w ca³lej strukturze, zwróci tablicê 3x unsigned short
	getDetails - zwraca tablicê float'ów, które reprezentuj¹ parametry komórki
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
	};
public:
	//
	virtual short* getCoords() const = 0;
	virtual int getId() const = 0;
	virtual int getGrain() const = 0;
	virtual std::vector<float> getDetails()const = 0;

};