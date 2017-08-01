//-----------------------------------------------------------------------------
// USER IMPLEMENTATION
// This file contains compile-time options for ImGui.
// Other options (memory allocation overrides, callbacks, etc.) can be Set at runtime via the ImGuiIO structure - ImGui::GetIO().
//-----------------------------------------------------------------------------

#pragma once

//---- Define assertion handler. Defaults to calling assert().
//#define IM_ASSERT(_EXPR)  MyAssert(_EXPR)

//---- Define attributes of all API symbols declarations, e.g. for DLL under Windows.
//#define IMGUI_API __declspec( dllexport )
//#define IMGUI_API __declspec( dllimport )

//---- Include imgui_user.h at the end of imgui.h
//#define IMGUI_INCLUDE_IMGUI_USER_H

//---- Don't implement default handlers for Windows (so as not to link with OpenClipboard() and others Win32 functions)
#define IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCS
#define IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCS

//---- Don't implement help and test window functionality (ShowUserGuide()/ShowStyleEditor()/ShowTestWindow() methods will be empty)
#define IMGUI_DISABLE_TEST_WINDOWS

//---- Don't define obsolete functions names
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

//---- Pack colors to BGRA instead of RGBA (remove need to post process vertex buffer in back ends)
//#define IMGUI_USE_BGRA_PACKED_COLOR

//---- Implement STB libraries in a namespace to avoid conflicts
#define IMGUI_STB_NAMESPACE     ImGuiStb

//---- Define constructor and implicit cast operators to convert back<>forth from your Math types and ImVec2/ImVec4.
/*
#define IM_VEC2_CLASS_EXTRA                                                    \
        ImVec2 operator+(const ImVec2& f) const { return ImVec2{x + f.x, y + f.y}; } \
        ImVec2 operator-(const ImVec2& f) const { return ImVec2{x - f.x, y - f.y}; } \
        ImVec2& operator+=(const ImVec2& f) { x += f.x; y += f.y; return *this; }   \
        ImVec2& operator-=(const ImVec2& f) { x -= f.x; y -= f.y; return *this; }   \
*/
#define IM_VEC4_CLASS_EXTRA                                               \
        ImVec4(const Vector& f) { x = f.x; y = f.y; z = f.z; w = 0; }     \
        operator Vector() const { return Vector(x,y,z); }                 \
        ImVec4(const Color& f) { x = f.r() / 255.0f; y = f.g() / 255.0f; z = f.b() / 255.0f; w = f.a() / 255.0f; }     \
        operator Color() const { return Color(x,y,z,w); }


//---- Tip: You can add extra functions within the ImGui:: namespace, here or in your own headers files.
//---- e.g. create variants of the ImGui::Value() helper for your low-level Math types, or your own widgets/helpers.
/*
namespace ImGui
{
    void    Value(const char* prefix, const MyMatrix44& v, const char* float_format = NULL);
}
*/

