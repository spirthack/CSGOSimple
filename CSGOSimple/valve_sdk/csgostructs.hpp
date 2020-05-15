#pragma once

#include "sdk.hpp"
#include <array>
#include "../helpers/utils.hpp"

class CCSPlayerAnimState
{
public:
	char pad_0000[120]; //0x0000
	float m_flEyeYaw; //0x0078
	float m_flPitch; //0x007C
	float m_flGoalFeetYaw; //0x0080
	float m_flCurrentFeetYaw; //0x0084
	char pad_0088[28]; //0x0088
	float m_flDuckAmount; //0x00A4
	char pad_00A8[8]; //0x00A8
	Vector m_vecOrigin; //0x00B0
	Vector m_vecLastOrigin; //0x00BC
	char pad_00C8[0x30]; //0x00C8
	float m_flSpeedFraction; //0x00F8
	float m_flSpeedFactor; //0x00FC
	char pad_0100[28]; //0x0100
	float m_flLandingRatio; //0x011C
	char pad_0120[528]; //0x0120
	float m_flMinBodyYawDegrees; //0x0330
	float m_flMaxBodyYawDegrees; //0x0334
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	void* pBaseEntity; //0x60
};

class CCSGOPlayerAnimState
{
public:
	void* pThis;
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
}; //Size=0x344

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

class C_BaseEntity : public IClientEntity
{
public:
	datamap_t* GetPredDescMap() 
	{
		typedef datamap_t* (__thiscall* o_GetPredDescMap)(void*);
		return CallVFunction<o_GetPredDescMap>(this, 17)(this);
	}
public:
	static __forceinline C_BaseEntity* GetEntityByIndex(int index) {
		return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
	}
	static __forceinline C_BaseEntity* get_entity_from_handle(CBaseHandle h) {
		return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntityFromHandle(h));
	}
public:
	NETVAR(int32_t, m_nModelIndex, "CBaseEntity", "m_nModelIndex");
	NETVAR(int32_t, m_iTeamNum, "CBaseEntity", "m_iTeamNum");
	NETVAR(Vector, m_vecOrigin, "CBaseEntity", "m_vecOrigin");
	NETVAR(Vector, m_vecAngles, "CBaseEntity", "m_vecAngles");
	NETVAR(bool, m_bShouldGlow, "CDynamicProp", "m_bShouldGlow");
	NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity, "CBaseEntity", "m_hOwnerEntity");
	NETVAR(bool, m_bSpotted, "CBaseEntity", "m_bSpotted");
	NETVAR(bool, m_bSpottedByMask, "CBaseEntity", "m_bSpottedByMask");
	NETVAR(float, m_flAnimTime, "CBaseEntity", "m_flAnimTime");
	NETVAR_OFFSET(matrix3x4_t&, m_rgflCoordinateFrame, "CBaseEntity", "m_CollisionGroup", -0x30);
	NETVAR_OFFSET(int, m_bIsAutoaimTarget, "CBaseEntity", "m_bIsAutoaimTarget", +0x4);

	NETVAR(Vector, m_vecMins, "CBaseEntity", "m_vecMins");
	NETVAR(Vector, m_vecMaxs, "CBaseEntity", "m_vecMaxs");

	bool IsPlayer();
	bool IsWeapon();
	bool IsPlantedC4();
	bool IsDefuseKit();
	bool IsLoot();
};

class C_PlantedC4 : public C_BaseEntity
{
public:
	NETVAR(bool, m_bBombTicking, "CPlantedC4", "m_bBombTicking");
	NETVAR(bool, m_bBombDefused, "CPlantedC4", "m_bBombDefused");
	NETVAR(float, m_flC4Blow, "CPlantedC4", "m_flC4Blow");
	NETVAR(float, m_flTimerLength, "CPlantedC4", "m_flTimerLength");
	NETVAR(float, m_flDefuseLength, "CPlantedC4", "m_flDefuseLength");
	NETVAR(float, m_flDefuseCountDown, "CPlantedC4", "m_flDefuseCountDown");
	NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser, "CPlantedC4", "m_hBombDefuser");
	NETVAR(int, m_nBombSite, "CPlantedC4", "m_nBombSite");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	NETVAR(uint64_t, m_OriginalOwnerXuid, "CBaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidLow, "CBaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidHigh, "CBaseAttributableItem", "m_OriginalOwnerXuidHigh");
	NETVAR(int32_t, m_nFallbackStatTrak, "CBaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(int32_t, m_nFallbackPaintKit, "CBaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int32_t, m_nFallbackSeed, "CBaseAttributableItem", "m_nFallbackSeed");
	NETVAR(float_t, m_flFallbackWear, "CBaseAttributableItem", "m_flFallbackWear");

	NETVAR(int32_t, m_bInitialized, "CBaseAttributableItem", "m_bInitialized");
	NETVAR(short, m_iItemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int32_t, m_iEntityLevel, "CBaseAttributableItem", "m_iEntityLevel");
	NETVAR(int32_t, m_iAccountID, "CBaseAttributableItem", "m_iAccountID");
	NETVAR(int32_t, m_iItemIDLow, "CBaseAttributableItem", "m_iItemIDLow");
	NETVAR(int32_t, m_iItemIDHigh, "CBaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int32_t, m_iEntityQuality, "CBaseAttributableItem", "m_iEntityQuality");
	NETVAR(char[32], m_szCustomName, "CBaseAttributableItem", "m_szCustomName");
};

