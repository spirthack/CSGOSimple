#pragma once

#include "Definitions.hpp"
namespace SourceEngine {
     class CGlobalVarsBase {
     public:
          float     realtime;
          int       framecount;
          float     absoluteframetime;
          float     absoluteframestarttimestddev;
          float     curtime;
          float     frametime;
          int       maxClients;
          int       tickcount;
          float     interval_per_tick;
          float     interpolation_amount;
          int       simTicksThisFrame;
          int       network_protocol;
          void*     pSaveData;
          bool      m_bClient;
          bool      m_bRemoteClient;

     private:
          // 100 (i.e., tickcount is rounded down to this base and then the "delta" from this base is networked
          int       nTimestampNetworkingBase;
          // 32 (entindex() % nTimestampRandomizeWindow ) is subtracted from gpGlobals->tickcount to set the networking basis, prevents
          //  all of the entities from forcing a new PackedEntity on the same tick (i.e., prevents them from getting lockstepped on this)
          int       nTimestampRandomizeWindow;
     };
}