#include "App.h"
#include <sstream>
#include "CubeCell.h"
#include "DataMiner.h"
#include <Wininet.h>
#include <ShlObj.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <memory>
App::App()
	:
	wnd(800, 600, L"App window")
{}
float pX = 0;
float pY = 0;
int cX = 0;
int cY = 0;
float size = 7;
int CubeCell::ids = 0;
int App::Go()
{
	std::string file = "sample.txt";
	file = "100.txt";
	{
		DataMiner miner(file);
		int n = 100;
		for (int i = 0; i < n; i++) {
			celled.push_back(miner.GetNextCell());
			for (int j = 0; j < n; j++) {
				miner.GetNextCell();
			}
		}
		miner.closeFile();
	}



	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (wnd.kbd.KeyIsPressed(VK_MENU)) {
			MessageBoxA(nullptr, "who did just pressed menu!? >:(", "kurdebele", MB_OK);
			//wnd.kbd.Flush;
		}

		while (!wnd.mouse.IsEmpty()) {
			const auto e = wnd.mouse.Read();
			//wnd.SetWindowTitle("try");
			switch (e.GetType()) {
			case Mouse::Event::Type::Leave:
			{
				std::ostringstream oss;
				oss << "Gone: (" << e.GetPosX() << "," << e.GetPosY() << ")";
				wnd.SetWindowTitle(oss.str());
				break;
			}
			case Mouse::Event::Type::LPress:
			{
				std::ostringstream oss;
				//unsigned short* coords = cells[0].getCoords();
				oss << "LBB: ";
				wnd.SetWindowTitle(oss.str());
				cX = e.GetPosX();
				cY = e.GetPosY();
				break;
			}
			case Mouse::Event::Type::RPress:
			{
				std::ostringstream oss;
				//unsigned short* coords = cells[0].getCoords();
				oss << "LFT: ";
				wnd.SetWindowTitle(oss.str());
				cX = e.GetPosX();
				cY = e.GetPosY();
				break;
			}
			case Mouse::Event::Type::LRelease:
			{
				Cell::Vertex tmp;
				std::ostringstream oss;
				oss << "coords: ";
				tmp = celled.at(0).getVertices()[4];
				oss <<tmp.pos.x;
				oss << " | " ;
				oss << tmp.pos.y;
				oss << " | ";
				oss << tmp.pos.z;
				
				tmp = celled.at(0).getVertices()[7];
				oss << "   ||   ";
				oss << tmp.pos.x;
				oss << " | ";
				oss << tmp.pos.y;
				oss << " | ";
				oss << tmp.pos.z;
				oss << " | ";
				wnd.SetWindowTitle(oss.str());
				break;
			}
			case Mouse::Event::Type::Move: 
			{
				std::ostringstream oss;
				oss << "P: ";
				oss << pX;
				oss << " | ";
				oss << pY;
				wnd.SetWindowTitle(oss.str());
				if (e.LeftIsPressed()) {
					float prosta1A = (float)(0 - cY) / (float)(0 - cX);
					float prosta2A = (float)(0 - e.GetPosY()) / (float)(0 - e.GetPosX());
					cX = e.GetPosX();
					cY = e.GetPosY();
					pX += std::abs((prosta1A - prosta2A) / (prosta1A * prosta2A + 1));
				}
				if (e.RightIsPressed()) {
					float prosta1A = (float)(0 - cY) / (float)(0 - cX);
					float prosta2A = (float)(0 - e.GetPosY()) / (float)(0 - e.GetPosX());
					cX = e.GetPosX();
					cY = e.GetPosY();
					pY += std::abs((prosta1A - prosta2A) / (prosta1A * prosta2A + 1));
				}
				break;
			}
			case Mouse::Event::Type::WheelDown:
			{
				size--;
				std::ostringstream oss;
				oss << "WheelDown :";
				oss << size;
				wnd.SetWindowTitle(oss.str());
				break;
			}
			case Mouse::Event::Type::WheelUp:
			{
				size++;
				std::ostringstream oss;
				oss << "WheelUp :";
				oss << size;
				wnd.SetWindowTitle(oss.str());
				break;
			}
			}
		}
		DoFrame();
	}
	return (int)msg.wParam;
}



void App::DoFrame()
{
	wnd.Gfx().ClearBuffer(0.1f,0.9f,0.5f);
	for (int i = 0; i < celled.size(); i++) {
		wnd.Gfx().DrawTestTriangle(&celled.at(i), pX, pY, 0, size);
	}
	wnd.Gfx().EndFrame();
}
