#include "anims.hpp"

namespace Animations
{
	AnimationRecord AnimationRecord_t[65];

	bool FreshTick()
	{
		static int OldTickCount;

		if (OldTickCount != g_GlobalVars->tickcount)
		{
			OldTickCount = g_GlobalVars->tickcount;
			return true;
		}
		return false;
	}

	void AccurateLocalPlayer()
	{
		if (!g_LocalPlayer)
			return;

		if (!g_LocalPlayer->IsAlive())
			return;

		auto cmd = g_Input->GetUserCmd(g_Options.sequence_number);

		if (!cmd)
			return;

		static float ProperAbsYaw = g_LocalPlayer->GetPlayerAnimState()->m_flGoalFeetYaw;
		static std::array<float, 24U> SentPoseParameters = g_LocalPlayer->m_flPoseParameter();
		auto& Data = AnimationRecord_t[g_LocalPlayer->EntIndex()];

		if (FreshTick())
		{
			std::memcpy(Data.BackupLayers, g_LocalPlayer->GetAnimOverlays(), (sizeof(AnimationLayer) * 15));
			Data.OverrideAnimations = false;
			g_LocalPlayer->UpdateAnimationState(g_LocalPlayer->GetPlayerAnimState(), cmd->viewangles);

			if (g_Options.bSendPacket)
			{
				ProperAbsYaw = g_LocalPlayer->GetPlayerAnimState()->m_flGoalFeetYaw;
				SentPoseParameters = g_LocalPlayer->m_flPoseParameter();
			}
		}
		Data.OverrideAnimations = true;
		g_LocalPlayer->SetAbsAngles(QAngle(0, ProperAbsYaw, 0));
		g_LocalPlayer->GetPlayerAnimState()->m_flUnknownFraction = 0.f;
		std::memcpy(g_LocalPlayer->GetAnimOverlays(), Data.BackupLayers, (sizeof(AnimationLayer) * 15));
		g_LocalPlayer->m_flPoseParameter() = SentPoseParameters;
	}
}