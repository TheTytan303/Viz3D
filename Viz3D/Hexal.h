#pragma once
#include "HexalFrame.h"
#include "DrawableCell.h"
#include <memory>


class Hexal : public DrawableCell<Hexal>
{
public:
	//--------------------=Static=----------------
	static bool frameDrawing;
private:
	//--------------------=Fields=----------------
	std::shared_ptr<HexalFrame> frame;
//--------------------=Methods=----------------
public:
	Hexal(unsigned short* meshSize, std::shared_ptr<Cell> cell, Graphics& gfx);

	//Inharited via DrawableCell
	float ifHit(DirectX::XMVECTOR origin, DirectX::XMVECTOR direction, float dist) const noexcept override;

	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	void Draw(Graphics& gfx) const noexcept override;
};