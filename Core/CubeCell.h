#pragma once
#include "Cell.h"
#include "Graphics.h"
#include "DrawableBase.h"
#include <vector>
#include <string>

/*
	Vertex verticies - kom�rka sze�cienna przechowuje po�o�enie swoich 8 wierzcho�k�w - obliczone na podstawie rozmiaru ca�kowitego siatki
	
	color - kazda sze�cienna kom�rka pami�ta sw�j kolor do narysowania (w postaci 3x float: 0.0f - 1.0f) - aktualnie
	// mozna zmieni� na 3 unsigned shorty (mniejszy nak�ad danych?) 
	// albo przechowywa� id ziarna do kt�rego kom�rka nale�y, a w "ziarnie" niech znajduj� si� 3x float i pewnie jego w�asciwo�ci
	// albo kolory przechowywa� w DataMiner

	position - 

	valuesNames - w�a�ciwo�ci kom�rki specyfikowane w danych wej�ciowych 
	//mo�na przechowywa� jedn� talic� nazw, a nie w ka�dej kom�rce (static? - zak�ada�by, �e ka�da kom�rka b�dzie mia�a takie same rodzaje w�a�ciwo�ci)
	//albo przechowywa� w DataMiner

	values - w�a�ciwo�ci kom�rki specyfikowane w danych wej�ciowych

	konstruktor - rozmiar siatki potrzebny do obliczenia bezwzgl�dnych pozycji wierzcho�k�w kom�rki
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
	float angleX, angleY, angleZ, x, y, z;
	float size;
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