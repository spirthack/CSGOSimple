#pragma once
//#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
//#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui/imgui_internal.h"
//#include "imgui/directx9/imgui_impl_dx9.h"
#include <Windows.h>
#include <string>
#include <functional>
#include <vector>

namespace ImGui {
	bool ToggleButton(const char * label, bool * v, const ImVec2 & size_arg = ImVec2(0, 0));
	// Combo box helper allowing to pass an array of strings.
	bool Combo(const char * label, int * currIndex, std::vector<std::string>& values);
	bool BeginGroupBox(const char * name, const ImVec2 & size_arg = ImVec2(0, 0));
	void EndGroupBox();
	bool Hotkey(const char * label, int * k, const ImVec2 & size_arg = ImVec2(0, 0));
	bool ListBox(const char * label, int * current_item, std::string items[], int items_count, int height_items);
	bool ListBox(const char * label, int * current_item, std::function<const char*(int)> lambda, int items_count, int height_in_items);
	bool Combo(const char * label, int * current_item, std::function<const char*(int)> lambda, int items_count, int height_in_items);
}