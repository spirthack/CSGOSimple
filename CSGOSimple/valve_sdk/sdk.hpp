#pragma once

#define NOMINMAX
#include <Windows.h>

#include "misc/Enums.hpp"
#include "misc/vfunc.hpp"

#include "math/VMatrix.hpp"
#include "math/QAngle.hpp"
#include "math/Vector.hpp"
#include "misc/Studio.hpp"

#include "interfaces/IAppSystem.hpp"
#include "interfaces/IBaseClientDll.hpp"
#include "interfaces/IClientEntity.hpp"
#include "interfaces/IClientEntityList.hpp"
#include "interfaces/IClientMode.hpp"
#include "interfaces/IConVar.hpp"
#include "interfaces/ICvar.hpp"
#include "interfaces/IEngineTrace.hpp"
#include "interfaces/IVEngineClient.hpp"
#include "interfaces/IVDebugOverlay.hpp"
#include "interfaces/ISurface.hpp"
#include "interfaces/CInput.hpp"
#include "interfaces/IVModelInfoClient.hpp"
#include "interfaces/IVModelRender.hpp"
#include "interfaces/IRenderView.hpp"
#include "interfaces/IGameEventmanager.hpp"
#include "interfaces/IMaterialSystem.hpp"
#include "interfaces/IMoveHelper.hpp"
#include "interfaces/IMDLCache.hpp"
#include "interfaces/IPrediction.hpp"
#include "interfaces/IPanel.hpp"
#include "interfaces/IEngineSound.hpp"
#include "interfaces/IViewRender.hpp"
#include "interfaces/CClientState.hpp"
#include "interfaces/IPhysics.hpp"

#include "misc/Convar.hpp"
#include "misc/CUserCmd.hpp"
#include "misc/glow_outline_effect.hpp"

#include "netvars.hpp"

struct IDirect3DDevice9;

namespace Interfaces
{
    void Initialize();
    void Dump();
}

extern IVEngineClient*       g_EngineClient;
extern IBaseClientDLL*       g_CHLClient;
extern IClientEntityList*    g_EntityList;
extern CGlobalVarsBase*      g_GlobalVars;
extern IEngineTrace*         g_EngineTrace;
extern ICvar*                g_CVar;
extern IPanel*               g_VGuiPanel;
extern IClientMode*          g_ClientMode;
extern IVDebugOverlay*       g_DebugOverlay;
extern ISurface*             g_VGuiSurface;
extern CInput*               g_Input;
extern IVModelInfoClient*    g_MdlInfo;
extern IVModelRender*        g_MdlRender;
extern IVRenderView*         g_RenderView;
extern IMaterialSystem*      g_MatSystem;
extern IGameEventManager2*   g_GameEvents;
extern IMoveHelper*          g_MoveHelper;
extern IMDLCache*            g_MdlCache;
extern IPrediction*          g_Prediction;
extern CGameMovement*        g_GameMovement;
extern IEngineSound*         g_EngineSound;
extern CGlowObjectManager*   g_GlowObjManager;
extern IViewRender*          g_ViewRender;
extern IDirect3DDevice9*     g_D3DDevice9;
extern CClientState*         g_ClientState;
extern IPhysicsSurfaceProps* g_PhysSurface;

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

#include "misc/EHandle.hpp"

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

extern C_LocalPlayer g_LocalPlayer;

