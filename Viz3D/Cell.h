#pragma once
#include <vector>
/*
	Reprezentacja kom�rki :
	struktura "vertex" reprezentuje wierzcho�ek, potrzebne do rysowania w przestrzeni przez DirectX 
	// oraz przechowuje kolory w tych wierzcho�kach
	getCoords - zwraca wsp�rz�dne tej kom�rki w ca�lej strukturze, zwr�ci tablic� 3x unsigned short
	getDetails - zwraca tablic� float'�w, kt�re reprezentuj� parametry kom�rki
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
	virtual int* getCoords() = 0;
	virtual int getId() = 0;
	virtual  std::vector<float> getDetails() = 0;

};