#pragma once
#include "HexalFrame.h"
#include "ConstantBuffer.h"
#include "Graphics.h"
#include "DrawableCell.h"
#include <memory>


class Hexal : public DrawableCell<Hexal>
{
//--------------------=Fields=----------------
	std::shared_ptr<HexalFrame> frame;
//--------------------=Methods=----------------
public:
	Hexal(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx);

	//Inharited via DrawableCell
	float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept override;
	float* getCoords() const override;
	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Draw(Graphics& gfx) const noexcept override;
};