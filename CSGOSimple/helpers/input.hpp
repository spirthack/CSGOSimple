#pragma once

#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <d3d9.h>

#include "../singleton.hpp"

enum class KeyState
{
    None = 1,
    Down,
    Up,
    Pressed /*Down and then up*/
};

DEFINE_ENUM_FLAG_OPERATORS(KeyState);

class InputSys 
    : public Singleton<InputSys>
{
    friend class Singleton<InputSys>;

    InputSys();
    ~InputSys();

public:
    void Initialize();

    HWND GetMainWindow() const { return m_hTargetWindow; }

    KeyState      GetKeyState(uint32_t vk);
    bool          IsKeyDown(uint32_t vk);
    bool          WasKeyPressed(uint32_t vk);

    void RegisterHotkey(uint32_t vk, std::function<void(void)> f);
    void RemoveHotkey(uint32_t vk);

private:
    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


    HWND            m_hTargetWindow;
    LONG_PTR        m_ulOldWndProc;
    KeyState       m_iKeyMap[256];

    std::function<void(void)> m_Hotkeys[256];
};