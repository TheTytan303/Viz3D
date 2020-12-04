#pragma once
#include "ConstantBuffer.h"
#include "DrawableCell.h"
#include "CubeFrame.h"
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

class CubeCell :  public DrawableCell<CubeCell>
{
public:
	static bool frameDrawing;
protected:
	std::shared_ptr<CubeFrame> frame;

	//--------------------=Methods=----------------
public:
	CubeCell(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx);

	CubeCell(CubeCell& cell) = default;

	std::shared_ptr<std::vector<float>> getColor();

	//Inharited via DrawableCell
	float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept override;


	//Inharited via DrawableBase
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Draw(Graphics& gfx) const noexcept override;

};
