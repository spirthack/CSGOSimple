#include "aimbot.h"
#include "../options.hpp"

void Aimbot::CreateMove(CUserCmd* cmd){
	Vector hitboxpos;
	Vector eyepos;
	QAngle aimvector;
	
	for (int i = 1; i <= g_EngineClient->GetMaxClients(); i++) {
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
		if (!weapon || !weapon->IsGun())
			continue;
		if (!g_LocalPlayer->CanSeePlayer(entity, hitboxpos))
			continue;
		eyepos = g_LocalPlayer->GetEyePos();
		aimvector = Math::CalcAngle(eyepos, hitboxpos);
		Math::Normalize3(aimvector);
		g_EngineClient->SetViewAngles(&aimvector);
	}
}
Aimbot g_Aimbot;
