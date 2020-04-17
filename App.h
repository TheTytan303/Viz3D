#pragma once
#include "Window.h"
#include "CubeCell.h"
#include <vector>
class App {

	std::vector<CubeCell> celled;
public:
	App();
	int Go();

private:
	void DoFrame();
private:
	Window wnd;
};