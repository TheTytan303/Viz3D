#pragma once
#include <vector>
#include <string>
#include <memory>

#ifdef _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif
/*
	Reprezentacja komórki :
	getMeshCoords - zwraca wspó³rzêdne tej komórki w ca³ej strukturze, zwróci tablicê 3x unsigned short
	getDetails - zwraca wektor float'ów, które reprezentuj¹ parametry komórki
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