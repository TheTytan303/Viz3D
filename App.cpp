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
{
	std::string file = "sample.txt";
	file = "100.txt";
	//file = "state_10x10x10.txt";
	//file = "state_20x20x20.txt";
	DataMiner miner(file);
	int n = 10000;
	for (int i = 0; i < n; i++) {
		cells.push_back( move(miner.GetNextCell( wnd.Gfx() )) );
	}
	wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 100.0f));
	
}
float pX = 0;
float pY = 0;
int cX = 0;
int cY = 0;
float size = 130;
int App::Go()
{
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
				try 
				{
					//celled.push_back(miner.GetNextCell());
				}
				catch (...)
				{
					MessageBoxA(nullptr, "out of file", "kurdebele", MB_OK);
				};
				std::ostringstream oss;
				oss << "LBB: ";
				wnd.SetWindowTitle(oss.str());
				cX = e.GetPosX();
				cY = e.GetPosY();
				break;
			}
			case Mouse::Event::Type::RPress:
			{
				std::ostringstream oss;
				oss << "LFT: ";
				wnd.SetWindowTitle(oss.str());
				cX = e.GetPosX();
				cY = e.GetPosY();
				break;
			}
			case Mouse::Event::Type::LRelease:
			{
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
					pX -= (prosta1A - prosta2A) / (prosta1A * prosta2A + 1);
				}
				if (e.RightIsPressed()) {
					float prosta1A = (float)(0 - cY) / (float)(0 - cX);
					float prosta2A = (float)(0 - e.GetPosY()) / (float)(0 - e.GetPosX());
					cX = e.GetPosX();
					cY = e.GetPosY();
					pY += (prosta1A - prosta2A) / (prosta1A * prosta2A + 1);
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
	wnd.Gfx().ClearBuffer(0.1f, 0.9f, 0.9f);
	for (auto& c : cells) {
		c->Update( pX, pY, 0, 0, 0, size);
		c->Draw(wnd.Gfx());
	}
	wnd.Gfx().EndFrame();
}
