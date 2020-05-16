#pragma once

#include "valve_sdk/csgostructs.hpp"
#include "helpers/vfunc_hook.hpp"
#include <d3d9.h>

namespace index
{
	constexpr auto EmitSound1               = 5;
	constexpr auto EmitSound2               = 6;
    constexpr auto Present                  = 17;
    constexpr auto Reset                    = 16;
    constexpr auto PaintTraverse            = 41;
    constexpr auto CreateMove               = 22;
    constexpr auto PlaySound                = 82;
    constexpr auto FrameStageNotify         = 37;
    constexpr auto DrawModelExecute         = 21;
    constexpr auto DoPostScreenSpaceEffects = 44;
	constexpr auto SvCheatsGetBool          = 13;
	constexpr auto OverrideView             = 18;
	constexpr auto LockCursor               = 67;
}

namespace Hooks
{
    void Initialize();
    void Shutdown();

    inline	std::shared_ptr<hook_t> hlclient_hook;
	inline  std::shared_ptr<hook_t> direct3d_hook;
	inline  std::shared_ptr<hook_t> vguipanel_hook;
	inline  std::shared_ptr<hook_t> vguisurf_hook;
	inline  std::shared_ptr<hook_t> mdlrender_hook;
	inline  std::shared_ptr<hook_t> viewrender_hook;
	inline  std::shared_ptr<hook_t> sound_hook;
	inline  std::shared_ptr<hook_t> clientmode_hook;
	inline  std::shared_ptr<hook_t> sv_cheats;


    long __stdcall hkPresent(IDirect3DDevice9* device, RECT* src_rect, RECT* dest_rect, HWND dest_wnd_override, RGNDATA* dirty_region);
    long __stdcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters);
    void __stdcall hkCreateMove(int sequence_number, float input_sample_frametime, bool active, bool& bSendPacket);
	void __fastcall hkCreateMove_Proxy(void* _this, int, int sequence_number, float input_sample_frametime, bool active);
	void __fastcall hkPaintTraverse(void* _this, int edx, vgui::VPANEL panel, bool forceRepaint, bool allowForce);
	void __fastcall hkEmitSound1(void* _this, int, IRecipientFilter & filter, int iEntIndex, int iChannel, const char * pSoundEntry, unsigned int nSoundEntryHash, const char * pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector * pOrigin, const Vector * pDirection, void * pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);
    void __fastcall hkDrawModelExecute(void* _this, int, IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pInfo, matrix3x4_t* pCustomBoneToWorld);
    void __fastcall hkFrameStageNotify(void* _this, int, ClientFrameStage_t stage);
	void __fastcall hkOverrideView(void* _this, int, CViewSetup * vsView);
	void __fastcall hkLockCursor(void* _this);
    int  __fastcall hkDoPostScreenEffects(void* _this, int, int a1);
}
