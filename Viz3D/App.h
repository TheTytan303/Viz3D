#pragma once
#include "Window.h"
#include "CubeCell.h"
#include "ImGuiManager.h"
#include "DataMiner.h"
#include "Camera.h"
#include <memory>
#include <vector>
class App {

	std::vector<std::unique_ptr<CubeCell>> cells;
	bool show_gui_window = true;
	float pX = 0;
	float pY = 0;
	int cX = 0;
	int cY = 0;
public:
	App();
	int Go();

private:
	void DoFrame();
	void mineData();
	std::shared_ptr<CubeCell> getPickedItem(float mouseX, float mouseY);
private:
	Camera camera;
	std::unique_ptr<DataMiner> pMiner;
	Window wnd;
	ImguiManager imgui;
	char filename[1024] = "state_20x20x20.txt";
};