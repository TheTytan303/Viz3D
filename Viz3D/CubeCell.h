#pragma once
#include "ConstantBuffer.h"
#include "Graphics.h"
#include "DrawableCell.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

class CubeCell :  public DrawableCell
{

//--------------------=Methods=----------------
public:
	CubeCell(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx);

	CubeCell(CubeCell& cell) = default;

	std::shared_ptr<std::vector<float>> getColor();

	//Inharited via DrawableCell
	float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept override;
	float* getCoords() const override;


	//Inharited via DrawableBase
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

};
