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

//Specific CellMakers:
shared_ptr<CubeCell> makeCubeCell(unsigned short* size, std::shared_ptr<Cell> c, Graphics& gfx)
{
	return std::make_shared<CubeCell>(size, c, gfx);
}

shared_ptr<Hexal> makeHexal(unsigned short* size, std::shared_ptr<Cell> c, Graphics& gfx)
{
	return std::make_shared<Hexal>(size, c, gfx);
}




App::App()
	:
	wnd(800, 600, L"App window")
{

	wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 200.0f));
	camera.SetCamera(30, 0, 0, 0, 0, 0);

}
int licznik = 0;
int App::Go()
{

	std::wstringstream wss;
	wss << "C:\\Users\\wcies\\source\\repos\\Viz3D\\samples\\state_10x10x10.txt";
	filepath = wss.str();
	mineData();

	//unsigned short meshSize[3] = { 1,1,1 };
	//std::vector<float> values;
	//values.push_back(2.3f);
	//std::shared_ptr<Cell> tmp_cell = std::make_shared<Cell>(1, 1, 1, 2, values);

	//Hexal h(tmp_cell, wnd.Gfx());
	//unique_ptr<Hexal> hexal = make_unique<Hexal>( tmp_cell,wnd.Gfx());
	//hexals.push_back(std::move(hexal));
	//unique_ptr<HexalFrame> hf = make_unique<HexalFrame>(meshSize, 1,1,1, 0,0,0,wnd.Gfx());
	//hexalFrames.push_back(std::move(hf));


	std::vector<float> v1 = {0,0,0};
	stars.push_back(std::make_unique<Star>(v1, 1.0f, 1.0f, 0.0f, wnd.Gfx()));
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
				//std::ostringstream oss;
				//oss << "Gone: (" << e.GetPosX() << "," << e.GetPosY() << ")";
				//wnd.SetWindowTitle(oss.str());
				//frames.clear();
				break;
			}
			case Mouse::Event::Type::LPress:
			{
				frames.clear();
				if (!wnd.kbd.KeyIsPressed(VK_SHIFT))
				{
					pickedCells.clear();
				}
				picked = getPickedItem(e.GetPosX(), e.GetPosY(), 800, 600);
				if( picked != nullptr)
				{
					pickedCells.push_back(picked);
				}
				ShowPickedFrame();
				if (pickedCells.size() > 2)
				{
					int last = (int)pickedCells.size();
					surfaces.clear();
					shared_ptr<Surface> pSurface = buildSurface(
						pickedCells[last - 3], pickedCells[last - 2], pickedCells[last - 1]
						);
					grid->Slice(pSurface, true);
					makeVisableCells();
				}
				break;
			}
			case Mouse::Event::Type::RPress:
			{
				grid->deSlice();
				makeVisableCells();
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
	picked = nullptr;
	pickedCells.clear();
	surfaces.clear();
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
	buildGrid(pMiner);
	makeVisableCells();
}


