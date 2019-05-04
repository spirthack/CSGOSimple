#include "aim.h"
#include "../options.hpp"

void Aimbot::CreateMove(CUserCmd* cmd){
	Vector hitboxpos;
	Vector eyepos;
	QAngle aimvector;
	
	for (int i = 1; i <= g_EntityList->GetHighestEntityIndex(); i++) {
		auto ent = C_BaseEntity::GetEntityByIndex(i);
		auto entity = (C_BasePlayer*)ent; 

		if (!entity || !entity->IsAlive())
			continue;
		if (entity == g_LocalPlayer || !entity->IsPlayer() || !g_Options.aim_enable)
			continue;
		if (entity->IsDormant() || entity->m_iTeamNum() == g_LocalPlayer->m_iTeamNum())
			continue;
		if (!entity->GetHitboxPos(HITBOX_HEAD, hitboxpos))
			continue;
		auto weapon = g_LocalPlayer->m_hActiveWeapon();
		if (!weapon)
			continue;
		if (!weapon->IsGun())
			continue;
      
		eyepos = g_LocalPlayer->GetEyePos();	
    
		CGameTrace tr;
		Ray_t ray;
		CTraceFilter filter;
		filter.pSkip = g_LocalPlayer;
		auto start = eyepos;
		ray.Init(start, hitboxpos);
		g_EngineTrace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);
		if (tr.hit_entity != entity)
			continue;
	
		aimvector = Math::CalcAngle(eyepos, hitboxpos);
		g_EngineClient->SetViewAngles(&aimvector);
	}
}
Aimbot g_Aimbot;
