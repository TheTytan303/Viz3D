#include "App.h"
#include <sstream>
#include "CubeCell.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <DirectXMath.h>


#include <Wininet.h>
#include <ShlObj.h>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <string>
#include <memory>
App::App()
	:
	wnd(800, 600, L"App window")
{
	//wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 200.0f));

}
int licznik = 0;
int App::Go()
{

	//camera.SetCamera(30, 0, 0, 0, 0, 0);
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (licznik != 0)
		{
			licznik++;
			if (licznik == 60)
			{
				licznik = 0;
			}
		}
		if (wnd.kbd.KeyIsPressed(VK_F1)) {
			if(licznik == 0)
			if (show_gui_window)
			{
				show_gui_window = false;
				//wnd.Gfx().DisableImGui();
			}
			else
			{
				show_gui_window = true;
				//wnd.Gfx().EnableImgui();
			}
			licznik++;
		}
		if (wnd.kbd.KeyIsPressed(VK_MENU)) {
		}
		if (wnd.kbd.KeyIsPressed(VK_F2)) {
			//cells.clear();
			licznik++;
			makeComboCubes();
		}
		while (!wnd.mouse.IsEmpty()) {
			const auto e = wnd.mouse.Read();
			switch (e.GetType()) {
			case Mouse::Event::Type::Leave:
			{
				std::ostringstream oss;
				oss << "Gone: (" << e.GetPosX() << "," << e.GetPosY() << ")";
				wnd.SetWindowTitle(oss.str());
				frames.clear();
				break;
			}
			case Mouse::Event::Type::LPress:
			{
				frames.clear();
				picked = getPickedItem(e.GetPosX(), e.GetPosY(), 800, 600);
				if( picked != nullptr)
				{
					ShowPickedFrame();
				}
				break;
			}
			case Mouse::Event::Type::RPress:
			{
				frames.clear();
				lines.clear();
				break;
			}
			case Mouse::Event::Type::WheelPress:
			{
				cX = e.GetPosX();
				cY = e.GetPosY();
				break;
			}
			case Mouse::Event::Type::WheelRelease:
			{
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
					//oss << "C: ";
					//oss << cX;
					//oss << " | ";
					//oss << cY;
					pY += (float)(cY - e.GetPosY());
					pY /= 100;
					cY = e.GetPosY();
					pX += (float)(cX - e.GetPosX());
					pX /= 100;
					cX = e.GetPosX();
					camera.UpdateCamera(0,pX,pY,0,0,0);
					//camera.UpdateCamera(0,0,0,pX,pY,0);
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
	try 
	{
		pMiner = std::make_unique<DataMiner>(filepath);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "file not found", "Error", MB_OK);
		return;
	}
	wnd.SetWindowTitleW(filepath);
	unsigned short* size = pMiner->meshSize;
	int n = (int)size[0] * (int)size[1] * (int)size[2];
	if (size[2] != 0 && size[2] >= 100)
	{
		camera.SetCamera(size[2] * 2.0f, 0, 0, 0, 0, 0);
		wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 1.0f, size[2] * 3.0f));
	}
	else
	{
		camera.SetCamera(size[2] * 2.0f, 0, 0, 0, 0, 0);
		wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 1.0f, 200.0f+ size[2] * 2.0f));
	}
	//grid.release();
	//grid.~unique_ptr();
	//std::Deleter(grid);
	grid = std::make_unique<Grid>(pMiner);
	grid->makeVisableCells(wnd.Gfx());
}


void App::makeComboCubes()
{
	/*
	if (comboCubes.size() > 0)
	{
		//comboCubes = vector<std::shared_ptr<ComboCube>>();
		comboCubes.clear();
		return;
	}
	//unsigned short* size = pMiner->meshSize;
	unsigned short* size = grid->getSize();
	//int n = (int)size[0] * (int)size[1] * (int)size[2];
	for (unsigned short i = 0; i < size[0]; i+=2)
	{
		for (unsigned short j = 0; j< size[1]; j+=2)
		{
			for (unsigned short k = 0; k<size[2]; k+=2)
			{
				try
				{
					std::vector<std::shared_ptr<CubeCell>> subCubes;
					subCubes.push_back(grid->getCell(i,		j,		k));			//LBD
					subCubes.push_back(grid->getCell(i + 1, j,		k));		//RBD
					subCubes.push_back(grid->getCell(i, j + 1, k));
					subCubes.push_back(grid->getCell(i + 1, j + 1, k));
					subCubes.push_back(grid->getCell(i, j, k + 1));
					subCubes.push_back(grid->getCell(i + 1, j,		k + 1));
					subCubes.push_back(grid->getCell(i, j + 1, k + 1));
					subCubes.push_back(grid->getCell(i + 1, j + 1,	k + 1));

					for (int i = 0; i < 8; i++)
					{
						if (subCubes[i]!=nullptr)
						{
							std::shared_ptr<ComboCube> pComboCube;
							pComboCube = std::make_shared<ComboCube>(size, subCubes, wnd.Gfx());
							comboCubes.push_back(std::move(pComboCube));
						}
					}
				}
				catch (...)
				{

				}
			}
		}
	}
	//*/
}


