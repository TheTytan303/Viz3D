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
//--------------------=Static=----------------
private:
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
	bool ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept;


	// Inherited via Cell
	short* getCoords() const override;
	int getId() const override;
	int getGrain() const override;
	std::vector<float> getDetails() const override;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Update(float angleX, float angleY, float angleZ, float x, float y, float z) noexcept override;
};
