#include "App.h"
#include <sstream>
#include "CubeCell.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"


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
	wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 200.0f));
}

int App::Go()
{
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (wnd.kbd.KeyIsPressed(VK_F1)) {
			std::ostringstream oss;
			oss << "F1";
			wnd.SetWindowTitle(oss.str());
			if (wnd.Gfx().IsImGuiEnabled()) 
			{
				show_gui_window = false;
				wnd.Gfx().DisableImGui();
			}
			else
			{
				show_gui_window = true;
				wnd.Gfx().EnableImgui();
			}
		}
		if (wnd.kbd.KeyIsPressed(VK_MENU)) {
		}
		while (!wnd.mouse.IsEmpty()) {
			const auto e = wnd.mouse.Read();
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
				//std::ostringstream oss;
				//oss << "LBB: ";
				//wnd.SetWindowTitle(oss.str());
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
			case Mouse::Event::Type::WheelPress:
			{
				std::ostringstream oss;
				oss << "wheel P: ";
				wnd.SetWindowTitle(oss.str());
				cX = e.GetPosX();
				cY = e.GetPosY();
				break;
			}
			case Mouse::Event::Type::WheelRelease:
			{
				std::ostringstream oss;
				oss << "wheel R: ";
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
				if (e.LeftIsPressed()) {
				}
				if (e.RightIsPressed()) {
				}
				if (e.WheelIsPressed()) {
					std::ostringstream oss;
					oss << "C: ";
					oss << cX;
					oss << " | ";
					oss << cY;
					pY += (float)(cY - e.GetPosY());
					pY /= 100;
					cY = e.GetPosY();
					pX += (float)(cX - e.GetPosX());
					pX /= 100;
					cX = e.GetPosX();
					oss << "     P: ";
					oss << pX;
					oss << " | ";
					oss << pY;
					wnd.SetWindowTitle(oss.str());
					camera.UpdateCamera(0,pX,pY,0,0,0);
				}
				break;
			}
			case Mouse::Event::Type::WheelDown:
			{
				if (!e.WheelIsPressed()) {
					camera.UpdateR(-1);
				}
				break;
			}
			case Mouse::Event::Type::WheelUp:
			{
				if (!e.WheelIsPressed())
				{
					camera.UpdateR(+1);
				}
				break;
			}
			}
		}
		DoFrame();
	}
	return (int)msg.wParam;
}

void App::mineData() 
{
	pMiner = std::make_unique<DataMiner>(filename);
	unsigned short* size = pMiner->meshSize;
	
	int n = (int)size[0] * (int)size[1]* (int)size[2];
	wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, (float(size[2])*2) + 100.0f));
	cells.clear();
	try 
	{
		if (n != 0) 
		{
			for (int i = 0; i < n; i++) {
				cells.push_back(move(pMiner->GetNextCell(wnd.Gfx())));
			}
		}
		else
		{
			for (;;) {
				cells.push_back(move(pMiner->GetNextCell(wnd.Gfx())));
			}
		}
	}
	catch (...) {};
	
}

void App::DoFrame()
{
	//wnd.Gfx().ClearBuffer(0.1f, 0.9f, 0.9f);
	wnd.Gfx().BeginFrame(0.1f, 0.9f, 0.9f);
	wnd.Gfx().SetCamera(camera.getMatrix());
	for (auto& c : cells) {
		//c->Update( pX, pY, 0, 0, 0, size);
		c->Draw(wnd.Gfx());
	}
	//*
	if (show_gui_window)
	{
		if (ImGui::Begin("frames: "))
		{
			ImGui::Text(" %.1f FPS", ImGui::GetIO().Framerate);
		}
		ImGui::End();
		if (ImGui::Begin("Input file name"))
		{
			ImGui::Text("file name:");
			ImGui::InputText("", filename, sizeof(filename));
			if (ImGui::Button("OK"))
			{
				mineData();
			}
		}
		ImGui::End();
		//ImGui::ShowDemoWindow(&show_gui_window);
	}
	//*/
	//ImGui::Render();
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	wnd.Gfx().EndFrame();
}
