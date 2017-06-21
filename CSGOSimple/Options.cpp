#include "Options.hpp"

namespace Options
{
    //Here we defined the extern variables declared on Options.hpp

    bool g_bMainWindowOpen = false;

    bool g_bESPEnabled = false;
    bool g_bESPShowBoxes = false;
    bool g_bESPShowNames = false;
    bool g_bRCSEnabled = false;
    bool g_bBHopEnabled = false;
    bool g_bAutoAccept = false;
    float g_bESPAllyColor[4] = {0.0f, 0.0f, 1.0f, 1.0f}; //RGBA color Team
    float g_bESPEnemyColor[4] = {1.0f, 0.0f, 0.0f, 1.0f}; //RGBA color Enemy
}