void App::DoFrame()
{
	wnd.Gfx().BeginFrame(0.1f, 0.9f, 0.9f);
	wnd.Gfx().SetCamera(camera.getMatrix());
	for (auto& f : frames) {
		f->Draw(wnd.Gfx());
	}
	for (auto& l : lines) {
		l->Draw(wnd.Gfx());
	}
	if (grid != nullptr)
	{
		grid->Draw(wnd.Gfx());
	}
	//*
	if (show_gui_window)
	{
		if (picked != nullptr)
		{
			if (ImGui::Begin("Picked Cell: "))
			{
				ImGui::Text("Grain ID: %d", picked->getGrain());
				ImGui::Text("coords: %d x %d x %d",
					picked->getMeshCoords()[0],
					picked->getMeshCoords()[1],
					picked->getMeshCoords()[2]);
				for(int i=0; i<CubeCell::getNames().size();i++)
				{
					std::ostringstream oss;
					oss << CubeCell::getNames().at(i);
					oss << ": ";
					oss << picked->getDetails().at(i);
					ImGui::Text(oss.str().c_str());
				}
			}
			ImGui::End();
		}
		if (ImGui::Begin("frames: "))
		{
			ImGui::Text(" %.1f FPS", ImGui::GetIO().Framerate);
		}
		ImGui::End();
		if (ImGui::Begin("Camera Position: "))
		{
			std::ostringstream oss;
			oss << "X: ";
			oss << DirectX::XMVectorGetX(camera.getPosition());
			oss << " | Y:";
			oss << DirectX::XMVectorGetY(camera.getPosition());
			oss << " | Z:";
			oss << DirectX::XMVectorGetZ(camera.getPosition());
			ImGui::Text(oss.str().c_str());
		}
		ImGui::End();
		if (ImGui::Begin("Camera Direction: "))
		{
			std::ostringstream oss2;
			oss2 << "X: ";
			oss2 << DirectX::XMVectorGetX(camera.getDirection());
			oss2 << " | Y:";
			oss2 << DirectX::XMVectorGetY(camera.getDirection());
			oss2 << " | Z:";
			oss2 << DirectX::XMVectorGetZ(camera.getDirection());
			ImGui::Text(oss2.str().c_str());
		}
		ImGui::End();
		if (ImGui::Begin("Input file"))
		{
			if (ImGui::Button("pickFile"))
			{
				openFile();
			}
		}
		ImGui::End();
		if (ImGui::Begin("Grid Data:"))
		{
			std::ostringstream oss;
			int count = Cell::getNames().size();
			if (count != 0)
			{
				ImGui::Text("variables range:");
				for (int i = 0; i < count; i++)
				{
					if (ImGui::Button(Cell::getNames().at(i).c_str()))
					{
						grid->showVariable(i, wnd.Gfx());
					}
					oss << Cell::getNames().at(i) << ": ";
					oss << grid->getMinis()[i] << " - ";
					oss << grid->getMaxes()[i] << endl;
				}
				ImGui::Text(oss.str().c_str());
				if (ImGui::Button("Show grains"))
				{
					grid->resetColors(wnd.Gfx());
				}
			}
			else
			{
				ImGui::Text("no variables specified");
			}
		}
		ImGui::End();
	}
	//*/
	//ImGui::Render();
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	wnd.Gfx().EndFrame();
}

