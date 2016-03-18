#include "SDK.hpp"

#include "../Utils.hpp"
#include "../XorStr.hpp"

namespace SourceEngine {
     
     IVEngineClient*     Interfaces::m_pEngine         = nullptr;
     IBaseClientDLL*     Interfaces::m_pClient         = nullptr;
     IClientEntityList*  Interfaces::m_pEntityList     = nullptr;
     CGlobalVarsBase*    Interfaces::m_pGlobals        = nullptr;
     IPanel*             Interfaces::m_pVGuiPanel      = nullptr;
     ISurface*           Interfaces::m_pVGuiSurface    = nullptr;
     CInput*             Interfaces::m_pInput          = nullptr;
     IEngineTrace*       Interfaces::m_pEngineTrace    = nullptr;
     ICvar*              Interfaces::m_pCVar           = nullptr;
     IClientMode*        Interfaces::m_pClientMode     = nullptr;

     CreateInterfaceFn GetFactory( HMODULE hMod ) {
          return( CreateInterfaceFn )GetProcAddress( hMod, XorStr("CreateInterface"));
     }

     template<typename T>
     T* CaptureInterface( CreateInterfaceFn f, const char* szInterfaceVersion ) {
          return (T*)f( szInterfaceVersion, NULL );
     }

     IVEngineClient* Interfaces::Engine() {
          if(!m_pEngine) {
               CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
               m_pEngine = CaptureInterface<IVEngineClient>(pfnFactory, XorStr("VEngineClient013"));
          }
          return m_pEngine;
     }
     IBaseClientDLL* Interfaces::Client() {
          if(!m_pClient) {
               CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("client.dll")));
               m_pClient = CaptureInterface<IBaseClientDLL>(pfnFactory, XorStr("VClient017"));
          }
          return m_pClient;
     }
     IClientEntityList* Interfaces::EntityList() {
          if(!m_pEntityList) {
               CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("client.dll")));
               m_pEntityList = CaptureInterface<IClientEntityList>(pfnFactory, XorStr("VClientEntityList003"));
          }
          return m_pEntityList;
     }
     CGlobalVarsBase* Interfaces::GlobalVars() {
          if(!m_pGlobals) {
               auto uAddress = Utils::FindSignature(XorStr("client.dll"), XorStr("A1 ? ? ? ? 5F 8B 40 10"));
               uint32_t g_dwGlobalVarsBase = *(uint32_t*)(uAddress + 0x1);
               m_pGlobals = *(CGlobalVarsBase**)(g_dwGlobalVarsBase);

          }
          return m_pGlobals;
     }
     IPanel* Interfaces::VGUIPanel() {
          if(!m_pVGuiPanel) {
               CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("vgui2.dll")));
               m_pVGuiPanel = CaptureInterface<IPanel>(pfnFactory, XorStr("VGUI_Panel009"));
          }
          return m_pVGuiPanel;
     }
     ISurface* Interfaces::MatSurface() {
          if(!m_pVGuiSurface) {
               CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("vguimatsurface.dll")));
               m_pVGuiSurface = CaptureInterface<ISurface>(pfnFactory, XorStr("VGUI_Surface031"));
          }
          return m_pVGuiSurface;
     }
     CInput* Interfaces::Input() {
          if(!m_pInput) {
               auto pClientVFTable = *(uint32_t**)Client();
               m_pInput = *(CInput**)(pClientVFTable[15] + 0x1);
          }
          return m_pInput;
     }
     IEngineTrace* Interfaces::EngineTrace() {
          if(!m_pEngineTrace) {
               CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("engine.dll")));
               m_pEngineTrace = CaptureInterface<IEngineTrace>(pfnFactory, XorStr("EngineTraceClient004"));
          }
          return m_pEngineTrace;
     }
     ICvar* Interfaces::CVar() {
          if(!m_pCVar) {
               SourceEngine::CreateInterfaceFn pfnFactory = GetFactory(GetModuleHandleA(XorStr("vstdlib.dll")));
               m_pCVar = CaptureInterface<SourceEngine::ICvar>(pfnFactory, XorStr("VEngineCvar007"));
          }
          return m_pCVar;
     }
     IClientMode* Interfaces::ClientMode() {
          if(!m_pClientMode) {
               auto uAddress = *(DWORD*)(Utils::FindSignature(XorStr("client.dll"), XorStr("8B 35 ? ? ? ? 85 FF 74 73")) + 2);
               m_pClientMode = *(IClientMode**)(uAddress);
          }
          return m_pClientMode;
     }
}