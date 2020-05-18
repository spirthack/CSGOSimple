#pragma once

#include "../singleton.hpp"
#include "../render.hpp"
#include "../helpers/math.hpp"
#include "../helpers/rect.hpp"
#include "../valve_sdk/csgostructs.hpp"




class Visuals : public Singleton<Visuals>
{
public:
	class Player
	{
	public:
		struct
		{
			C_BasePlayer* pl;
			bool          is_enemy;
			bool          is_visible;
			Color         clr;
			Vector        head_pos;
			Vector        feet_pos;
			SRect         bbox;
		} ctx;

		bool Begin(C_BasePlayer * pl);
		void RenderBox();
		void RenderName();
		void RenderWeaponName();
		void RenderHealth();
		void RenderArmour();
		void RenderSnapline();
	};
	void RenderCrosshair();
	void RenderWeapon(C_BaseCombatWeapon* ent);
	void RenderDefuseKit(C_BaseEntity* ent);
	void RenderPlantedC4(C_BaseEntity* ent);
	void RenderItemEsp(C_BaseEntity* ent);
	void ThirdPerson();
public:
	void Render();
};
