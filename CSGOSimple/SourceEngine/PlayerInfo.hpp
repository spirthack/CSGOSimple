#pragma once

#include "Definitions.hpp"

namespace SourceEngine {
	class PlayerInfo {
	public:
		char           pad_0x0000[0x10]; //0x0000
		char           szName[32]; //0x0010 
		char           pad_0x0030[0x64]; //0x0030
		char           szSteamID[20]; //0x0094 SteamID on text format (STEAM_X:Y:Z)
		char           pad_0x00A8[0x10]; //0x00A8
		unsigned long  iSteamID; //0x00B8 
		char           unknown2[0x14C];
	};
}