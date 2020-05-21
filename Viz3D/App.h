#pragma once
#include "Window.h"
#include "CubeCell.h"
#include "CubeFrame.h"
#include "Line.h"
#include "Star.h"
#include "ComboCube.h"
#include "Grid.h"
#include "ImGuiManager.h"
#include "DataMiner.h"
#include "Camera.h"
#include <memory>
#include <vector>
class App {

	std::vector<std::shared_ptr<CubeCell>> cells;
	std::unique_ptr<Grid> grid;
	std::vector<std::unique_ptr<CubeFrame>> frames;
	std::vector<std::unique_ptr<Line>> lines;
	std::vector<std::unique_ptr<Star>> stars;
	std::vector<std::shared_ptr<ComboCube>> comboCubes;
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
	void ShowPickedFrame();
	void makeComboCubes();
	void openFile();
	std::shared_ptr<CubeCell> getPickedItem(int mouseX, int mouseY, int screenWidth, int screenHeight);
	//std::shared_ptr<CubeCell> getPickedItem2(int mouseX, int mouseY, int screenWidth, int screenHeight);
	//void sample(float mouseX, float mouseY, int screenWidth, int screenHeight);
private:
	Camera camera;
	std::shared_ptr<DataMiner> pMiner;
	Window wnd;
	ImguiManager imgui;
	//char filename[1024] = "state_30x30x30.txt";
	wstring filepath = L"100.txt";
	std::shared_ptr<CubeCell> picked;
};