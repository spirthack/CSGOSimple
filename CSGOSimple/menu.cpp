#include "Menu.hpp"
#define NOMINMAX
#include <Windows.h>
#include <chrono>

#include "valve_sdk/csgostructs.hpp"
#include "helpers/input.hpp"
#include "options.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "imgui/directx9/imgui_impl_dx9.h"


// =========================================================
// 
// These are the tabs on the sidebar
// 
// =========================================================
static char* sidebar_tabs[] = {
    "ESP",
    "AIM",
    "MISC",
    "CONFIG"
};

static ConVar* cl_mouseenable = nullptr;

constexpr static float get_sidebar_item_width() { return 150.0f; }
constexpr static float get_sidebar_item_height() { return  50.0f; }

namespace ImGuiEx
{
    inline bool ColorEdit4(const char* label, Color* v, bool show_alpha = true)
    {
        auto clr = ImVec4{
            v->r() / 255.0f,
            v->g() / 255.0f,
            v->b() / 255.0f,
            v->a() / 255.0f
        };

        if(ImGui::ColorEdit4(label, &clr.x, show_alpha)) {
            v->SetColor(clr.x, clr.y, clr.z, clr.w);
            return true;
        }
        return false;
    }
    inline bool ColorEdit3(const char* label, Color* v)
    {
        return ColorEdit4(label, v, false);
    }
}

template<size_t N>
void render_tabs(char* (&names)[N], int& activetab, float w, float h, bool sameline)
{
    bool values[N] = { false };

    values[activetab] = true;

    for(auto i = 0; i < N; ++i) {
        if(ImGui::ToggleButton(names[i], &values[i], ImVec2{ w, h })) {
            activetab = i;
        }
        if(sameline && i < N - 1)
            ImGui::SameLine();
    }
}

ImVec2 get_sidebar_size()
{
    constexpr float padding = 10.0f;
    constexpr auto size_w = padding * 2.0f + get_sidebar_item_width();
    constexpr auto size_h = padding * 2.0f + (sizeof(sidebar_tabs) / sizeof(char*)) * get_sidebar_item_height();

    return ImVec2{ size_w, ImMax(325.0f, size_h) };
}

int get_fps()
{
    using namespace std::chrono;
    static int count = 0;
    static auto last = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    static int fps = 0;

    count++;

    if(duration_cast<milliseconds>(now - last).count() > 1000) {
        fps = count;
        count = 0;
        last = now;
    }

    return fps;
}

