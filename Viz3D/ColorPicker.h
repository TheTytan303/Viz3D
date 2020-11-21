#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include "CubeCell.h"

class ColorPicker
{
public:
	ColorPicker(const char* fileName);
	ColorPicker(const std::string fileName);
	ColorPicker(const std::wstring fileName);
	ColorPicker(const ColorPicker&) = delete;

	~ColorPicker() noexcept;
	ColorPicker& operator=(const ColorPicker&) = delete;
	ColorPicker() = delete;
	void closeFile() noexcept;
	bool loadNextColor();
	void loadColors() noexcept;


private:
	const char* fileName;
	std::ifstream file;
};