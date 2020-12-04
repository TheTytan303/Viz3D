#ifdef _DEBUG
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
//#define new DEBUG_NEW
#else
#define DBG_NEW new
#endif

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
#include <stdlib.h>
#include <ctime>
#include <vector>
#include <string>
#include <memory>


//Specific CellMakers:
shared_ptr<Hexal> makeHexal(unsigned short* size, std::shared_ptr<Cell> c, Graphics& gfx)
{
	return std::make_shared<Hexal>(size, c, gfx);
}

//Specyfic CellSetters
shared_ptr<CellView> HexalSetter(shared_ptr<Cell> cell, GridBase* base)
{
	int x = cell.get()->getMeshCoords()[0];
	int y = cell.get()->getMeshCoords()[1];
	int z = cell.get()->getMeshCoords()[2];
	shared_ptr<CellView> target = make_shared<CellView>();
	target->cell = cell;
	target->neighbours = 14u;

	shared_ptr<CellView> tmp;

	// góra
	tmp = base->getCellView(x, y+2, z);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}

	// góra - œrodek

	tmp = base->getCellView(x - 1, y+1, z-1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x + 1, y+1, z+1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x+1, y+1, z - 1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x-1, y+1, z + 1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}

	// œrodek
	tmp = base->getCellView(x-1, y, z);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x+1, y, z);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x, y, z-1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x, y, z+1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}

	//  dó³ - œrodek

	tmp = base->getCellView(x - 1, y - 1, z - 1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x + 1, y - 1, z + 1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x + 1, y - 1, z - 1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	tmp = base->getCellView(x - 1, y - 1, z + 1);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	// dó³
	tmp = base->getCellView(x, y - 2, z);
	if (tmp != nullptr)
	{
		target->neighbours -= 1;
		tmp->neighbours -= 1;
	}
	return target;
}



App::App()
	:
	wnd(windowWidth, windowHeight, L"App window")
{

	wnd.Gfx().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 200.0f));
	camera.SetCamera(30, 0, 0, 0, 0, 0);

}
int licznik = 0;
int App::Go()
{

	//std::wstringstream wss;
	//wss << "C:\\Users\\wcies\\source\\repos\\Viz3D\\samples\\state_10x10x10.txt";
	//filepath = wss.str();

	mineData();


	std::vector<float> v1 = {0,0,0};
	stars.push_back(std::make_unique<Star>(v1, 1.0f, 1.0f, 0.0f, wnd.Gfx()));
	MSG msg;
	BOOL gResult;
	//_CrtDumpMemoryLeaks();
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
					grid->clearPickedCells();
				}
				shared_ptr<Cell> picked = getPickedItem(e.GetPosX(), e.GetPosY(), 800, 600);
				
				//if( picked != nullptr)
				//{
				//	pickedCells.push_back(picked);
				//}
				//ShowPickedFrame();
				vector<shared_ptr<Cell>> pickedCells = grid->getPickedCells();
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

	grid.reset();
	//Sleep(5000);
	_CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}