void RenderEspTab()
{
    static char* esp_tab_names[] = { "ESP", "GLOW", "CHAMS" };
    static int   active_esp_tab = 0;

    bool placeholder_true = true;

    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
    {
        render_tabs(esp_tab_names, active_esp_tab, group_w / _countof(esp_tab_names), 25.0f, true);
    }
    ImGui::PopStyleVar();
    ImGui::BeginGroupBox("##body_content");
    {
        if(active_esp_tab == 0) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, nullptr, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::Checkbox("Enabled", &g_Options.esp_enabled);
            ImGui::Checkbox("Team check", &g_Options.esp_enemies_only);
            ImGui::Checkbox("Boxes", &g_Options.esp_player_boxes);
            ImGui::Checkbox("Names", &g_Options.esp_player_names);
            ImGui::Checkbox("Health", &g_Options.esp_player_health);
            ImGui::Checkbox("Armour", &g_Options.esp_player_armour);
            ImGui::Checkbox("Weapon", &g_Options.esp_player_weapons);
            ImGui::Checkbox("Snaplines", &g_Options.esp_player_snaplines);

            ImGui::NextColumn();

            ImGui::Checkbox("Crosshair", &g_Options.esp_crosshair);
            ImGui::Checkbox("Dropped Weapons", &g_Options.esp_dropped_weapons);
            ImGui::Checkbox("Defuse Kit", &g_Options.esp_defuse_kit);
            ImGui::Checkbox("Planted C4", &g_Options.esp_planted_c4);

            ImGui::NextColumn();

            ImGui::PushItemWidth(100);
            ImGui::ColorEditMode(ImGuiColorEditMode_HEX);
            ImGuiEx::ColorEdit3("Allies Visible", &g_Options.color_esp_ally_visible);
            ImGuiEx::ColorEdit3("Enemies Visible", &g_Options.color_esp_enemy_visible);
            ImGuiEx::ColorEdit3("Allies Occluded", &g_Options.color_esp_ally_occluded);
            ImGuiEx::ColorEdit3("Enemies Occluded", &g_Options.color_esp_enemy_occluded);
            ImGuiEx::ColorEdit3("Crosshair", &g_Options.color_esp_crosshair);
            ImGuiEx::ColorEdit3("Dropped Weapons", &g_Options.color_esp_weapons);
            ImGuiEx::ColorEdit3("Defuse Kit", &g_Options.color_esp_defuse);
            ImGuiEx::ColorEdit3("Planted C4", &g_Options.color_esp_c4);
            ImGui::PopItemWidth();

            ImGui::Columns(1, nullptr, false);
            ImGui::PopStyleVar();
        } else if(active_esp_tab == 1) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, nullptr, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::Checkbox("Enabled", &g_Options.glow_enabled);
            ImGui::Checkbox("Team check", &g_Options.glow_enemies_only);
            ImGui::Checkbox("Players", &g_Options.glow_players);
            ImGui::Checkbox("Chickens", &g_Options.glow_chickens);
            ImGui::Checkbox("C4 Carrier", &g_Options.glow_c4_carrier);
            ImGui::Checkbox("Planted C4", &g_Options.glow_planted_c4);
            ImGui::Checkbox("Defuse Kits", &g_Options.glow_defuse_kits);
            ImGui::Checkbox("Weapons", &g_Options.glow_weapons);

            ImGui::NextColumn();

            ImGui::PushItemWidth(100);
            ImGui::ColorEditMode(ImGuiColorEditMode_HEX);
            ImGuiEx::ColorEdit3("Ally", &g_Options.color_glow_ally);
            ImGuiEx::ColorEdit3("Enemy", &g_Options.color_glow_enemy);
            ImGuiEx::ColorEdit3("Chickens", &g_Options.color_glow_chickens);
            ImGuiEx::ColorEdit3("C4 Carrier", &g_Options.color_glow_c4_carrier);
            ImGuiEx::ColorEdit3("Planted C4", &g_Options.color_glow_planted_c4);
            ImGuiEx::ColorEdit3("Defuse Kits", &g_Options.color_glow_defuse);
            ImGuiEx::ColorEdit3("Weapons", &g_Options.color_glow_weapons);
            ImGui::PopItemWidth();

            ImGui::NextColumn();

            ImGui::Columns(1, nullptr, false);
            ImGui::PopStyleVar();
        } else if(active_esp_tab == 2) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
            ImGui::Columns(3, nullptr, false);
            ImGui::SetColumnOffset(1, group_w / 3.0f);
            ImGui::SetColumnOffset(2, 2 * group_w / 2.9f);
            ImGui::SetColumnOffset(3, group_w);

            ImGui::BeginGroupBox("Players");
            {
                ImGui::Checkbox("Enabled", &g_Options.chams_player_enabled); ImGui::SameLine();
                ImGui::Checkbox("Team Check", &g_Options.chams_player_enemies_only);
                ImGui::Checkbox("Wireframe", &g_Options.chams_player_wireframe);
                ImGui::Checkbox("Flat", &g_Options.chams_player_flat);
                ImGui::Checkbox("Ignore-Z", &g_Options.chams_player_ignorez); ImGui::SameLine();
                ImGui::Checkbox("Glass", &g_Options.chams_player_glass);
                ImGui::ColorEditMode(ImGuiColorEditMode_HEX);
                ImGui::PushItemWidth(110);
                ImGuiEx::ColorEdit4("Ally (Visible)", &g_Options.color_chams_player_ally_visible);
                ImGuiEx::ColorEdit4("Ally (Occluded)", &g_Options.color_chams_player_ally_occluded);
                ImGuiEx::ColorEdit4("Enemy (Visible)", &g_Options.color_chams_player_enemy_visible);
                ImGuiEx::ColorEdit4("Enemy (Occluded)", &g_Options.color_chams_player_enemy_occluded);
                ImGui::PopItemWidth();
            }
            ImGui::EndGroupBox();

            ImGui::NextColumn();

            ImGui::BeginGroupBox("Arms");
            {
                ImGui::Checkbox("Enabled", &g_Options.chams_arms_enabled);
                ImGui::Checkbox("Wireframe", &g_Options.chams_arms_wireframe);
                ImGui::Checkbox("Flat", &g_Options.chams_arms_flat);
                ImGui::Checkbox("Ignore-Z", &g_Options.chams_arms_ignorez);
                ImGui::Checkbox("Glass", &g_Options.chams_arms_glass);
                ImGui::ColorEditMode(ImGuiColorEditMode_HEX);
                ImGui::PushItemWidth(110);
                ImGuiEx::ColorEdit4("Color (Visible)", &g_Options.color_chams_arms_visible);
                ImGuiEx::ColorEdit4("Color (Occluded)", &g_Options.color_chams_arms_occluded);
                ImGui::PopItemWidth();
            }
            ImGui::EndGroupBox();

            ImGui::Columns(1, nullptr, false);
            ImGui::PopStyleVar();
        }
    }
    ImGui::EndGroupBox();
}

