#pragma once
#include <sstream>
#include <cstdlib>
#include <ctime>
#include "ColorPicker.h"

ColorPicker::ColorPicker(const char* fileName)
{
	this->fileName = fileName;
	this->file.open(fileName);
	if (file.fail())
	{
		throw "failed to open file";
	}
	return;
};
ColorPicker::ColorPicker(const std::string fileName) 
	:
	ColorPicker(fileName.c_str())
{

};
ColorPicker::ColorPicker(const std::wstring fileName)
{
	//this->fileName = fileName;
	this->file.open(fileName);
	if (file.fail())
	{
		throw "failed to open file";
	}
	return;
};

ColorPicker::~ColorPicker() noexcept
{

}

bool ColorPicker::loadNextColor()
{
	unsigned short r, g, b, grainID;
	std::string cline;

	getline(this->file, cline);
	std::istringstream iss(cline);
	iss >> r >> g >> b >> grainID;
	if (file.eof()) {
		return false;
	}
	std::vector<float> colors;
	colors.push_back(static_cast <float> (r) / static_cast <float> (255));
	colors.push_back(static_cast <float> (g) / static_cast <float> (255));
	colors.push_back(static_cast <float> (b) / static_cast <float> (255));
	DrawableCell<CubeCell>::setColor(grainID, colors);
	DrawableCell<Hexal>::setColor(grainID, colors);//todo
	return true;
}

void ColorPicker::loadColors() noexcept
{
	while(loadNextColor())
	{}
}