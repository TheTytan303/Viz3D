#pragma once
#include "Window.h"
#include "CubeCell.h"
#include "CubeFrame.h"
#include "Line.h"
#include "ImGuiManager.h"
#include "DataMiner.h"
#include "Camera.h"
#include <memory>
#include <vector>
class App {

	std::vector<std::unique_ptr<CubeCell>> cells;
	std::vector<std::unique_ptr<CubeFrame>> frames;
	std::vector<std::unique_ptr<Line>> lines;
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
	std::shared_ptr<CubeCell> getPickedItem(int mouseX, int mouseY, int screenWidth, int screenHeight);
	//void sample(float mouseX, float mouseY, int screenWidth, int screenHeight);
private:
	Camera camera;
	std::unique_ptr<DataMiner> pMiner;
	Window wnd;
	ImguiManager imgui;
	char filename[1024] = "state_10x10x10.txt";
};