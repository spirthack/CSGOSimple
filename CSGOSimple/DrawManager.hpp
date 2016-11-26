#pragma once

#include <memory>
#include <string>
#include <vector>
#include <windows.h>

#include <d3d9.h>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_internal.h"

#define RGBA(r, g, b, a) ((a << 24) | (b << 16) | (g << 8) | r)

enum text_flags
{ 
    centered_x  = 1,
    centered_y  = 2,
    outline     = 4,
    drop_shadow = 8
};

DEFINE_ENUM_FLAG_OPERATORS(text_flags)

class DrawManager
{
public:
    DrawManager(IDirect3DDevice9* device);
    ~DrawManager();

    void CreateObjects();
    void InvalidateObjects();
    void BeginRendering();
    void EndRendering();

    void AddText(ImVec2 point, ImU32 color, text_flags flags, const char* format, ...);
    void AddRect(const ImVec2& a, float w, float h, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ~0, float thickness = 1.0f);

    void AddLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.0f);
    void AddRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ~0, float thickness = 1.0f);
    void AddRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ~0);                  
    void AddRectFilledMultiColor(const ImVec2& a, const ImVec2& b, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left);
    void AddQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness = 1.0f);
    void AddQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col);
    void AddTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness = 1.0f);
    void AddTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col);
    void AddCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12, float thickness = 1.0f);
    void AddCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12);
    void AddPolyline(const ImVec2* points, const int num_points, ImU32 col, bool closed, float thickness, bool anti_aliased);
    void AddConvexPolyFilled(const ImVec2* points, const int num_points, ImU32 col, bool anti_aliased);
    void AddBezierCurve(const ImVec2& pos0, const ImVec2& cp0, const ImVec2& cp1, const ImVec2& pos1, ImU32 col, float thickness, int num_segments = 0);

private:
    ImDrawData* GetDrawData();

    IDirect3DDevice9*   _device;
    IDirect3DTexture9*  _texture;
    ImDrawList*         _drawList;
    ImDrawData          _drawData;
    ImFontAtlas         _fonts;

    const std::string   FontName = "Tahoma";
    const float         FontSize = 14.0f;
};
