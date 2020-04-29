#pragma once
#include "Window.h"
#include "CubeCell.h"
#include "Box.h"
#include "memory"
#include <vector>
class App {

	std::vector<std::unique_ptr<CubeCell>> cells;
	std::vector<std::unique_ptr<Box>> boxes;
public:
	App();
	int Go();

private:
	void DoFrame();
private:
	Window wnd;
};