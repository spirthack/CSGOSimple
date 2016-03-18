#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <memory> //unique_ptr & make_unique

#include "SourceEngine/SDK.hpp"
#include "VFTableHook.hpp"

//Link with the D3D9 implementations
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace Hooks
{
     ///<summary>
     /// Sets up and hooks some functions. This should be called when the dll is loaded.
     ///</summary>
     void Initialize();

     ///<summary>
     /// Cleans up and removes the hooks. This should be called when the dll is unloaded.
     ///</summary>
     void Restore();

     ///<summary>
     /// Initialized the GUI system. Called from EndScene.
     ///</summary>
     ///<param name="pDevice"> Pointer to the D3D9 Device </param>
     void GUI_Init(IDirect3DDevice9* pDevice);

     //---------------------------------------------------
     // Hook prototypes
     //---------------------------------------------------
     typedef long(__stdcall* EndScene_t)(IDirect3DDevice9* device);
     typedef long(__stdcall* Reset_t)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pp);
     typedef bool(__thiscall* CreateMove_t)(SourceEngine::IClientMode*, float, SourceEngine::CUserCmd*);
     typedef void(__thiscall* PlaySound_t)(SourceEngine::ISurface*, const char*);

     //---------------------------------------------------
     // Hooked functions
     //---------------------------------------------------
     HRESULT   __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);
     HRESULT   __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
     bool      __stdcall Hooked_CreateMove(float sample_input_frametime, SourceEngine::CUserCmd* pCmd);
     void      __stdcall Hooked_PlaySound(const char* szFileName);

     //We're gonna replace the main window's WNDPROC for this one to capture user input and pass it down to the GUI
     LRESULT   __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}

