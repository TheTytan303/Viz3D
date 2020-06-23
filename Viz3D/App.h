#pragma once
#include "Window.h"
#include "CubeCell.h"
#include "CubeFrame.h"
#include "Line.h"
#include "Star.h"
#include "Hexal.h"
#include "HexalFrame.h"
#include "Surface.h"
#include "ComboCube.h"
#include "Grid.h"
#include "GridBase.h"
#include "ImGuiManager.h"
#include "DataMiner.h"
#include "Camera.h"
#include <memory>
#include <vector>
class App {

	//std::vector<std::shared_ptr<CubeCell>> cells; 
	//TODO: Multiple Grids 
	//std::unique_ptr<GridBase> grid;
	//std::vector<std::shared_ptr<GridBase>> grids;
	shared_ptr<GridBase> grid;
	//GridBase* grid;
	std::vector<std::unique_ptr<CubeFrame>> frames;
	std::vector<std::unique_ptr<Line>> lines;
	std::vector<std::unique_ptr<Star>> stars;
	std::vector<std::shared_ptr<Surface>> surfaces;
	std::vector<std::shared_ptr<Hexal>> hexals;
	std::vector<std::shared_ptr<HexalFrame>> hexalFrames;
	std::vector<std::shared_ptr<ComboCube>> comboCubes;
	bool show_gui_window = true;
	float pX = 0;
	float pY = 0;
	int cX = 0;
	int cY = 0;
	int cellType=0;
	enum CellType {
		CubeCell,
		Hexal
	};
	//CellType cellType = CubeCell;
	//CellType cellType = Hexal;
public:
	App();
	int Go();

private:
	void DoFrame();
	void mineData();
	void ShowPickedFrame();
	void showFramesOf(vector<std::shared_ptr<Cell>> cells);
	void makeComboCubes();
	void openFile();
	shared_ptr<Surface> buildSurface(std::shared_ptr<Cell> c1, std::shared_ptr<Cell> c2, std::shared_ptr<Cell> c3);

private:
	std::shared_ptr<Cell> getPickedItem(int mouseX, int mouseY, int screenWidth, int screenHeight);
	Camera camera;
	std::shared_ptr<DataMiner> pMiner;
	Window wnd;
	ImguiManager imgui;
	//char filename[1024] = "state_30x30x30.txt";
	wstring filepath = L"100.txt";

	void makeVisableCells();
	void buildGrid(std::shared_ptr<DataMiner> pMiner);
};