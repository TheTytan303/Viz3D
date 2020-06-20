#include <Windows.h>
#include "Window.h"
#include "Services.cpp"
#include <iostream>
#include <sstream>
#include "app.h"

#include <Wininet.h>
#include <ShlObj.h>
#include <iostream>

using namespace std;

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
	) {

	try {
		return App().Go();
	}
	catch (const CustomException& e) {
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch ( ... ) {
		MessageBoxA(nullptr, "No details", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}