class C_BaseWeaponWorldModel : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "CBaseWeaponWorldModel", "m_nModelIndex");
};

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
	NETVAR(float_t, m_flNextPrimaryAttack, "CBaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(float_t, m_flNextSecondaryAttack, "CBaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(int32_t, m_iClip1, "CBaseCombatWeapon", "m_iClip1");
	NETVAR(int32_t, m_iPrimaryReserveAmmoCount, "CBaseCombatWeapon", "m_iPrimaryReserveAmmoCount");
	NETVAR(int32_t, m_iClip2, "CBaseCombatWeapon", "m_iClip2");
	NETVAR(float_t, m_flRecoilIndex, "CWeaponCSBase", "m_flRecoilIndex");
	NETVAR(int32_t, m_iViewModelIndex, "CBaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(int32_t, m_iWorldModelIndex, "CBaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int32_t, m_iWorldDroppedModelIndex, "CBaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(bool, m_bPinPulled, "CBaseCSGrenade", "m_bPinPulled");
	NETVAR(float_t, m_fThrowTime, "CBaseCSGrenade", "m_fThrowTime");
	NETVAR(float_t, m_flThrowStrength, "CBaseCSGrenade", "m_flThrowStrength");
	NETVAR(float_t, m_flPostponeFireReadyTime, "CBaseCombatWeapon", "m_flPostponeFireReadyTime");
	NETVAR(CHandle<C_BaseWeaponWorldModel>, m_hWeaponWorldModel, "CBaseCombatWeapon", "m_hWeaponWorldModel");
	NETVAR(int32_t, m_zoomLevel, "CWeaponCSBaseGun", "m_zoomLevel");
public:
	CCSWeaponInfo* GetCSWeaponData();
	bool HasBullets();
	bool CanFire();
	bool IsGrenade();
	bool IsKnife();
	bool IsReloading();
	bool IsRifle();
	bool IsPistol();
	bool IsSniper();
	bool IsGun();

	float GetInaccuracy();
	float GetSpread();
	void UpdateAccuracyPenalty();
};

class C_BasePlayer : public C_BaseEntity
{
public:
	static __forceinline C_BasePlayer* GetPlayerByUserId(int id)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(g_EngineClient->GetPlayerForUserID(id)));
	}
	static __forceinline C_BasePlayer* GetPlayerByIndex(int i)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(i));
	}