std::shared_ptr<CubeCell> App::getPickedItem(int mouseX, int mouseY, int screenWidth, int screenHeight)
{
	float m_screenWidth = (float)screenWidth;
	float m_screenHeight = (float)screenHeight;

	// Normalized device coordinates
	float x = (2.0f * (float)mouseX) / m_screenWidth - 1.0f;
	float y = (-2.0f * (float)mouseY) / m_screenHeight + 1.0f;
	

	//XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//XMVECTOR rayDir = XMVectorSet(x, y, 1.0f, 0.0f);
	
	DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixTranspose(camera.getMatrix());
	DirectX::XMMATRIX projectionMatrix = DirectX::XMMatrixTranspose(wnd.Gfx().getProjection());
	DirectX::XMMATRIX inverseviewproj = DirectX::XMMatrixInverse(nullptr, viewMatrix * projectionMatrix);
	
	
	/*
	DirectX::XMVECTOR origin = DirectX::XMVectorSet(x, y, 0, 0);
	DirectX::XMVECTOR farPoint = DirectX::XMVectorSet(x, y, 1, 0);

	DirectX::XMVECTOR rayorigin = DirectX::XMVector3TransformCoord(origin, inverseviewproj);
	DirectX::XMVECTOR rayend = DirectX::XMVector3TransformCoord(farPoint, inverseviewproj);
	DirectX::XMVECTOR raydirection = DirectX::XMVectorSet(
		DirectX::XMVectorGetX(rayend) - DirectX::XMVectorGetX(rayorigin),
		DirectX::XMVectorGetY(rayend) - DirectX::XMVectorGetY(rayorigin),
		DirectX::XMVectorGetZ(rayend) - DirectX::XMVectorGetZ(rayorigin),
		DirectX::XMVectorGetW(rayend) - DirectX::XMVectorGetW(rayorigin)
	);
	raydirection = DirectX::XMVector3Normalize(raydirection);
	//*/
	DirectX::XMVECTOR rayOrigin = DirectX::XMVectorSet(
		DirectX::XMVectorGetX(camera.getPosition()) ,
		DirectX::XMVectorGetY(camera.getPosition()) ,
		DirectX::XMVectorGetZ(camera.getPosition()) ,
		DirectX::XMVectorGetW(camera.getPosition()) 
	);
	DirectX::XMVECTOR rayDirection = DirectX::XMVectorSet(
		DirectX::XMVectorGetX(camera.getDirection()) ,
		DirectX::XMVectorGetY(camera.getDirection()) ,
		DirectX::XMVectorGetZ(camera.getDirection()) ,
		DirectX::XMVectorGetW(camera.getDirection()) 
	);

	rayDirection = DirectX::XMVector3Normalize(rayDirection);

	std::vector<float> v1 = {
		DirectX::XMVectorGetX(rayOrigin),
		DirectX::XMVectorGetY(rayOrigin),
		DirectX::XMVectorGetZ(rayOrigin),
	};
	std::vector<float> v2 = {
		DirectX::XMVectorGetX(rayDirection),
		DirectX::XMVectorGetY(rayDirection),
		DirectX::XMVectorGetZ(rayDirection), };
	lines.push_back(std::move(std::make_unique<Line>(v1,v2,1,0,0,wnd.Gfx())));
	return grid->ifHit(rayOrigin, rayDirection);
}
void App::ShowPickedFrame()
{
	unsigned short* tmp = pMiner->meshSize;
	
	int x1 = picked.get()->getMeshCoords()[0];
	int y1 = picked.get()->getMeshCoords()[1];
	int z1 = picked.get()->getMeshCoords()[2];
	std::unique_ptr<CubeFrame> ptr = std::make_unique<CubeFrame>
		(tmp, x1, y1, z1,
			0.0f, 0.0f, 0.0f,
			wnd.Gfx());
	frames.push_back(std::move(ptr));
}
void App::openFile()
{
	IFileDialog* dialog = NULL;
	HRESULT hr = CoCreateInstance(
		CLSID_FileOpenDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&dialog));
	if (SUCCEEDED(hr))
	{
		// Create an event handling object, and hook it up to the dialog.
		//IFileDialogEvents* pfde = NULL;
		//hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
		//if (SUCCEEDED(hr))
		//{
		DWORD dwFlags;
		hr = dialog->GetOptions(&dwFlags);
		if (SUCCEEDED(hr))
		{
			// In this case, get shell items only for file system items.
			hr = dialog->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
			if (SUCCEEDED(hr))
			{
				// Set the default extension to be ".txt" / ".csv" file.
				hr = dialog->SetDefaultExtension(L"txt;csv");
				if (SUCCEEDED(hr))
				{
					// Show the dialog
					hr = dialog->Show(NULL);
					if (SUCCEEDED(hr))
					{
						// Obtain the result once the user clicks 
						// the 'Open' button.
						// The result is an IShellItem object.
						IShellItem* psiResult;
						hr = dialog->GetResult(&psiResult);
						if (SUCCEEDED(hr))
						{
							PWSTR pszFilePath = NULL;
							hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
								&pszFilePath);
							if (SUCCEEDED(hr))
							{
								std::wstringstream wss;
								wss << pszFilePath;
								filepath = wss.str();
								mineData();
								//std::ostringstream oss;
								//oss << pszFilePath;
								//filepath = oss.str();
								//wcstombs(filename, pszFilePath, wcslen(pszFilePath));
								//strncpy_s(filename, filepath.c_str(), sizeof(filepath.c_str()) - 1);
							}
							psiResult->Release();
						}
					}
				}
			}
		}
		dialog->Release();
	}
}