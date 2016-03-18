#pragma once

#include "CSGOStructs.hpp"
#include "DrawManager.hpp"

class EntityESP {
public:
     EntityESP(C_CSPlayer* pEntity);
     ~EntityESP();

     void RenderESP(DrawManager& renderer);
     void RenderName(DrawManager& renderer);

private:
     C_CSPlayer* m_pEntity;
};

