//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================

#include <string.h>
#include "characterset.hpp"

namespace se
{
    //-----------------------------------------------------------------------------
    // Purpose: builds a simple lookup table of a group of important characters
    // Input  : *pParseGroup - pointer to the buffer for the group
    //			*pGroupString - null terminated list of characters to flag
    //-----------------------------------------------------------------------------
    void CharacterSetBuild(characterset_t *pSetBuffer, const char *pszSetString)
    {
        int i = 0;

        // Test our pointers
        if(!pSetBuffer || !pszSetString)
            return;

        memset(pSetBuffer->set, 0, sizeof(pSetBuffer->set));

        while(pszSetString[i]) {
            pSetBuffer->set[pszSetString[i]] = 1;
            i++;
        }

    }
}
