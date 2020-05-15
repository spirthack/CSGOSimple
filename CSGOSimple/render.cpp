#define IMGUI_DEFINE_MATH_OPERATORS
#include "render.hpp"
#include <mutex>
#include "features/visuals.hpp"
#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "helpers/math.hpp"

ImFont* g_pDefaultFont = nullptr;

std::mutex render_mutex;

void Render::Initialize()
{
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(InputSys::Get().GetMainWindow());
	ImGui_ImplDX9_Init(g_D3DDevice9);

	m_draw_list = std::make_shared<ImDrawList>(ImGui::GetDrawListSharedData());
	m_temp_draw_list = std::make_shared<ImDrawList>(ImGui::GetDrawListSharedData());

	auto& io = ImGui::GetIO();

	ImFontConfig font_config;
	font_config.RasterizerFlags = ImGuiFreeType::ForceAutoHint;

	g_pDefaultFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 14.f, &font_config, io.Fonts->GetGlyphRangesCyrillic());

	ImGuiFreeType::BuildFontAtlas(io.Fonts);
}

ImVec2 Render::Text(const std::string& txt, const ImVec2& pos, float size, const Color& clr, ImFont* font, int flags)
{
	if (!font->ContainerAtlas)
		return ImVec2();

	auto new_pos = pos;
	auto text_size = font->CalcTextSizeA(size, FLT_MAX, 0.f, txt.c_str());

	flags& FONT_CENTERED_X ? new_pos.x -= text_size.x * 0.5f : 0;

	flags& FONT_CENTERED_Y ? new_pos.y -= text_size.y * 0.5f : 0;

	m_draw_list->PushTextureID(font->ContainerAtlas->TexID);

	flags& FONT_DROP_SHADOW ? m_draw_list->AddText(font, size, ImVec2(new_pos.x + 1, new_pos.y + 1), Color(clr.a()).GetRawColor(), txt.c_str()) : 0;

	if (flags & FONT_OUTLINE) {
		m_draw_list->AddText(font, size, ImVec2(new_pos.x + 1, new_pos.y + 1), Color(clr.a()).GetRawColor(), txt.c_str());
		m_draw_list->AddText(font, size, ImVec2(new_pos.x - 1, new_pos.y - 1), Color(clr.a()).GetRawColor(), txt.c_str());
		m_draw_list->AddText(font, size, ImVec2(new_pos.x + 1, new_pos.y - 1), Color(clr.a()).GetRawColor(), txt.c_str());
		m_draw_list->AddText(font, size, ImVec2(new_pos.x - 1, new_pos.y + 1), Color(clr.a()).GetRawColor(), txt.c_str());
	}

	m_draw_list->AddText(font, size, ImVec2(new_pos.x, new_pos.y), clr.GetRawColor(), txt.c_str());

	m_draw_list->PopTextureID();

	return ImVec2(pos.x + text_size.x, pos.y + text_size.y);
}

void Render::Line(const ImVec2& from, const ImVec2& to, const Color& clr)
{
	m_draw_list->AddLine(from, to, clr.GetRawColor());
}

void Render::Rect(const ImVec2& pos, const ImVec2& size, const Color& clr)
{
	m_draw_list->AddRect(pos, pos + size, clr.GetRawColor());
}

void Render::RectFilled(const ImVec2& pos, const ImVec2& size, const Color& clr)
{
	m_draw_list->AddRectFilled(pos, pos + size, clr.GetRawColor());
}

void Render::Box(SRect r, Color color)
{
	Rect({ r.left, r.top }, { r.right, r.bottom }, color);
}

void Render::AddToDrawList()
{
	std::unique_lock<std::mutex> lock(render_mutex, std::try_to_lock);
	if (!lock.owns_lock())
		return;

	*ImGui::GetBackgroundDrawList() = *m_temp_draw_list;
}

void Render::Begin()
{
	m_draw_list->Clear();
	m_draw_list->PushClipRectFullScreen();

	Visuals::Get().Render();
	{
		std::unique_lock<std::mutex> lock(render_mutex);
		*m_temp_draw_list = *m_draw_list;
	}
}




