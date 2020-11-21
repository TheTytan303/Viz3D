#pragma once
#include <vector>
#include <string>
#include <memory>

#ifdef _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
//#define new DEBUG_NEW
#else
#define DBG_NEW new
#endif
/*
	Reprezentacja kom�rki :
	getMeshCoords - zwraca wsp�rz�dne tej kom�rki w ca�ej strukturze, zwr�ci tablic� 3x unsigned short
	getDetails - zwraca wektor float'�w, kt�re reprezentuj� parametry kom�rki
	getGrain - zwraca ID ziarna

*/

struct ColorBuffer
{
	struct
	{
		float r;
		float g;
		float b;
		float a;
	}face_colors[1];
};
class Cell 
{
	

//--------------------=Static=----------------
protected:
	static std::vector<std::string> valuesNames;

public:
	static std::vector<std::string> getNames() noexcept;
	static void addName(std::string name) noexcept;
	static void setNames(std::vector<std::string> names) noexcept;

protected:
	unsigned short* meshCoords; //parametrize
	int grainID;
	std::vector<float> values;

public:
	unsigned short* getMeshCoords() const;
	int getGrain() const;
	std::vector<float> getDetails() const;


	Cell(unsigned short x, unsigned short y, unsigned short z, 
		int grainID ,std::vector<float> values);
	Cell(Cell& cell);
	Cell(std::shared_ptr<Cell> cell);

	~Cell();
};