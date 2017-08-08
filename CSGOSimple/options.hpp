#pragma once

#include <string>
#include "valve_sdk/Misc/Color.hpp"

#define OPTION(type, var, val) type var = val

class Config
{
public:
    // 
    // ESP
    // 
    OPTION(bool, esp_enabled, false);
    OPTION(bool, esp_enemies_only, false);
    OPTION(bool, esp_player_boxes, false);
    OPTION(bool, esp_player_names, false);
    OPTION(bool, esp_player_health, false);
    OPTION(bool, esp_player_armour, false);
    OPTION(bool, esp_player_weapons, false);
    OPTION(bool, esp_player_snaplines, false);
    OPTION(bool, esp_crosshair, false);
    OPTION(bool, esp_dropped_weapons, false);
    OPTION(bool, esp_defuse_kit, false);
    OPTION(bool, esp_planted_c4, false);

    // 
    // GLOW
    // 
    OPTION(bool, glow_enabled, false);
    OPTION(bool, glow_enemies_only, false);
    OPTION(bool, glow_players, false);
    OPTION(bool, glow_chickens, false);
    OPTION(bool, glow_c4_carrier, false);
    OPTION(bool, glow_planted_c4, false);
    OPTION(bool, glow_defuse_kits, false);
    OPTION(bool, glow_weapons, false);

    //
    // CHAMS
    //
    OPTION(bool, chams_player_enabled, false);
    OPTION(bool, chams_player_enemies_only, false);
    OPTION(bool, chams_player_wireframe, false);
    OPTION(bool, chams_player_flat, false);
    OPTION(bool, chams_player_ignorez, false);
    OPTION(bool, chams_player_glass, false);
    OPTION(bool, chams_arms_enabled, false);
    OPTION(bool, chams_arms_wireframe, false);
    OPTION(bool, chams_arms_flat, false);
    OPTION(bool, chams_arms_ignorez, false);
    OPTION(bool, chams_arms_glass, false);

    //
    // MISC
    //
    OPTION(bool, misc_bhop, false);
    OPTION(bool, misc_no_hands, false);
    OPTION(int, viewmodel_fov, 68);
    OPTION(float, mat_ambient_light_r, 0.0f);
    OPTION(float, mat_ambient_light_g, 0.0f);
    OPTION(float, mat_ambient_light_b, 0.0f);

    // 
    // COLORS
    // 
    OPTION(Color, color_esp_ally_visible, Color(0, 128, 255));
    OPTION(Color, color_esp_enemy_visible, Color(255, 0, 0));
    OPTION(Color, color_esp_ally_occluded, Color(0, 128, 255));
    OPTION(Color, color_esp_enemy_occluded, Color(255, 0, 0));
    OPTION(Color, color_esp_crosshair, Color(255, 255, 255));
    OPTION(Color, color_esp_weapons, Color(128, 0, 128));
    OPTION(Color, color_esp_defuse, Color(0, 128, 255));
    OPTION(Color, color_esp_c4, Color(255, 255, 0));

    OPTION(Color, color_glow_ally, Color(0, 128, 255));
    OPTION(Color, color_glow_enemy, Color(255, 0, 0));
    OPTION(Color, color_glow_chickens, Color(0, 128, 0));
    OPTION(Color, color_glow_c4_carrier, Color(255, 255, 0));
    OPTION(Color, color_glow_planted_c4, Color(128, 0, 128));
    OPTION(Color, color_glow_defuse, Color(255, 255, 255));
    OPTION(Color, color_glow_weapons, Color(255, 128, 0));

    OPTION(Color, color_chams_player_ally_visible, Color(0, 128, 255));
    OPTION(Color, color_chams_player_ally_occluded, Color(0, 255, 128));
    OPTION(Color, color_chams_player_enemy_visible, Color(255, 0, 0));
    OPTION(Color, color_chams_player_enemy_occluded, Color(255, 128, 0));
    OPTION(Color, color_chams_arms_visible, Color(0, 128, 255));
    OPTION(Color, color_chams_arms_occluded, Color(0, 128, 255));
};

extern Config g_Options;
extern bool   g_Unload;
