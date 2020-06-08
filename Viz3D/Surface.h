#pragma once
#include "DrawableBase.h"


class Surface : public DrawableBase<Surface>
{
public:
	struct Point
	{
		float x;
		float y;
		float z;
	};
	float A, B, C, D;

	//--------------------=Fields=----------------
private:
	std::vector<Point> points;
	float size = 1;
	//--------------------=Methods=----------------
public:
	Surface(
		std::vector<Point> points,
		float red, float blue, float green,
		float alpha,
		Graphics& gfx);
	Surface(Surface& surface) = default;

	void AddPoint(Point p);
	void RemovePoint(Point p);
	void SetSize(float size);
	float onSurface(Point p);
	//Inharited via Drawable
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};