public:
	NETVAR(bool, m_bHasDefuser, "CCSPlayer", "m_bHasDefuser");
	NETVAR(bool, m_bGunGameImmunity, "CCSPlayer", "m_bGunGameImmunity");
	NETVAR(int32_t, m_iShotsFired, "CCSPlayer", "m_iShotsFired");
	NETVAR(QAngle, m_angEyeAngles, "CCSPlayer", "m_angEyeAngles[0]");
	NETVAR(int, m_ArmorValue, "CCSPlayer", "m_ArmorValue");
	NETVAR(bool, m_bHasHeavyArmor, "CCSPlayer", "m_bHasHeavyArmor");
	NETVAR(bool, m_bHasHelmet, "CCSPlayer", "m_bHasHelmet");
	NETVAR(bool, m_bIsScoped, "CCSPlayer", "m_bIsScoped");
	NETVAR(bool, m_bIsDefusing, "CCSPlayer", "m_bIsDefusing");
	NETVAR(float, m_flLowerBodyYawTarget, "CCSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(int32_t, m_iHealth, "CBasePlayer", "m_iHealth");
	NETVAR(int32_t, m_lifeState, "CBasePlayer", "m_lifeState");
	NETVAR(int32_t, m_fFlags, "CBasePlayer", "m_fFlags");
	NETVAR(int32_t, m_nTickBase, "CBasePlayer", "m_nTickBase");
	NETVAR(Vector, m_vecViewOffset, "CBasePlayer", "m_vecViewOffset[0]");
	NETVAR(QAngle, m_viewPunchAngle, "CBasePlayer", "m_viewPunchAngle");
	NETVAR(QAngle, m_aimPunchAngle, "CBasePlayer", "m_aimPunchAngle");
	NETVAR(CHandle<C_BaseViewModel>, m_hViewModel, "CBasePlayer", "m_hViewModel[0]");
	NETVAR(Vector, m_vecVelocity, "CBasePlayer", "m_vecVelocity[0]");
	NETVAR(float, m_flMaxspeed, "CBasePlayer", "m_flMaxspeed");
	NETVAR(int32_t, m_iObserverMode, "CBasePlayer", "m_iObserverMode");
	NETVAR(CHandle<C_BasePlayer>, m_hObserverTarget, "CBasePlayer", "m_hObserverTarget");
	NETVAR(float, m_flFlashMaxAlpha, "CCSPlayer", "m_flFlashMaxAlpha");
	NETVAR_OFFSET(float, m_flFlashMaxAlphaOffset, "CCSPlayer", "m_flFlashMaxAlpha", -0x8);
	NETVAR(int32_t, m_nHitboxSet, "CBaseAnimating", "m_nHitboxSet");
	NETVAR(int32_t, m_nForceBone, "CBaseAnimating", "m_nForceBone");
	NETVAR(bool, m_bClientSideAnimation, "CBaseAnimating", "m_bClientSideAnimation");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hActiveWeapon, "CBaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(int32_t, m_iAccount, "CCSPlayer", "m_iAccount");
	NETVAR(float, m_flFlashDuration, "CCSPlayer", "m_flFlashDuration");
	NETVAR(float, m_flSimulationTime, "CBaseEntity", "m_flSimulationTime");
	NETVAR(float, m_flCycle, "CServerAnimationData", "m_flCycle");
	NETVAR(int, m_nSequence, "CBaseViewModel", "m_nSequence");
	PNETVAR(char, m_szLastPlaceName, "CBasePlayer", "m_szLastPlaceName");
	NETPROP(m_aimPunchAngleProp, "CBasePlayer", "m_aimPunchAngle");
	NETPROP(m_viewPunchAngleProp, "CBasePlayer", "m_viewPunchAngle");
	NETPROP(m_flLowerBodyYawTargetProp, "CCSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(QAngle, m_angAbsAngles, "CBaseEntity", "m_angAbsAngles");
	NETVAR(Vector, m_angAbsOrigin, "CBaseEntity", "m_angAbsOrigin");
	NETVAR(float, m_flDuckSpeed, "CBaseEntity", "m_flDuckSpeed");
	NETVAR(float, m_flDuckAmount, "CBaseEntity", "m_flDuckAmount");
	NETVAR(int, m_nSurvivalTeam, "CCSPlayer", "m_nSurvivalTeam");
	NETVAR(int, m_iMoney, "CCSPlayer", "m_iAccount");
	NETVAR(int32_t, m_iFOV, "CBasePlayer", "m_iFOV");
	NETVAR(int32_t, m_iDefaultFOV, "CBasePlayer", "m_iDefaultFOV");
	PNETVAR(CHandle<C_BaseCombatWeapon>, m_hMyWeapons, "CBaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(CBaseHandle, m_hMyWearables, "CBaseCombatCharacter", "m_hMyWearables");
	NETVAR_OFFSET(QAngle*, GetVAngles, "CBasePlayer", "deadflag", +0x4);
	NETVAR(float, m_flNextAttack, "CBaseCombatCharacter", "m_flNextAttack");
public:
	CUserCmd*&	  m_pCurrentCommand();
	Vector        GetEyePos();
	player_info_t GetPlayerInfo();
	bool          IsAlive();
	bool		  IsEnemy();
	bool		  IsFlashed();
	bool          HasC4();
	Vector        GetHitboxPos(int hitbox_id);
	mstudiobbox_t* GetHitbox(int hitbox_id);
	bool          GetHitboxPos(int hitbox, Vector& output);
	Vector        GetBonePos(int bone);
	bool          CanSeePlayer(C_BasePlayer* player, int hitbox);
	bool          CanSeePlayer(C_BasePlayer* player, const Vector& pos);
	int			  m_nMoveType();
};
	

class C_BaseViewModel : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
	NETPROP(m_nSequence, "DT_BaseViewModel", "m_nSequence");
	void SendViewModelMatchingSequence(int sequence);
};