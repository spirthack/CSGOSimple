#include "Hooks.hpp"

#include "Menu.hpp"
#include "Options.hpp"
#include "helpers/InputSys.hpp"
#include "helpers/Utils.hpp"
#include "features/bhop.hpp"
#include "features/Chams.hpp"
#include "features/Visuals.hpp"

namespace Hooks
{
    vfunc_hook hlclient_hook;
    vfunc_hook direct3d_hook;
    vfunc_hook vguipanel_hook;
    vfunc_hook vguisurf_hook;
    vfunc_hook mdlrender_hook;
    vfunc_hook viewrender_hook;

    void Initialize()
    {
        hlclient_hook .setup(g_CHLClient);
        direct3d_hook .setup(g_D3DDevice9);
        vguipanel_hook.setup(g_VGuiPanel);
        vguisurf_hook .setup(g_VGuiSurface);
        mdlrender_hook.setup(g_MdlRender);
        viewrender_hook.setup(g_ViewRender);

        direct3d_hook.hook_index(index::EndScene, hkEndScene);
        direct3d_hook.hook_index(index::Reset, hkReset);

        hlclient_hook.hook_index(index::FrameStageNotify, hkFrameStageNotify);
        hlclient_hook.hook_index(index::CreateMove, hkCreateMove_Proxy);

        vguipanel_hook.hook_index(index::PaintTraverse, hkPaintTraverse);

        vguisurf_hook.hook_index(index::PlaySound, hkPlaySound);

        mdlrender_hook.hook_index(index::DrawModelExecute, hkDrawModelExecute);

        viewrender_hook.hook_index(index::RenderView, hkRenderView);

        Visuals::CreateFonts();
    }
    //--------------------------------------------------------------------------------
    void Shutdown()
    {
        hlclient_hook.unhook_all();
        direct3d_hook.unhook_all();
        vguipanel_hook.unhook_all();
        vguisurf_hook.unhook_all();
        mdlrender_hook.unhook_all();
        Visuals::DestroyFonts();
    }
    //--------------------------------------------------------------------------------
    long __stdcall hkEndScene(IDirect3DDevice9* device)
    {
        auto oEndScene = direct3d_hook.get_original<EndScene>(index::EndScene);

        static auto viewmodel_fov       = g_CVar->FindVar("viewmodel_fov");
        static auto mat_ambient_light_r = g_CVar->FindVar("mat_ambient_light_r");
        static auto mat_ambient_light_g = g_CVar->FindVar("mat_ambient_light_g");
        static auto mat_ambient_light_b = g_CVar->FindVar("mat_ambient_light_b");

        viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
        viewmodel_fov->SetValue(g_Options.viewmodel_fov);
        mat_ambient_light_r->SetValue(g_Options.mat_ambient_light_r);
        mat_ambient_light_g->SetValue(g_Options.mat_ambient_light_g);
        mat_ambient_light_b->SetValue(g_Options.mat_ambient_light_b);

        Menu::Get().Render();

        if(InputSys::Get().IsKeyDown(VK_TAB))
            Utils::RankRevealAll();
        
        return oEndScene(device);
    }
    //--------------------------------------------------------------------------------
    long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters)
    {
        auto oReset = direct3d_hook.get_original<Reset>(index::Reset);

        Visuals::DestroyFonts();
        Menu::Get().OnDeviceLost();

        auto hr = oReset(device, pPresentationParameters);

        if(hr >= 0) {
            Menu::Get().OnDeviceReset();
            Visuals::CreateFonts();
        }

        return hr;
    }
    //--------------------------------------------------------------------------------
    void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket)
    {
        auto oCreateMove = hlclient_hook.get_original<CreateMove>(index::CreateMove);

        oCreateMove(g_CHLClient, sequence_number, input_sample_frametime, active);

        auto cmd      = g_Input->GetUserCmd(sequence_number);
        auto verified = g_Input->GetVerifiedCmd(sequence_number);

        if(!cmd || !cmd->command_number)
            return;

        if(g_Options.misc_bhop) {
            BunnyHop::OnCreateMove(cmd);
        }

        verified->m_cmd = *cmd;
        verified->m_crc = cmd->GetChecksum();
    }
    //--------------------------------------------------------------------------------
    __declspec(naked) void __stdcall hkCreateMove_Proxy(int sequence_number, float input_sample_frametime, bool active)
    {
        __asm
        {
            push ebp
            mov  ebp, esp
            push ebx
            lea  ecx, [esp]
            push ecx
            push dword ptr[active]                
            push dword ptr[input_sample_frametime]
            push dword ptr[sequence_number]       
            call Hooks::hkCreateMove
            pop  ebx
            pop  ebp
            retn 0Ch
        }
    }
    //--------------------------------------------------------------------------------
    void __stdcall hkPaintTraverse(vgui::VPANEL panel, bool forceRepaint, bool allowForce)
    {
        static auto panelId = vgui::VPANEL{ 0 };
        static auto oPaintTraverse = vguipanel_hook.get_original<PaintTraverse>(index::PaintTraverse);

        oPaintTraverse(g_VGuiPanel, panel, forceRepaint, allowForce);

        if(!panelId) {
            const auto panelName = g_VGuiPanel->GetName(panel);
            if(!strcmp(panelName, "FocusOverlayPanel")) {
                panelId = panel;
            }
        } else if(panelId == panel) {
            if(g_EngineClient->IsInGame() && !g_EngineClient->IsTakingScreenshot()) {

                if(!g_LocalPlayer)
                    return;

                for(auto i = 1; i <= g_EntityList->GetMaxEntities(); ++i) {
                    auto entity = (C_BaseEntity*)C_BaseEntity::GetEntityByIndex(i);

                    if(!entity)
                        continue;

                    if(entity == g_LocalPlayer)
                        continue;

                    if(i < 65 && g_Options.esp_enabled) {
                        auto player = (C_BasePlayer*)entity;
                        if(!entity->IsDormant() && player->IsAlive() && Visuals::player::begin(player)) {
                            if(g_Options.esp_player_snaplines) Visuals::player::RenderSnapline();
                            if(g_Options.esp_player_boxes)     Visuals::player::RenderBox();
                            if(g_Options.esp_player_weapons)   Visuals::player::RenderWeapon();
                            if(g_Options.esp_player_names)     Visuals::player::RenderName();
                            if(g_Options.esp_player_health)    Visuals::player::RenderHealth();
                        }
                    } else if(g_Options.esp_dropped_weapons && entity->IsWeapon()) {
                        Visuals::Misc::RenderWeapon((C_BaseCombatWeapon*)entity);
                    } else if(g_Options.esp_defuse_kit && entity->IsDefuseKit()) {
                        Visuals::Misc::RenderDefuseKit(entity);
                    } else if(entity->IsPlantedC4()) {
                        if(g_Options.esp_planted_c4)
                            Visuals::Misc::RenderPlantedC4(entity);
                    }
                }

                if(g_Options.esp_crosshair)
                    Visuals::Misc::RenderCrosshair();
            }
        }
    }
    //--------------------------------------------------------------------------------
    void __stdcall hkPlaySound(const char* name)
    {
        static auto oPlaySound = vguisurf_hook.get_original<PlaySound>(index::PlaySound);

        oPlaySound(g_VGuiSurface, name);

        //if(strstr(name, "competitive_accept_beep.wav")) {
        //    static auto fnAccept =
        //        (void(*)())Utils::PatternScan(GetModuleHandleA("client.dll"), "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE");
        //
        //    fnAccept();
        //}
    }
    //--------------------------------------------------------------------------------
    void __stdcall hkRenderView(const CViewSetup& view, CViewSetup& a3, int a4, int a5)
    {
        static auto ofunc = viewrender_hook.get_original<RenderView>(index::RenderView);
        
        if(g_LocalPlayer && g_Options.glow_enabled) {
            for(auto i = 0; i < g_GlowObjManager->m_GlowObjectDefinitions.Count(); i++) {
                auto& glowObject = g_GlowObjManager->m_GlowObjectDefinitions[i];
                auto entity = (C_BaseEntity*)glowObject.m_pEntity;

                if(glowObject.IsUnused())
                    continue;

                if(!entity || entity->IsDormant())
                    continue;

                auto class_id = entity->GetClientClass()->m_ClassID;
                auto color = Color{};

                switch(class_id) {
                case ClassId_CCSPlayer:
                {
                    auto is_enemy = entity->m_iTeamNum() != g_LocalPlayer->m_iTeamNum();

                    if(((C_BasePlayer*)entity)->HasC4() && is_enemy && g_Options.glow_c4_carrier) {
                        color = g_Options.color_glow_c4_carrier;
                        break;
                    }

                    if(!g_Options.glow_players || !((C_BasePlayer*)entity)->IsAlive())
                        continue;

                    if(!is_enemy && g_Options.glow_enemies_only)
                        continue;

                    color = is_enemy ? g_Options.color_glow_enemy : g_Options.color_glow_ally;

                    break;
                }
                case ClassId_CChicken:
                    if(!g_Options.glow_chickens)
                        continue;
                    entity->m_bShouldGlow() = true;
                    color = g_Options.color_glow_chickens;
                    break;
                case ClassId_CBaseAnimating:
                    if(!g_Options.glow_defuse_kits)
                        continue;
                    color = g_Options.color_glow_defuse;
                    break;
                case ClassId_CPlantedC4:
                    if(!g_Options.glow_planted_c4)
                        continue;
                    color = g_Options.color_glow_planted_c4;
                    break;
                default:
                {
                    if(entity->IsWeapon()) {
                        if(!g_Options.glow_weapons)
                            continue;
                        color = g_Options.color_glow_weapons;
                    }
                }
                }

                glowObject.m_flRed   = color.r() / 255.0f;
                glowObject.m_flGreen = color.g() / 255.0f;
                glowObject.m_flBlue  = color.b() / 255.0f;
                glowObject.m_flAlpha = color.a() / 255.0f;
                glowObject.m_bRenderWhenOccluded   = true;
                glowObject.m_bRenderWhenUnoccluded = false;
            }
        }

        return ofunc(g_ViewRender, view, a3, a4, a5);
    }
    //--------------------------------------------------------------------------------
    void __stdcall hkFrameStageNotify(ClientFrameStage_t stage)
    {
        static auto ofunc = hlclient_hook.get_original<FrameStageNotify>(index::FrameStageNotify);

        ofunc(g_CHLClient, stage);
    }
    //--------------------------------------------------------------------------------
    void __stdcall hkDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld)
    {
        static auto ofunc = mdlrender_hook.get_original<DrawModelExecute>(index::DrawModelExecute);

        Chams::Get().OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

        ofunc(g_MdlRender, ctx, state, pInfo, pCustomBoneToWorld);

        g_MdlRender->ForcedMaterialOverride(nullptr);
    }
}
