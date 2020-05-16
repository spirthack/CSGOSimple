#pragma once

#include <string>
#include <functional>
#include "singleton.hpp"
#include "imgui/imgui.h"

struct IDirect3DDevice9;

class CTab
{
public:
    const char* szName = nullptr;
    std::function<void()> pRenderFunction = nullptr;
};

class Menu
    : public Singleton<Menu>
{
public:
    void Initialize();
    void Shutdown();

    void OnDeviceLost();
    void OnDeviceReset();

    void Render();

    void Toggle();

    bool IsVisible() const { return _visible; }

private:
    void CreateStyle();

    ImGuiStyle        _style;
    bool              _visible;
 
};