void RenderMiscTab()
{
    bool placeholder_true = true;

    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
    ImGui::ToggleButton("MISC", &placeholder_true, ImVec2{ group_w, 25.0f });
    ImGui::PopStyleVar();

    ImGui::BeginGroupBox("##body_content");
    {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ style.WindowPadding.x, style.ItemSpacing.y });
        ImGui::Columns(3, nullptr, false);
        ImGui::SetColumnOffset(1, group_w / 3.0f);
        ImGui::SetColumnOffset(2, 2 * group_w / 3.0f);
        ImGui::SetColumnOffset(3, group_w);

        ImGui::Checkbox("Bunny hop", &g_Options.misc_bhop);
        ImGui::Checkbox("No hands", &g_Options.misc_no_hands);
        ImGui::PushItemWidth(-1.0f);
        ImGui::SliderInt("viewmodel_fov:", &g_Options.viewmodel_fov, 68, 120);
        ImGui::SliderFloat("mat_ambient_light_r:", &g_Options.mat_ambient_light_r, 0, 1);
        ImGui::SliderFloat("mat_ambient_light_g:", &g_Options.mat_ambient_light_g, 0, 1);
        ImGui::SliderFloat("mat_ambient_light_b:", &g_Options.mat_ambient_light_b, 0, 1);
        ImGui::PopItemWidth();

        ImGui::Columns(1, nullptr, false);
        ImGui::PopStyleVar();
    }
    ImGui::EndGroupBox();
}

void RenderEmptyTab()
{
    auto& style = ImGui::GetStyle();
    float group_w = ImGui::GetCurrentWindow()->Size.x - style.WindowPadding.x * 2;

    bool placeholder_true = true;

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
    ImGui::ToggleButton("AIM", &placeholder_true, ImVec2{ group_w, 25.0f });
    ImGui::PopStyleVar();

    ImGui::BeginGroupBox("##body_content");
    {
        auto message = "There's nothing here yet. Add something you want!";

        auto pos = ImGui::GetCurrentWindow()->Pos;
        auto wsize = ImGui::GetCurrentWindow()->Size;

        pos = pos + wsize / 2.0f;

        ImGui::RenderText(pos - ImGui::CalcTextSize(message) / 2.0f, message);
    }
    ImGui::EndGroupBox();
}

void Menu::Initialize()
{
    _visible = true;

    cl_mouseenable = g_CVar->FindVar("cl_mouseenable");

    ImGui_ImplDX9_Init(InputSys::Get().GetMainWindow(), g_D3DDevice9);

    CreateStyle();
}

void Menu::Shutdown()
{
    ImGui_ImplDX9_Shutdown();
    cl_mouseenable->SetValue(true);
}

void Menu::OnDeviceLost()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void Menu::OnDeviceReset()
{
    ImGui_ImplDX9_CreateDeviceObjects();
}

