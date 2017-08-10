#pragma once

#include "sdk.hpp"

#define NETVAR(type, name, table, netvar)                           \
    type& name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return *(type*)((uintptr_t)this + _##name);                 \
    }

#define PNETVAR(type, name, table, netvar)                           \
    type* name##() const {                                          \
        static int _##name = NetvarSys::Get().GetOffset(table, netvar);     \
        return (type*)((uintptr_t)this + _##name);                 \
    }

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

class C_BaseEntity;

// Created with ReClass.NET by KN4CK3R
class CHudTexture
{
public:
    char szShortName[64];    //0x0000
    char szTextureFile[64];  //0x0040
    bool bRenderUsingFont;   //0x0080
    bool bPrecached;         //0x0081
    int8_t cCharacterInFont; //0x0082
    uint8_t pad_0083[1];     //0x0083
    uint32_t hFont;          //0x0084
    int32_t iTextureId;      //0x0088
    float afTexCoords[4];    //0x008C
    uint8_t pad_009C[16];    //0x009C
};

#pragma pack(push, 1)
class CCSWeaponInfo
{
public:
    void*         vftbl;
    bool          bParsedScript;
    bool          bLoadedHudElements;
    char          szClassName[80];
    char          szPrintName[80];
    char          szViewModel[80];
    char          szWorldModel[80];
    char          szBulletType[32];
    char          szWorldDroppedModel[80];
    char          szAnimationPrefix[16];
    uint8_t       pad_01C6[2];
    int32_t       iSlot;
    int32_t       iPosition;
    int32_t       iMaxClip1;
    int32_t       iMaxClip2;
    int32_t       iDefaultClip1;
    int32_t       iDefaultClip2;
    int32_t       iWeight;
    int32_t       iRumbleEffect;
    bool          bAutoSwitchTo;
    bool          bAutoSwitchFrom;
    uint8_t       pad_01EA[2];
    uint32_t      iFlags;
    char          szAmmo1[16];
    char          szAmmo2[16];
    char          szAIAddOn[80];
    char          aShootSounds[18][80];
    int           iAmmoType;
    int           iAmmo2Type;
    bool          bMeleeWeapon;
    bool          bBuiltRightHanded;
    bool          bAllowFlipping;
    BYTE          pad_07BB;
    int           iSpriteCount;
    CHudTexture*  iconWeaponS;
    CHudTexture*  iconWeapon;
    CHudTexture*  iconAmmo;
    CHudTexture*  iconAmmo2;
    CHudTexture*  iconCrosshair;
    CHudTexture*  iconAutoaim;
    CHudTexture*  iconZoomedCrosshair;
    CHudTexture*  iconZoomedAutoaim;
    uint8_t       pad_07E0[8];
    CSWeaponType  m_WeaponType;
    int32_t       m_iTeam;
    int32_t       iWeaponID;
    bool          bFullAuto;
    uint8_t       pad_07F5[7];
    int32_t       m_iWeaponPrice;
    float         m_flArmorRatio;
    float         m_flMaxSpeed;
    float         m_flMaxSpeedAlt;
    int32_t       m_iCrosshairMinDistance;
    int32_t       m_iCrosshairDeltaDistance;
    float         m_flPenetration;
    int32_t       m_iDamage;
    float         m_flRange;
    float         m_flRangeModifier;
    int32_t       m_iBullets;
}; //Size: 0x0828
#pragma pack(pop)

class C_EconItemView
{
public:
    NETVAR(int32_t, m_bInitialized,          "DT_BaseAttributableItem", "m_bInitialized");
    NETVAR(int32_t, m_iItemDefinitionIndex,  "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
    NETVAR(int32_t, m_iEntityLevel,          "DT_BaseAttributableItem", "m_iEntityLevel");
    NETVAR(int32_t, m_iAccountID,            "DT_BaseAttributableItem", "m_iAccountID");
    NETVAR(int32_t, m_iItemIDLow,            "DT_BaseAttributableItem", "m_iItemIDLow");
    NETVAR(int32_t, m_iItemIDHigh,           "DT_BaseAttributableItem", "m_iItemIDHigh");
    NETVAR(int32_t, m_iEntityQuality,        "DT_BaseAttributableItem", "m_iEntityQuality");
};

class C_BaseEntity : public IClientEntity
{
public:
    static __forceinline C_BaseEntity* GetEntityByIndex(int index)
    {
        return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(index));
    }
    static __forceinline C_BaseEntity* get_entity_from_handle(CBaseHandle h)
    {
        return static_cast<C_BaseEntity*>(g_EntityList->GetClientEntityFromHandle(h));
    }

    NETVAR(int32_t,               m_nModelIndex,    "DT_BaseEntity",  "m_nModelIndex");
    NETVAR(int32_t,               m_iTeamNum,       "DT_BaseEntity",  "m_iTeamNum");
    NETVAR(Vector,                m_vecOrigin,      "DT_BaseEntity",  "m_vecOrigin");
    NETVAR(bool,                  m_bShouldGlow,    "DT_DynamicProp", "m_bShouldGlow");
    NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity,   "DT_BaseEntity",  "m_hOwnerEntity");
    
    const matrix3x4_t& m_rgflCoordinateFrame()
    {
        static auto _m_rgflCoordinateFrame = NetvarSys::Get().GetOffset("DT_BaseEntity", "m_CollisionGroup") - 0x30;
        return *(matrix3x4_t*)((uintptr_t)this + _m_rgflCoordinateFrame);
    }

    bool IsPlayer();
    bool IsWeapon();
    bool IsPlantedC4();
    bool IsDefuseKit();
};