void App::mineData() 
{
	//picked = nullptr;
	if (grid != nullptr)
	{
		grid->clearPickedCells();
	}
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
	{
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
	if (grid != nullptr )
	{
		grid->Draw(wnd.Gfx());
	}
	}
	//*
	if (show_gui_window)
	{
		if (grid != nullptr)
		{

			if (ImGui::Begin("Grid Data:"))
			{
				int count = (int)Cell::getNames().size();
				if (count != 0)
				{

					if (ImGui::Button("Show grains"))
					{
						grid->resetColors(wnd.Gfx());
					}
					ImGui::Text("variables");
					for (int i = 0; i < count; i++)
					{
						std::ostringstream oss;
						oss << Cell::getNames().at(i);
						oss << "(";
						oss << grid->getMinis()[i] << " - ";
						oss << grid->getMaxes()[i];
						oss << ")";
						//ImGui::Text(oss.str().c_str());
						if (ImGui::Button(oss.str().c_str()))
						{
							grid->showVariable(i, wnd.Gfx());
							ImGui::SameLine();
						}
						//oss << Cell::getNames().at(i) << ": ";
					}
				}
				else
				{
					ImGui::Text("no variables specified");
				}
			}
			ImGui::End();
			
			
			vector<shared_ptr<Cell>> pickedCells = grid->getPickedCells();
			if (pickedCells.size() > 0)
			{
				if (ImGui::Begin("Picked Cells: "))
				{
					for (int i = 0; i < pickedCells.size(); i++)
					{
						ImGui::Text("Grain ID: %d", pickedCells[i]->getGrain());
						ImGui::Text("coords: %d x %d x %d",
							pickedCells[i]->getMeshCoords()[0],
							pickedCells[i]->getMeshCoords()[1],
							pickedCells[i]->getMeshCoords()[2]);
						for (int j = 0; j < CubeCell::getNames().size(); j++)
						{
							std::ostringstream oss;
							oss << CubeCell::getNames().at(j);
							oss << ": ";
							oss << pickedCells[i]->getDetails().at(j);
							ImGui::Text(oss.str().c_str());
						}
					}
				}
				ImGui::End();
			}


		}

		if (ImGui::Begin("Input files"))
		{
			if (ImGui::Button("pickDataFile"))
			{
				openDataFile();
			}
			if (ImGui::Button("pickColorFile"))
			{
				openColorFile();
			}
		}
		ImGui::End();

		if (ImGui::Begin("Celling Type"))
		{
			ImGui::RadioButton("Cube Cell", &cellType, 0);
			ImGui::RadioButton("Hexal", &cellType, 1);
			if (ImGui::Button("Rebuild"))
			{
				if (pMiner != nullptr)
				{
					buildGrid(pMiner);
					makeVisableCells();
				}
			}
		}
		ImGui::End();

		if (ImGui::Begin("Framing"))
		{
			switch (cellType)
			{
			case 0:
				ImGui::Checkbox("BasicFrames", &CubeCell::frameDrawing);
				break;
			case 1:
				ImGui::Checkbox("BasicFrames", &Hexal::frameDrawing);
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
	/*unsigned short* tmp = pMiner->meshSize;
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
	}*/
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
void App::openDataFile()
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
void App::openColorFile()
{
	IFileDialog* dialog = NULL;
	HRESULT hr = CoCreateInstance(
		CLSID_FileOpenDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&dialog));
	if (SUCCEEDED(hr))
	{

		DWORD dwFlags;
		hr = dialog->GetOptions(&dwFlags);
		if (SUCCEEDED(hr))
		{
			hr = dialog->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
			if (SUCCEEDED(hr))
			{
				hr = dialog->SetDefaultExtension(L"txt;csv");
				if (SUCCEEDED(hr))
				{
					hr = dialog->Show(NULL);
					if (SUCCEEDED(hr))
					{
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
								wstring path = wss.str();
								shared_ptr<ColorPicker> colorPicker = make_shared<ColorPicker>(path);
								colorPicker->loadColors();

								//ColorPicker colorPicker(path);
								//colorPicker.loadColors();
								makeVisableCells();
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
	return grid->pickCell(rayOrigin, rayDirection, wnd.Gfx());
}


//switching through celltypes:
void App::makeVisableCells() //rebuild
{
	switch (cellType)
	{
	case 0:
		dynamic_cast<Grid<::CubeCell, ::CubeFrame>*>(grid.get())->makeVisableCells(wnd.Gfx()); //default making CubeCell (Moore neighbour)
		break;
	case 1:
		dynamic_cast<Grid<::Hexal, ::HexalFrame>*>(grid.get())->makeVisableCells(wnd.Gfx(), makeHexal);
	}
}

void App::buildGrid(std::shared_ptr<DataMiner> pMiner) //initiazlization
{
	grid.release();
	switch (cellType)
	{
	case 0:
		grid.reset(dynamic_cast<GridBase*>(new Grid<::CubeCell, ::CubeFrame>(pMiner))); //default making CubeCell (Moore neighbour)
		break;
	case 1:
		grid.reset(dynamic_cast<GridBase*>(new Grid<::Hexal, ::HexalFrame>(pMiner, HexalSetter)));
	}
}
