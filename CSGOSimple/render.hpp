#pragma once
#include <string>
#include <sstream>
#include <stdint.h>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "freetype.lib")

#include "singleton.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_freetype.h"
#include "valve_sdk/misc/Color.hpp"
#include "valve_sdk/math/SRect.hpp"

enum EFontsFlags
{
	FONT_NONE = 0,
	FONT_CENTERED_X = (1 << 0),
	FONT_CENTERED_Y = (1 << 1),
	FONT_CENTERED = FONT_CENTERED_X | FONT_CENTERED_Y,
	FONT_DROP_SHADOW = (1 << 2),
	FONT_OUTLINE = (1 << 3)
};

extern ImFont* g_pDefaultFont;


class Render
	: public Singleton<Render>
{
public:
	void Initialize();
	void AddToDrawList();

	void Begin();

	ImVec2 Text(const std::string& txt, const ImVec2& pos, float size, const Color& clr, ImFont* font = g_pDefaultFont, int flags = FONT_NONE);

	void Line(const ImVec2& from, const ImVec2& to, const Color& clr);

	void Rect(const ImVec2& pos, const ImVec2& size, const Color& clr);

	void RectFilled(const ImVec2& pos, const ImVec2& size, const Color& clr);

	void Box(SRect r, Color color);
private:
	std::shared_ptr<ImDrawList> m_draw_list;
	std::shared_ptr<ImDrawList> m_temp_draw_list;
};