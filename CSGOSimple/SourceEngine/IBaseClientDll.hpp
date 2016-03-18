#pragma once

#include "Definitions.hpp"

#include "CGlobalVarsBase.hpp"
#include "ClientClass.hpp"

namespace SourceEngine {
     class IBaseClientDLL {
     public:
          // Connect appsystem components, get global interfaces, don't run any other init code
          virtual int              Connect( CreateInterfaceFn appSystemFactory, CGlobalVarsBase *pGlobals ) = 0;
          virtual int              Disconnect( void ) = 0;
          virtual int              Init( CreateInterfaceFn appSystemFactory, CGlobalVarsBase *pGlobals ) = 0;
          virtual void             PostInit() = 0;
          virtual void             Shutdown( void ) = 0;
          virtual void             LevelInitPreEntity( char const* pMapName ) = 0;
          virtual void             LevelInitPostEntity() = 0;
          virtual void             LevelShutdown( void ) = 0;
          virtual ClientClass*     GetAllClasses( void ) = 0;
     };
}