class C_PlantedC4
{
public:
    NETVAR(bool,                  m_bBombTicking,      "DT_PlantedC4", "m_bBombTicking");
    NETVAR(bool,                  m_bBombDefused,      "DT_PlantedC4", "m_bBombDefused");
    NETVAR(float,                 m_flC4Blow,          "DT_PlantedC4", "m_flC4Blow");
    NETVAR(float,                 m_flTimerLength,     "DT_PlantedC4", "m_flTimerLength");
    NETVAR(float,                 m_flDefuseLength,    "DT_PlantedC4", "m_flDefuseLength");
    NETVAR(float,                 m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");
    NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser,      "DT_PlantedC4", "m_hBombDefuser");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
    NETVAR(uint64_t, m_OriginalOwnerXuid,    "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidLow,  "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
    NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
    NETVAR(int32_t, m_nFallbackStatTrak,     "DT_BaseAttributableItem", "m_nFallbackStatTrak");
    NETVAR(int32_t, m_nFallbackPaintKit,     "DT_BaseAttributableItem", "m_nFallbackPaintKit");
    NETVAR(int32_t, m_nFallbackSeed,         "DT_BaseAttributableItem", "m_nFallbackSeed");
    NETVAR(float_t, m_flFallbackWear,        "DT_BaseAttributableItem", "m_flFallbackWear");

    C_EconItemView& m_Item()
    {
        // Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
        // A real fix for this requires a rewrite of the netvar manager
        return *(C_EconItemView*)this;
    }
};


class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
    NETVAR(float_t, m_flNextPrimaryAttack,   "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
    NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
    NETVAR(int32_t, m_iClip1,                "DT_BaseCombatWeapon", "m_iClip1");
    NETVAR(int32_t, m_iClip2,                "DT_BaseCombatWeapon", "m_iClip2");
    NETVAR(float_t, m_flRecoilIndex,         "DT_WeaponCSBase", "m_flRecoilIndex");

    CCSWeaponInfo* GetCSWeaponData();
    bool HasBullets();
    bool CanFire();
    bool IsGrenade();
    bool IsKnife();
    bool IsReloading();
	bool IsRifle();
	bool IsPistol();
	bool IsSniper();
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

    NETVAR(bool,                             m_bHasDefuser,          "DT_CSPlayer", "m_bHasDefuser");
    NETVAR(bool,                             m_bGunGameImmunity,     "DT_CSPlayer", "m_bGunGameImmunity");
    NETVAR(int32_t,                          m_iShotsFired,          "DT_CSPlayer", "m_iShotsFired");
    NETVAR(QAngle,                           m_angEyeAngles,         "DT_CSPlayer", "m_angEyeAngles[0]");
    NETVAR(int,                              m_ArmorValue,           "DT_CSPlayer", "m_ArmorValue");
    NETVAR(bool,                             m_bHasHelmet,           "DT_CSPlayer", "m_bHasHelmet");
    NETVAR(bool,                             m_bIsScoped,            "DT_CSPlayer", "m_bIsScoped");;
    NETVAR(float,                            m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
    NETVAR(int32_t,                          m_iHealth,              "DT_BasePlayer", "m_iHealth");
    NETVAR(int32_t,                          m_lifeState,            "DT_BasePlayer", "m_lifeState");
    NETVAR(int32_t,                          m_fFlags,               "DT_BasePlayer", "m_fFlags");
    NETVAR(int32_t,                          m_nTickBase,            "DT_BasePlayer", "m_nTickBase");
    NETVAR(Vector,                           m_vecViewOffset,        "DT_BasePlayer", "m_vecViewOffset[0]");
    NETVAR(QAngle,                           m_viewPunchAngle,       "DT_BasePlayer", "m_viewPunchAngle");
    NETVAR(QAngle,                           m_aimPunchAngle,        "DT_BasePlayer", "m_aimPunchAngle");
    NETVAR(CHandle<C_BaseViewModel>,         m_hViewModel,           "DT_BasePlayer", "m_hViewModel[0]");
    NETVAR(Vector,                           m_vecVelocity,          "DT_BasePlayer", "m_vecVelocity[0]");
    NETVAR(float,                            m_flMaxspeed,           "DT_BasePlayer", "m_flMaxspeed");
    NETVAR(CHandle<C_BasePlayer>,            m_hObserverTarget,      "DT_BasePlayer", "m_hObserverTarget");
    NETVAR(CHandle<C_BaseCombatWeapon>,      m_hActiveWeapon,        "DT_BaseCombatCharacter", "m_hActiveWeapon");
    PNETVAR(CHandle<C_BaseCombatWeapon>,     m_hMyWeapons,           "DT_BaseCombatCharacter", "m_hMyWeapons");
    PNETVAR(CHandle<C_BaseAttributableItem>, m_hMyWearables,         "DT_BaseCombatCharacter", "m_hMyWearables");
    
    CUserCmd*& m_pCurrentCommand();

    Vector        GetEyePos();
    player_info_t GetPlayerInfo();
    bool          IsAlive();
    bool          HasC4();
    Vector        GetHitboxPos(int hitbox_id);
    bool          GetHitboxPos(int hitbox, Vector &output);
    Vector        GetBonePos(int bone);
    bool          CanSeePlayer(C_BasePlayer* player, int hitbox);
    bool          CanSeePlayer(C_BasePlayer* player, const Vector& pos);
};

class C_BaseViewModel : public C_BaseEntity
{
public:
    NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
    NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
    NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
    NETVAR(CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
};
