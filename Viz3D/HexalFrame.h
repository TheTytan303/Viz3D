#pragma once
#include "ConstantBuffer.h"
#include "Cell.h"
#include "Graphics.h"
#include "DrawableBase.h"


class HexalFrame : public DrawableBase<HexalFrame>
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

public:
	HexalFrame(
		unsigned short* meshSize,
		unsigned short x, unsigned short y, unsigned short z,
		float red, float blue, float green,
		Graphics& gfx);




	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};