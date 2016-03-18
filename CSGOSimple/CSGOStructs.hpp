#pragma once

#include <Windows.h>
#include "SourceEngine/IClientEntity.hpp"
#include "SourceEngine/Vector.hpp"
#include "NetvarManager.hpp"

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