void Menu::Render()
{
    if(!_visible)
        return;

    ImGui_ImplDX9_NewFrame();

    ImGui::GetIO().MouseDrawCursor = _visible;

    const auto sidebar_size = get_sidebar_size();
    static int active_sidebar_tab = 0;

    ImGui::PushStyle(_style);

    ImGui::SetNextWindowPos(ImVec2{ 0, 0 }, ImGuiSetCond_Once);
    ImGui::SetNextWindowSize(ImVec2{ 1000, 0 }, ImGuiSetCond_Once);

    if(ImGui::Begin("CSGOSimple",
                    &_visible,
                    ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_ShowBorders |
                    ImGuiWindowFlags_NoResize)) {
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2::Zero);
        {
            ImGui::BeginGroupBox("##sidebar", sidebar_size);
            {
                ImGui::GetCurrentWindow()->Flags &= ~ImGuiWindowFlags_ShowBorders;

                render_tabs(sidebar_tabs, active_sidebar_tab, get_sidebar_item_width(), get_sidebar_item_height(), false);
            }
            ImGui::EndGroupBox();
        }
        ImGui::PopStyleVar();
        ImGui::SameLine();

        // Make the body the same vertical size as the sidebar
        // except for the width, which we will set to auto
        auto size = ImVec2{ 0.0f, sidebar_size.y };

        ImGui::BeginGroupBox("##body", size);
        if(active_sidebar_tab == 0) {
            RenderEspTab();
        } else if(active_sidebar_tab == 1) {
            RenderEmptyTab();
        } else if(active_sidebar_tab == 2) {
            RenderMiscTab();
        } else if(active_sidebar_tab == 3) {
            RenderEmptyTab();
        }
        ImGui::EndGroupBox();

        ImGui::TextColored(ImVec4{ 0.0f, 0.5f, 0.0f, 1.0f }, "FPS: %03d", get_fps());
        ImGui::SameLine(ImGui::GetWindowWidth() - 150 - ImGui::GetStyle().WindowPadding.x);
        if(ImGui::Button("Unload", ImVec2{ 150, 25 })) {
            g_Unload = true;
        }
        ImGui::End();
    }

    ImGui::PopStyle();

    ImGui::Render();
}

void Menu::Show()
{
    _visible = true;
    cl_mouseenable->SetValue(false);
}

void Menu::Hide()
{
    _visible = false;
    cl_mouseenable->SetValue(true);
}

void Menu::Toggle()
{
    _visible = !_visible;
    cl_mouseenable->SetValue(!_visible);
}

