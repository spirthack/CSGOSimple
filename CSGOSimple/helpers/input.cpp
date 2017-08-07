#include "input.hpp"

#include "../valve_sdk/sdk.hpp"
#include "../imgui/imgui.h"
#include "../imgui/directx9/imgui_impl_dx9.h"
#include "../menu.hpp"

LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    switch(msg) {
        case WM_LBUTTONDOWN:
            io.MouseDown[0] = true;
            return true;
        case WM_LBUTTONUP:
            io.MouseDown[0] = false;
            return true;
        case WM_RBUTTONDOWN:
            io.MouseDown[1] = true;
            return true;
        case WM_RBUTTONUP:
            io.MouseDown[1] = false;
            return true;
        case WM_MBUTTONDOWN:
            io.MouseDown[2] = true;
            return true;
        case WM_MBUTTONUP:
            io.MouseDown[2] = false;
            return true;
        case WM_XBUTTONDOWN:
            if((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
                io.MouseDown[3] = true;
            else if((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
                io.MouseDown[4] = true;
            return true;
        case WM_XBUTTONUP:
            if((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
                io.MouseDown[3] = false;
            else if((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
                io.MouseDown[4] = false;
            return true;
        case WM_MOUSEWHEEL:
            io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
            return true;
        case WM_MOUSEMOVE:
            io.MousePos.x = (signed short)(lParam);
            io.MousePos.y = (signed short)(lParam >> 16);
            return true;
        case WM_KEYDOWN:
            if(wParam < 256)
                io.KeysDown[wParam] = 1;
            return true;
        case WM_KEYUP:
            if(wParam < 256)
                io.KeysDown[wParam] = 0;
            return true;
        case WM_CHAR:
            // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
            if(wParam > 0 && wParam < 0x10000)
                io.AddInputCharacter((unsigned short)wParam);
            return true;
    }
    return 0;
}

InputSys::InputSys()
    : m_hTargetWindow(nullptr), m_ulOldWndProc(0)
{
}

InputSys::~InputSys()
{
    if(m_ulOldWndProc)
        SetWindowLongPtr(m_hTargetWindow, GWLP_WNDPROC, m_ulOldWndProc);
    m_ulOldWndProc = 0;
}

void InputSys::Initialize()
{
    D3DDEVICE_CREATION_PARAMETERS params;

    if(FAILED(g_D3DDevice9->GetCreationParameters(&params)))
        throw std::runtime_error("[InputSys] GetCreationParameters failed.");

    m_hTargetWindow = params.hFocusWindow;
    m_ulOldWndProc  = SetWindowLongPtr(m_hTargetWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);

    if(!m_ulOldWndProc)
        throw std::runtime_error("[InputSys] SetWindowLongPtr failed.");
}

LRESULT __stdcall InputSys::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Get().ProcessMessage(msg, wParam, lParam);

    if(Menu::Get().IsVisible() && ImGui_ImplDX9_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    return CallWindowProcW((WNDPROC)Get().m_ulOldWndProc, hWnd, msg, wParam, lParam);
}

bool InputSys::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_LBUTTONDBLCLK:
        case WM_XBUTTONDBLCLK:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_XBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        case WM_LBUTTONUP:
        case WM_XBUTTONUP:
            return ProcessMouseMessage(uMsg, wParam, lParam);
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
            return ProcessKeybdMessage(uMsg, wParam, lParam);
        default:
            return false;
    }
}

bool InputSys::ProcessMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto key = VK_LBUTTON;
    auto state = KeyState::None;
    switch(uMsg) {
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            state = uMsg == WM_MBUTTONUP ? KeyState::Up : KeyState::Down;
            key = VK_MBUTTON;
            break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            state = uMsg == WM_RBUTTONUP ? KeyState::Up : KeyState::Down;
            key = VK_RBUTTON;
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            state = uMsg == WM_LBUTTONUP ? KeyState::Up : KeyState::Down;
            key = VK_LBUTTON;
            break;
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
            state = uMsg == WM_XBUTTONUP ? KeyState::Up : KeyState::Down;
            key = (HIWORD(wParam) == XBUTTON1 ? VK_XBUTTON1 : VK_XBUTTON2);
            break;
        default:
            return false;
    }

    if(state == KeyState::Up && m_iKeyMap[key] == KeyState::Down)
        m_iKeyMap[key] = KeyState::Pressed;
    else
        m_iKeyMap[key] = state;
    return true;
}

bool InputSys::ProcessKeybdMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto key = wParam;
    auto state = KeyState::None;

    switch(uMsg) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            state = KeyState::Down;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            state = KeyState::Up;
            break;
        default:
            return false;
    }

    if(state == KeyState::Up && m_iKeyMap[int(key)] == KeyState::Down) {
        m_iKeyMap[int(key)] = KeyState::Pressed;

        auto& hotkey_callback = m_Hotkeys[key];

        if(hotkey_callback)
            hotkey_callback();

    } else {
        m_iKeyMap[int(key)] = state;
    }
    
    return true;
}
KeyState InputSys::GetKeyState(std::uint32_t vk)
{
    return m_iKeyMap[vk];
}
bool InputSys::IsKeyDown(std::uint32_t vk)
{
    return m_iKeyMap[vk] == KeyState::Down;
}
bool InputSys::WasKeyPressed(std::uint32_t vk)
{
    if(m_iKeyMap[vk] == KeyState::Pressed) {
        m_iKeyMap[vk] = KeyState::Up;
        return true;
    }
    return false;
}

void InputSys::RegisterHotkey(std::uint32_t vk, std::function<void(void)> f)
{
    m_Hotkeys[vk] = f;
}
void InputSys::RemoveHotkey(std::uint32_t vk)
{
    m_Hotkeys[vk] = nullptr;
}