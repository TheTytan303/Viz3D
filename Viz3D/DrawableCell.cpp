#include "DrawableCell.h"

//inits:

float DrawableCell::size = 0.5f;

std::map<int, std::shared_ptr<std::vector<float>>> DrawableCell::colors = {};


//--------------------=Static=----------------

std::shared_ptr<std::vector<float>> DrawableCell::getColorOf(int grain) noexcept
{
	return DrawableCell::colors.at(grain);
}

bool DrawableCell::addColor(int grain) noexcept
{
	if (colors.count(grain)) {
		std::shared_ptr<std::vector<float>> returnVale = std::make_shared<std::vector<float>>();
		for (int i = 0; i < 3; i++) {
			returnVale->push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		}
		colors[grain] = returnVale;
		return true;
	}
	else
	{
		std::shared_ptr<std::vector<float>> returnVale = std::make_shared<std::vector<float>>();
		for (int i = 0; i < 3; i++) {
			returnVale->push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		}
		colors[grain] = returnVale;
		return false;
	}
}

bool DrawableCell::setColor(int grain, const std::vector<float> color) noexcept
{
	if (colors.count(grain)) {
		colors[grain] = std::make_shared<std::vector<float>>(color);
		return true;
	}
	else
	{
		colors[grain] = std::make_shared<std::vector<float>>(color);
		return false;
	}
}

void DrawableCell::initGrain(int id) {
	if (colors.find(id) == colors.end())
	{
		DrawableCell::addColor(id);
	}
}

DrawableCell::DrawableCell(Cell cell)
	:
	Cell(cell)
{
	DrawableCell::initGrain(cell.getGrain());
};

void DrawableCell::setColor(std::vector<float> color, Graphics& gfx)
{
	const ColorBuffer cb = {
		{
			{
			color.at(0),
			color.at(1),
			color.at(2),
			1.0f
			}
		}
	};
	pColorBuffer->update(gfx, cb);
}
void DrawableCell::resetColor(Graphics& gfx)
{
	const ColorBuffer cb2 = {
		{
			{
			colors.at(grainID)->at(0),
			colors.at(grainID)->at(1),
			colors.at(grainID)->at(2),
			1.0f
			}
		}
	};
	pColorBuffer->update(gfx, cb2);
}