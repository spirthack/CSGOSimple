#pragma once

#define NOMINMAX
#include <Windows.h>

#include "Misc/Enums.hpp"
#include "Misc/vfunc.hpp"

#include "Math/VMatrix.hpp"
#include "Math/QAngle.hpp"
#include "Math/Vector.hpp"
#include "Misc/Studio.hpp"

#include "Interfaces/IAppSystem.hpp"
#include "Interfaces/IBaseClientDll.hpp"
#include "Interfaces/IClientEntity.hpp"
#include "Interfaces/IClientEntityList.hpp"
#include "Interfaces/IClientMode.hpp"
#include "Interfaces/IConVar.hpp"
#include "Interfaces/ICvar.hpp"
#include "Interfaces/IEngineTrace.hpp"
#include "Interfaces/IVEngineClient.hpp"
#include "Interfaces/IVDebugOverlay.hpp"
#include "Interfaces/ISurface.hpp"
#include "Interfaces/CInput.hpp"
#include "Interfaces/IVModelInfoClient.hpp"
#include "Interfaces/IVModelRender.hpp"
#include "Interfaces/IRenderView.hpp"
#include "Interfaces/IGameEventmanager.hpp"
#include "Interfaces/IMaterialSystem.hpp"
#include "Interfaces/IMoveHelper.hpp"
#include "Interfaces/IMDLCache.hpp"
#include "Interfaces/IPrediction.hpp"
#include "Interfaces/IPanel.hpp"
#include "Interfaces/IEngineSound.hpp"
#include "Interfaces/IViewRender.hpp"
#include "Interfaces/CClientState.hpp"
#include "Interfaces/IPhysics.hpp"
#include "Interfaces/IInputSystem.hpp"
#include "interfaces/IRefCounted.hpp"

#include "Misc/Convar.hpp"
#include "Misc/CUserCmd.hpp"
#include "Misc/glow_outline_effect.hpp"
#include "Misc/datamap.hpp"

#include "netvars.hpp"

struct IDirect3DDevice9;

namespace Interfaces
{
    void Initialize();
    void Dump();
}

inline IVEngineClient*       g_EngineClient;
inline IBaseClientDLL*       g_CHLClient;
inline IClientEntityList*    g_EntityList;
inline CGlobalVarsBase*      g_GlobalVars;
inline IEngineTrace*         g_EngineTrace;
inline ICvar*                g_CVar;
inline IPanel*               g_VGuiPanel;
inline IClientMode*          g_ClientMode;
inline IVDebugOverlay*       g_DebugOverlay;
inline ISurface*             g_VGuiSurface;
inline CInput*               g_Input;
inline IVModelInfoClient*    g_MdlInfo;
inline IVModelRender*        g_MdlRender;
inline IVRenderView*         g_RenderView;
inline IMaterialSystem*      g_MatSystem;
inline IGameEventManager2*   g_GameEvents;
inline IMoveHelper*          g_MoveHelper;
inline IMDLCache*            g_MdlCache;
inline IPrediction*          g_Prediction;
inline CGameMovement*        g_GameMovement;
inline IEngineSound*         g_EngineSound;
inline CGlowObjectManager*   g_GlowObjManager;
inline IViewRender*          g_ViewRender;
inline IDirect3DDevice9*     g_D3DDevice9;
inline CClientState*         g_ClientState;
inline IPhysicsSurfaceProps* g_PhysSurface;
inline IInputSystem*         g_InputSystem;

template<typename... Args>
void ConMsg(const char* pMsg, Args... args)
{
    static auto import = (void(*)(const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConMsg@@YAXPBDZZ");
    return import(pMsg, args...);
}
template<typename... Args>
void ConColorMsg(const Color& clr, const char* pMsg, Args... args)
{
    static auto import = (void(*)(const Color&, const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");
    return import(clr, pMsg, args...);
}

#include "Misc/EHandle.hpp"

class C_LocalPlayer
{
    friend bool operator==(const C_LocalPlayer& lhs, void* rhs);
public:
    C_LocalPlayer() : m_local(nullptr) {}

    operator bool() const { return *m_local != nullptr; }
    operator C_BasePlayer*() const { return *m_local; }

    C_BasePlayer* operator->() { return *m_local; }

private:
    C_BasePlayer** m_local;
};

inline C_LocalPlayer g_LocalPlayer;

