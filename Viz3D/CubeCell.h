#pragma once
#include "Cell.h"
#include "Graphics.h"
#include "DrawableBase.h"
#include <vector>
#include <string>

/*
	Vertex verticies - komórka szeœcienna przechowuje po³o¿enie swoich 8 wierzcho³ków - obliczone na podstawie rozmiaru ca³kowitego siatki
	
	color - kazda szeœcienna komórka pamiêta swój kolor do narysowania (w postaci 3x float: 0.0f - 1.0f) - aktualnie
	// mozna zmieniæ na 3 unsigned shorty (mniejszy nak³ad danych?) 
	// albo przechowywaæ id ziarna do którego komórka nale¿y, a w "ziarnie" niech znajduj¹ siê 3x float i pewnie jego w³asciwoœci
	// albo kolory przechowywaæ w DataMiner

	position - 

	valuesNames - w³aœciwoœci komórki specyfikowane w danych wejœciowych 
	//mo¿na przechowywaæ jedn¹ talicê nazw, a nie w ka¿dej komórce (static? - zak³ada³by, ¿e ka¿da komórka bêdzie mia³a takie same rodzaje w³aœciwoœci)
	//albo przechowywaæ w DataMiner

	values - w³aœciwoœci komórki specyfikowane w danych wejœciowych

	konstruktor - rozmiar siatki potrzebny do obliczenia bezwzglêdnych pozycji wierzcho³ków komórki
*/


class CubeCell : public Cell, public DrawableBase<CubeCell>
{
public:
	int coords[3];
	int id;
	std::vector<float> color;
	//std::vector<std::string> valuesNames;
	std::vector<float> values;
private:
	static float size;
public:
	CubeCell(int id, unsigned short* meshSize, unsigned short x, unsigned short y, unsigned short z, std::vector<float> color, std::vector<float> values, Graphics& gfx);
	CubeCell(CubeCell& cell) = default;
	std::vector<float> getColor();

	// Inherited via Cell
	int* getCoords() override;
	int getId() override;
	std::vector<float> getDetails() override;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Update(float angleX, float angleY, float angleZ, float x, float y, float z) noexcept override;
};