void Menu::CreateStyle()
{
    _style.Alpha                  = 1.0f;                                // Global alpha applies to everything in ImGui
    _style.WindowPadding          = ImVec2(10, 10);                      // Padding within a window
    _style.WindowMinSize          = ImVec2(100, 100);                    // Minimum window size
    _style.WindowRounding         = 0.0f;                                // Radius of window corners rounding. Set to 0.0f to have rectangular windows
    _style.WindowTitleAlign       = ImVec2(0.0f, 0.5f);                  // Alignment for title bar text
    _style.ChildWindowRounding    = 0.0f;                                // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
    _style.FramePadding           = ImVec2(5, 5);                        // Padding within a framed rectangle (used by most widgets)
    _style.FrameRounding          = 0.0f;                                // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
    _style.ItemSpacing            = ImVec2(5, 5);                        // Horizontal and vertical spacing between widgets/lines
    _style.ItemInnerSpacing       = ImVec2(4, 4);                        // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
    _style.TouchExtraPadding      = ImVec2(0, 0);                        // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
    _style.IndentSpacing          = 21.0f;                               // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
    _style.ColumnsMinSpacing      = 6.0f;                                // Minimum horizontal spacing between two columns
    _style.ScrollbarSize          = 16.0f;                               // Width of the vertical scrollbar, Height of the horizontal scrollbar
    _style.ScrollbarRounding      = 9.0f;                                // Radius of grab corners rounding for scrollbar
    _style.GrabMinSize            = 10.0f;                               // Minimum width/height of a grab box for slider/scrollbar
    _style.GrabRounding           = 0.0f;                                // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
    _style.ButtonTextAlign        = ImVec2(0.5f, 0.5f);                  // Alignment of button text when button is larger than text.
    _style.DisplayWindowPadding   = ImVec2(22, 22);                      // Window positions are clamped to be IsVisible within the display area by at least this amount. Only covers regular windows.
    _style.DisplaySafeAreaPadding = ImVec2(4, 4);                        // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
    _style.AntiAliasedLines       = true;                                // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
    _style.AntiAliasedShapes      = true;                                // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
    _style.CurveTessellationTol   = 1.25f;                               // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.

    _style.Colors[ImGuiCol_Text]                 = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    _style.Colors[ImGuiCol_TextDisabled]         = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    _style.Colors[ImGuiCol_TextSelectedBg]       = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);
    _style.Colors[ImGuiCol_WindowBg]             = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    _style.Colors[ImGuiCol_ChildWindowBg]        = ImVec4(0.10f, 0.10f, 0.10f, 0.00f);
    _style.Colors[ImGuiCol_PopupBg]              = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
    _style.Colors[ImGuiCol_Border]               = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
    _style.Colors[ImGuiCol_BorderShadow]         = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    _style.Colors[ImGuiCol_FrameBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    _style.Colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
    _style.Colors[ImGuiCol_FrameBgActive]        = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
    _style.Colors[ImGuiCol_TitleBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    _style.Colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    _style.Colors[ImGuiCol_TitleBgActive]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    _style.Colors[ImGuiCol_TitleText]            = ImVec4(0.80f, 0.80f, 1.00f, 1.00f);
    _style.Colors[ImGuiCol_MenuBarBg]            = ImVec4(0.40f, 0.40f, 0.55f, 0.80f);
    _style.Colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
    _style.Colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.40f, 0.40f, 0.80f, 0.30f);
    _style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.80f, 0.40f);
    _style.Colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.80f, 0.50f, 0.50f, 0.40f);
    _style.Colors[ImGuiCol_ComboBg]              = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
    _style.Colors[ImGuiCol_CheckMark]            = ImVec4(0.00f, 0.60f, 0.90f, 0.50f);
    _style.Colors[ImGuiCol_SliderGrab]           = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    _style.Colors[ImGuiCol_SliderGrabActive]     = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
    _style.Colors[ImGuiCol_Button]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    _style.Colors[ImGuiCol_ButtonHovered]        = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
    _style.Colors[ImGuiCol_ButtonActive]         = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
    _style.Colors[ImGuiCol_Header]               = ImVec4(0.40f, 0.40f, 0.90f, 0.45f);
    _style.Colors[ImGuiCol_HeaderHovered]        = ImVec4(0.45f, 0.45f, 0.90f, 0.80f);
    _style.Colors[ImGuiCol_HeaderActive]         = ImVec4(0.53f, 0.53f, 0.87f, 0.80f);
    _style.Colors[ImGuiCol_Column]               = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    _style.Colors[ImGuiCol_ColumnHovered]        = ImVec4(0.70f, 0.60f, 0.60f, 1.00f);
    _style.Colors[ImGuiCol_ColumnActive]         = ImVec4(0.90f, 0.70f, 0.70f, 1.00f);
    _style.Colors[ImGuiCol_ResizeGrip]           = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    _style.Colors[ImGuiCol_ResizeGripHovered]    = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
    _style.Colors[ImGuiCol_ResizeGripActive]     = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
    _style.Colors[ImGuiCol_CloseButton]          = ImVec4(0.10f, 0.10f, 0.10f, 0.50f);
    _style.Colors[ImGuiCol_CloseButtonHovered]   = ImVec4(0.40f, 0.00f, 0.00f, 1.00f);
    _style.Colors[ImGuiCol_CloseButtonActive]    = ImVec4(0.70f, 0.20f, 0.00f, 0.83f);
    _style.Colors[ImGuiCol_PlotLines]            = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    _style.Colors[ImGuiCol_PlotLinesHovered]     = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    _style.Colors[ImGuiCol_PlotHistogram]        = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    _style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    _style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}
