#pragma once
#include "Cell.h"
#include <vector>
#include <string>

/*
	Vertex verticies - komórka sześcienna przechowuje położenie swoich 8 wierzchołków - obliczone na podstawie rozmiaru całkowitego siatki
	
	color - kazda sześcienna komórka pamięta swój kolor do narysowania (w postaci 3x float: 0.0f - 1.0f) - aktualnie
	// mozna zmienić na 3 unsigned shorty (mniejszy nakład danych?) 
	// albo przechowywać id ziarna do którego komórka należy, a w "ziarnie" niech znajdują się 3x float i pewnie jego własciwości
	// albo kolory przechowywać w DataMiner

	valuesNames - właściwości komórki specyfikowane w danych wejściowych 
	//można przechowywać jedną talicę nazw, a nie w każdej komórce (static? - zakładałby, że każda komórka będzie miała takie same rodzaje właściwości)
	//albo przechowywać w DataMiner

	values - właściwości komórki specyfikowane w danych wejściowych

	konstruktor - rozmiar siatki potrzebny do obliczenia bezwzględnych pozycji wierzchołków komórki
*/


class CubeCell : public Cell
{
private:
	static int ids;
public:
	unsigned short coords[3];
	int id;
	Vertex vertices[8];
	std::vector<float> color;
	//std::vector<std::string> valuesNames;
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

	virtual std::vector<float> getDetails() override;

};
