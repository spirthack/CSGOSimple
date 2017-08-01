#pragma once

#include "../valve_sdk/Math/Vector.hpp"
#include "../valve_sdk/Interfaces/IVEngineClient.hpp"

class C_BasePlayer;
class C_BaseEntity;
class C_BaseCombatWeapon;
class C_PlantedC4;
class Color;
class ClientClass;

namespace Visuals
{

    struct ESPBox
    {
        int x, y, w, h, gay;
    };

    namespace player
    {
        bool begin(C_BasePlayer* pl);

        void RenderBox();
        void RenderName();
        void RenderHealth();
        void RenderWeapon();
        void RenderSnapline();
    }

    namespace Misc
    {
        void RenderCrosshair();
        void RenderWeapon(C_BaseCombatWeapon* ent);
        void RenderDefuseKit(C_BaseEntity* ent);
        void RenderPlantedC4(C_BaseEntity* ent);
    }

    bool CreateFonts();
    void DestroyFonts();
}
