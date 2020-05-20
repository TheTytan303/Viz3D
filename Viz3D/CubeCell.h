#pragma once
#include "ConstantBuffer.h"
#include "Cell.h"
#include "Graphics.h"
#include "DrawableBase.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>


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


class CubeCell : public Cell, public DrawableBase<CubeCell>
{
//--------------------=Static=----------------
protected:
	static std::map<int, std::shared_ptr<std::vector<float>>> colors;
public:
	static std::shared_ptr<std::vector<float>> getColorOf(int grain) noexcept;
	static bool addColor(int grain) noexcept;
	static bool setColor(int grain, std::vector<float> color) noexcept;
	static void initGrain(int id);
	static float size;			
//--------------------=Fields=----------------
private:
	short coords[3];
	ConstantBuffer<ColorBuffer>* pColorBuffer;

//--------------------=Methods=----------------
public:
	CubeCell(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx);

	CubeCell(CubeCell& cell) = default;

	std::shared_ptr<std::vector<float>> getColor();
	float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept;
	void setColor(std::vector<float> color, Graphics& gfx);
	void resetColor(Graphics& gfx);
	short* getCoords() const;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

};
