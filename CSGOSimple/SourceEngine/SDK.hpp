#pragma once

#include "Definitions.hpp"

#include "CRC.hpp"
#include "Vector.hpp"
#include "Vector2D.hpp"
#include "Vector4D.hpp"
#include "QAngle.hpp"
#include "CHandle.hpp"
#include "CGlobalVarsBase.hpp"
#include "ClientClass.hpp"
#include "Color.hpp"
#include "IBaseClientDll.hpp"
#include "IClientEntity.hpp"
#include "IClientEntityList.hpp"
#include "IClientNetworkable.hpp"
#include "IClientRenderable.hpp"
#include "IClientThinkable.hpp"
#include "IClientUnknown.hpp"
#include "IPanel.hpp"
#include "ISurface.hpp"
#include "IVEngineClient.hpp"
#include "IEngineTrace.hpp"
#include "PlayerInfo.hpp"
#include "Recv.hpp"
#include "VMatrix.hpp"
#include "IClientMode.hpp"
#include "CInput.hpp"
#include "ICvar.hpp"
#include "Convar.h"

namespace SourceEngine {
     class Interfaces {
     public:
          static IVEngineClient*        Engine();
          static IBaseClientDLL*        Client();
          static IClientEntityList*     EntityList();
          static CGlobalVarsBase*       GlobalVars();
          static IPanel*                VGUIPanel();
          static ISurface*              MatSurface();
          static CInput*                Input();
          static IEngineTrace*          EngineTrace();
          static ICvar*                 CVar();
          static IClientMode*           ClientMode();

     private:
          static IVEngineClient*        m_pEngine;
          static IBaseClientDLL*        m_pClient;
          static IClientEntityList*     m_pEntityList;
          static CGlobalVarsBase*       m_pGlobals;
          static IPanel*                m_pVGuiPanel;
          static ISurface*              m_pVGuiSurface;
          static CInput*                m_pInput;
          static IEngineTrace*          m_pEngineTrace;
          static ICvar*                 m_pCVar;
          static IClientMode*           m_pClientMode;
     };
}