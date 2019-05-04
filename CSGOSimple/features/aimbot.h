#pragma once
#include "../valve_sdk/csgostructs.hpp"
#include "../valve_sdk/sdk.hpp"
#include "../helpers/math.hpp"
class Aimbot {
public:
	void CreateMove(CUserCmd* cmd);
};
extern Aimbot g_Aimbot;