void App::makeComboCubes()
{

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
	for (auto& s : stars) {
		s->Draw(wnd.Gfx());
	}
	for (auto& s : surfaces) {
		s->Draw(wnd.Gfx());
	}
	for (auto& h : hexals) {
		h->Draw(wnd.Gfx());
	}
	for (auto& hs : hexalFrames) {
		hs->Draw(wnd.Gfx());
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
			int count = (int)Cell::getNames().size();
			if (count != 0)
			{
				ImGui::Text("variables range:");
				for (int i = 0; i < count; i++)
				{
					if (ImGui::Button(Cell::getNames().at(i).c_str()))
					{
						grid->showVariable(i, wnd.Gfx());
						ImGui::SameLine();
					}
					std::ostringstream oss;
					oss << Cell::getNames().at(i) << ": ";
					oss << grid->getMinis()[i] << " - ";
					oss << grid->getMaxes()[i];
					ImGui::Text(oss.str().c_str());
				}
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

		if (ImGui::Begin("Celling Type"))
		{
			ImGui::RadioButton("Cube Cell", &cellType, 0);
			ImGui::RadioButton("Hexal", &cellType, 1);
			if (ImGui::Button("Rebuild"))
			{
				buildGrid(pMiner);
				makeVisableCells();
			}
		}
		ImGui::End();
	}
	//*/
	//ImGui::Render();
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	wnd.Gfx().EndFrame();
}

void App::ShowPickedFrame()
{
	unsigned short* tmp = pMiner->meshSize;
	for (int i = 0; i < pickedCells.size(); i++)
	{
		int x1 = pickedCells[i].get()->getMeshCoords()[0];
		int y1 = pickedCells[i].get()->getMeshCoords()[1];
		int z1 = pickedCells[i].get()->getMeshCoords()[2];
		std::unique_ptr<CubeFrame> ptr = std::make_unique<CubeFrame>
			(tmp, x1, y1, z1,
				0.0f, 0.0f, 0.0f,
				wnd.Gfx());
		frames.push_back(std::move(ptr));
	}
}
void App::showFramesOf(vector<std::shared_ptr<Cell>> cells)
{
	unsigned short* tmp = pMiner->meshSize;
	for (int i = 0; i < cells.size(); i++)
	{
		int x1 = cells[i].get()->getMeshCoords()[0];
		int y1 = cells[i].get()->getMeshCoords()[1];
		int z1 = cells[i].get()->getMeshCoords()[2];
		std::unique_ptr<CubeFrame> ptr = std::make_unique<CubeFrame>
			(tmp, x1, y1, z1,
				0.0f, 0.0f, 0.0f,
				wnd.Gfx());
		frames.push_back(std::move(ptr));
	}

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

shared_ptr<Surface> App::buildSurface(std::shared_ptr<Cell> c1, std::shared_ptr<Cell> c2, std::shared_ptr<Cell> c3)
{ 
	float size = 1.0f;
	vector<Surface::Point> points
	{
		{(float)c1->getMeshCoords()[0] * size,(float)c1->getMeshCoords()[1] * size,(float)c1->getMeshCoords()[2] * size},
		{(float)c2->getMeshCoords()[0] * size,(float)c2->getMeshCoords()[1] * size,(float)c2->getMeshCoords()[2] * size},
		{(float)c3->getMeshCoords()[0] * size,(float)c3->getMeshCoords()[1] * size,(float)c3->getMeshCoords()[2]*size},
	};
	return make_shared<Surface>(points,
		0.0f,1.0f,0.0f,0.5f,
		wnd.Gfx());
}

std::shared_ptr<Cell> App::getPickedItem(int mouseX, int mouseY, int screenWidth, int screenHeight)
{
	using namespace DirectX;
	float m_screenWidth = (float)screenWidth;
	float m_screenHeight = (float)screenHeight;

	float x =(float)mouseX;  
	float y =(float)mouseY;
	std::ostringstream oss;
	oss << "X: ";
	oss << x;
	oss << "Y: ";
	oss << y;
	
	//wnd.SetWindowTitle(oss.str());

	DirectX::XMMATRIX viewMatrix = camera.getMatrix();
	DirectX::XMMATRIX projectionMatrix = wnd.Gfx().getProjection();
	
	DirectX::XMVECTOR rayOrigin = DirectX::XMVector3Unproject(
		XMVectorSet(x, y, 0, 0),
		0,
		0,
		m_screenWidth,
		m_screenHeight,
		0,
		1,
		projectionMatrix,
		viewMatrix,
		XMMatrixSet(
			1,0,0,0,
			0,1,0,0,
			0,0,1,0,
			0,0,0,1)
	);
	DirectX::XMVECTOR rayDest = DirectX::XMVector3Unproject(
		XMVectorSet(x, y, 1, 0),
		0,
		0,
		m_screenWidth,
		m_screenHeight,
		0,
		1,
		projectionMatrix,
		viewMatrix,
		XMMatrixSet(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	);
	std::vector<float> v1 = {
		DirectX::XMVectorGetX(rayOrigin),
		DirectX::XMVectorGetY(rayOrigin),
		DirectX::XMVectorGetZ(rayOrigin),
	};
	std::vector<float> v2 = {
		DirectX::XMVectorGetX(rayDest),
		DirectX::XMVectorGetY(rayDest),
		DirectX::XMVectorGetZ(rayDest), };
	DirectX::XMVECTOR rayDirection = rayDest - rayOrigin;
	rayDirection = DirectX::XMVector3Normalize(rayDirection);
	v2 = {
		DirectX::XMVectorGetX(rayDirection),
		DirectX::XMVectorGetY(rayDirection),
		DirectX::XMVectorGetZ(rayDirection), };
	return grid->ifHit(rayOrigin, rayDirection);
}


//switching through celltypes:
void App::makeVisableCells()
{
	switch (cellType)
	{
	case 0:
		dynamic_cast<Grid<::CubeCell>*>(grid)->makeVisableCells(wnd.Gfx(), makeCubeCell);
		break;
	case 1:
		dynamic_cast<Grid<::Hexal>*>(grid)->makeVisableCells(wnd.Gfx(), makeHexal);
	}
}

void App::buildGrid(std::shared_ptr<DataMiner> pMiner)
{
	switch (cellType)
	{
	case 0:
		grid = dynamic_cast<GridBase*>(new Grid<::CubeCell>(pMiner));
		break;
	case 1:
		grid = dynamic_cast<GridBase*>(new Grid<::Hexal>(pMiner));
	}
}
