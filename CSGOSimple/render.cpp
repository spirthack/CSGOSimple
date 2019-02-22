#include "render.hpp"

#include <mutex>

#include "features/visuals.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "droid.hpp"
#include "helpers/math.hpp"

ImFont* g_pDefaultFont;
ImFont* g_pC4Font;
ImFont* g_pIconFont;


//CRITICAL_SECTION render_cs;

Render::Render()
{
	//InitializeCriticalSection(&render_cs);

}

ImDrawListSharedData _data;

std::mutex render_mutex;

void Render::Initialize()
{
	ImGui::CreateContext();


	ImGui_ImplWin32_Init(InputSys::Get().GetMainWindow());
	ImGui_ImplDX9_Init(g_D3DDevice9);

	_data = ImDrawListSharedData();

	draw_list = new ImDrawList(&_data);
	draw_list_act = new ImDrawList(&_data);
	draw_list_rendering = new ImDrawList(&_data);

	GetFonts();
}

void Render::GetFonts() {
	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Droid_compressed_data, Droid_compressed_size, 14.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	g_pDefaultFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Droid_compressed_data, Droid_compressed_size, 18.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	g_pC4Font = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Droid_compressed_data, Droid_compressed_size, 32.f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());

	g_pIconFont = ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(Droid_compressed_data, Droid_compressed_size, 18.f);
}

void Render::ClearDrawList() {
	render_mutex.lock();
	draw_list_act->Clear();
	render_mutex.unlock();
}

void Render::BeginScene() {
	draw_list->Clear();
	draw_list->PushClipRectFullScreen();


	if (g_Options.misc_watermark) //lol
		Render::Get().RenderText("CSGOSimple", 10, 5, 14.f, g_Options.color_watermark);


	if (g_EngineClient->IsInGame() && g_LocalPlayer && g_Options.esp_enabled)
		Visuals::Get().AddToDrawList();


	render_mutex.lock();
	*draw_list_act = *draw_list;
	render_mutex.unlock();
}

ImDrawList* Render::RenderScene() {

	if (render_mutex.try_lock()) {
		*draw_list_rendering = *draw_list_act;
		render_mutex.unlock();
	}

	return draw_list_rendering;
	/*
	if (!draw_list_rendering->VtxBuffer.empty()) {
	draw_data.Valid = true;
	draw_data.CmdLists = &draw_list_rendering;
	draw_data.CmdListsCount = 1;
	draw_data.TotalVtxCount = draw_list_rendering->VtxBuffer.Size;
	draw_data.TotalIdxCount = draw_list_rendering->IdxBuffer.Size;
	}



	if (draw_data.Valid)
	ImGui_ImplDX9_RenderDrawData(&draw_data);*/
}


float Render::RenderText(const std::string& text, const ImVec2& pos, float size, Color color, bool center, ImFont* pFont)
{
	ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, text.c_str());
	if (!pFont->ContainerAtlas) return 0.f;
	draw_list->PushTextureID(pFont->ContainerAtlas->TexID);

	if (center)
	{
		draw_list->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y) + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y) - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y) - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y) + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());

		draw_list->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y), GetU32(color), text.c_str());
	}
	else
	{
		draw_list->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y) + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y) - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y) - 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());
		draw_list->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y) + 1), GetU32(Color(0, 0, 0, color.a())), text.c_str());

		draw_list->AddText(pFont, size, ImVec2(pos.x, pos.y), GetU32(color), text.c_str());
	}

	draw_list->PopTextureID();

	return pos.y + textSize.y;
}

void Render::RenderCircle3D(Vector position, float points, float radius, Color color)
{
	float step = (float)M_PI * 2.0f / points;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vector start(radius * cosf(a) + position.x, radius * sinf(a) + position.y, position.z);
		Vector end(radius * cosf(a + step) + position.x, radius * sinf(a + step) + position.y, position.z);

		Vector start2d, end2d;
		if (g_DebugOverlay->ScreenPosition(start, start2d) || g_DebugOverlay->ScreenPosition(end, end2d))
			return;

		RenderLine(start2d.x, start2d.y, end2d.x, end2d.y, color);
	}
}
