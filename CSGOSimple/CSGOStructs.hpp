#pragma once

#include <Windows.h>
#include "SourceEngine/SDK.hpp"
#include "NetvarManager.hpp"
#include "XorStr.hpp"

class C_CSPlayer;

class C_BaseCombatWeapon : public SourceEngine::IClientEntity {
     template<class T>
     inline T GetFieldValue(int offset) {
          return *(T*)((DWORD)this + offset);
     }

public:
     C_CSPlayer* GetOwner() {
          using namespace SourceEngine;
          static int m_hOwnerEntity = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_hOwnerEntity"));
          return (C_CSPlayer*)Interfaces::EntityList()->GetClientEntityFromHandle(GetFieldValue<CHandle<C_CSPlayer>>(m_hOwnerEntity));
     }
     float NextPrimaryAttack() {
          static int m_flNextPrimaryAttack = GET_NETVAR(XorStr("DT_BaseCombatWeapon"), XorStr("LocalActiveWeaponData"), XorStr("m_flNextPrimaryAttack"));
          return GetFieldValue<float>(m_flNextPrimaryAttack);
     }
     int GetId() {
          typedef int(__thiscall* tGetId)(void*);
          return SourceEngine::CallVFunction<tGetId>(this, 458)(this);
     }
     const char* GetName() {
          typedef const char* (__thiscall* tGetName)(void*);
          return SourceEngine::CallVFunction<tGetName>(this, 378)(this);
     }
};


class C_CSPlayer : public SourceEngine::IClientEntity {
     template<class T>
     inline T GetFieldValue(int offset) {
          return *(T*)((DWORD)this + offset);
     }

     template<class T>
     T* GetFieldPointer(int offset) {
          return (T*)((DWORD)this + offset);
     }
public:
     static C_CSPlayer* GetLocalPlayer() {
          return (C_CSPlayer*)SourceEngine::Interfaces::EntityList()->GetClientEntity(SourceEngine::Interfaces::Engine()->GetLocalPlayer());
     }
     C_BaseCombatWeapon* GetActiveWeapon() {
          using namespace SourceEngine;
          static int m_hActiveWeapon = GET_NETVAR(XorStr("DT_BaseCombatCharacter"), XorStr("m_hActiveWeapon"));
          return (C_BaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(GetFieldValue<CHandle<IClientEntity>>(m_hActiveWeapon));
     }
     int GetHealth() {
          static int m_iHealth = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_iHealth"));
          return GetFieldValue<int>(m_iHealth);
     }
     bool IsAlive() {
          static int m_lifeState = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_lifeState"));
          return GetFieldValue<int>(m_lifeState) == 0;
     }
     int GetTeamNum() {
          static int m_iTeamNum = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_iTeamNum"));
          return GetFieldValue<int>(m_iTeamNum);
     }
     int GetFlags() {
          static int m_fFlags = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("m_fFlags"));
          return GetFieldValue<int>(m_fFlags);
     }
     SourceEngine::Vector GetViewOffset() {
          static int m_vecViewOffset = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_vecViewOffset[0]"));
          return GetFieldValue<SourceEngine::Vector>(m_vecViewOffset);
     }
     SourceEngine::Vector GetOrigin() {
          static int m_vecOrigin = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_vecOrigin"));
          return GetFieldValue<SourceEngine::Vector>(m_vecOrigin);
     }
     SourceEngine::Vector GetEyePos() {
          return GetOrigin() + GetViewOffset();
     }
     SourceEngine::Vector* ViewPunch() {
          static int m_viewPunchAngle = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_viewPunchAngle"));
          return GetFieldPointer<SourceEngine::Vector>(m_viewPunchAngle);
     }
     SourceEngine::Vector* AimPunch() {
          static int m_aimPunchAngle = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_Local"), XorStr("m_aimPunchAngle"));
          return GetFieldPointer<SourceEngine::Vector>(m_aimPunchAngle);
     }
};