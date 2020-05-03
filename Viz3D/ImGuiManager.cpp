#include "ImGuiManager.h"
#include "imgui.h"
ImguiManager::ImguiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::StyleColorsClassic();
}

ImguiManager::~ImguiManager() 
{
	ImGui::DestroyContext();
}
