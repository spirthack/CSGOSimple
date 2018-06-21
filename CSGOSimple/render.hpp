#pragma once
#include <string>
#include <stdint.h>
#include <d3d9.h>

#include "singleton.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/directx9/imgui_impl_dx9.h"

#include "valve_sdk/misc/Color.hpp"

extern ImFont* g_pDefaultFont;

class Render
	: public Singleton<Render>
{
	friend class Singleton<Render>;
private:
	
public:
	void Initialize();
	void BeginScene();
	void EndScene();
	float RenderText(const std::string& text, const ImVec2& position, float size, uint32_t color, bool center = false, ImFont* pFont = g_pDefaultFont);
	void RenderLine(const ImVec2& from, const ImVec2& to, uint32_t color, float thickness = 1.f);
	void RenderCircle(const ImVec2& position, float radius, uint32_t color, float thickness = 1.f);
	void RenderCircleFilled(const ImVec2& position, float radius, uint32_t color);
	void RenderBox(const ImVec2 & from, const ImVec2 & to, uint32_t color, float thickness = 1.f, float rounding = 0.f);
	void RenderBoxFilled(const ImVec2 & from, const ImVec2 & to, uint32_t color, float thickness = 1.f, float rounding = 0.f);

	template <class T>
	inline void RenderBox(T x1, T y1, T x2, T y2, Color clr, float thickness = 1.f, float rounding = 0.f) {
		RenderBox(ImVec2(x1, y1), ImVec2(x2, y2), D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a()), thickness, rounding);
	}
	template <class T>
	inline void RenderBoxFilled(T x1, T y1, T x2, T y2, Color clr, float thickness = 1.f, float rounding = 0.f) {
		RenderBoxFilled(ImVec2(x1, y1), ImVec2(x2, y2), D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a()), thickness, rounding);
	}
	template <class T>
	inline void RenderLine(T x1, T y1, T x2, T y2, Color clr, float thickness = 1.f) {
		return RenderLine(ImVec2(x1, y1), ImVec2(x2, y2), D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a()), thickness);
	}
	template <class T>
	inline float RenderText(const std::string& text, T x, T y, float size, Color clr, bool center = false, ImFont* pFont = g_pDefaultFont) {
		return RenderText(text, ImVec2(x, y), size, D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a()), center, pFont);
	}
	template <class T>
	inline void RenderCircle(T x, T y, float radius, Color clr) {
		return RenderCircle(ImVec2(x, y), radius, D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a()));
	}
	template <class T>
	inline void RenderCircleFilled(T x, T y, float radius, Color clr) {
		return RenderCircleFilled(ImVec2(x, y), radius, D3DCOLOR_RGBA(clr.r(), clr.g(), clr.b(), clr.a()));
	}
};