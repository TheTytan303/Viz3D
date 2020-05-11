#pragma once
#include "CubeCell.h"

class ComboCube : public DrawableBase<ComboCube>
{
	//--------------------=Static=----------------
	/*
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
	*/
	//--------------------=Fields=----------------

public:
	//short coords[3];				//6B
	//int id;						//4B
	//int grain;					//4B
	//short meshCoords[3];			//6B
	std::vector<std::shared_ptr<CubeCell>> subCells; //32B (=8*4B)
	int cell;						//4B
	//18b / cell
	//std::vector<float> values;

	//--------------------=Methods=----------------
public:
	ComboCube(unsigned short* meshSize, 
		std::vector<std::shared_ptr<CubeCell>> subCells, 
		Graphics& gfx);
	ComboCube(ComboCube& cell) = default;
	//std::shared_ptr<std::vector<float>> getColor();


	//std::vector<DirectX::XMVECTOR> GetTriangles() const noexcept;
	bool ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept;


	// Inherited via Cell
	// short* getCoords() const override;
	// int getId() const override;
	// int getGrain() const override;
	std::vector<float> getDetails() const;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};
