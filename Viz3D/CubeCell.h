#pragma once
#include "Cell.h"
#include "Graphics.h"
#include "DrawableBase.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

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
//--------------------=Static=----------------
protected:
	static std::map<int, std::shared_ptr<std::vector<float>>> colors;
	static std::vector<std::string> valuesNames;
public:
	static std::vector<std::string> getNames() noexcept;
	static void addName(std::string name) noexcept;
	static void setNames(std::vector<std::string> names) noexcept;
	static std::shared_ptr<std::vector<float>> getColorOf(int grain) noexcept;
	static bool addColor(int grain) noexcept;
	static bool setColor(int grain, std::vector<float> color) noexcept;
	static float size;			
//--------------------=Fields=----------------
public:
	short coords[3];			//6B
	int id;						//4B
	int grain;					//4B
	short meshCoords[3];			//6B
	//18b / cell
	std::vector<float> values;

//--------------------=Methods=----------------
public:
	CubeCell(int id, unsigned short* meshSize, unsigned short x, unsigned short y, unsigned short z, int grain, std::vector<float> values, Graphics& gfx);
	CubeCell(CubeCell& cell) = default;
	std::shared_ptr<std::vector<float>> getColor();

	std::vector<DirectX::XMVECTOR> GetTriangles() const noexcept;
	float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept;


	// Inherited via Cell
	short* getCoords() const override;
	int getId() const override;
	int getGrain() const override;
	std::vector<float> getDetails() const override;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};
