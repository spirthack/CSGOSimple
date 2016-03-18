#pragma once

#include "Definitions.hpp"
#include "IAppSystem.hpp"
#include "IConVar.hpp"

namespace SourceEngine
{
     class ConCommandBase;
     class ConCommand;
     class ConVar;
     class Color;

     typedef int CVarDLLIdentifier_t;

     class IConsoleDisplayFunc
     {
     public:
          virtual void ColorPrint(const Color& clr, const char *pMessage) = 0;
          virtual void Print(const char *pMessage) = 0;
          virtual void DPrint(const char *pMessage) = 0;
     };

     class ICvar : public IAppSystem
     {
     public:
          virtual CVarDLLIdentifier_t        AllocateDLLIdentifier() = 0; // 9
          virtual void                       RegisterConCommand(ConCommandBase *pCommandBase) = 0; //10
          virtual void                       UnregisterConCommand(ConCommandBase *pCommandBase) = 0;
          virtual void                       UnregisterConCommands(CVarDLLIdentifier_t id) = 0;
          virtual const char*                GetCommandLineValue(const char *pVariableName) = 0; //11
          virtual ConCommandBase*            FindCommandBase(const char *name) = 0; //12
          virtual const ConCommandBase*      FindCommandBase(const char *name) const = 0;
          virtual ConVar*                    FindVar(const char *var_name) = 0;
          virtual const ConVar*              FindVar(const char *var_name) const = 0;
          virtual ConCommand*                FindCommand(const char *name) = 0;
          virtual const ConCommand*          FindCommand(const char *name) const = 0; //17
          virtual void                       InstallGlobalChangeCallback(FnChangeCallback_t callback) = 0;
          virtual void                       RemoveGlobalChangeCallback(FnChangeCallback_t callback) = 0;
          virtual void                       CallGlobalChangeCallbacks(ConVar *var, const char *pOldString, float flOldValue) = 0;
          virtual void                       InstallConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
          virtual void                       RemoveConsoleDisplayFunc(IConsoleDisplayFunc* pDisplayFunc) = 0;
          virtual void                       ConsoleColorPrintf(const Color& clr, const char *pFormat, ...) const = 0;
          virtual void                       ConsolePrintf(const char *pFormat, ...) const = 0;
          virtual void                       ConsoleDPrintf(const char *pFormat, ...) const = 0;
          virtual void                       RevertFlaggedConVars(int nFlag) = 0;
     };
}
