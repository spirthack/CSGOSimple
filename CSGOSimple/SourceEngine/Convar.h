#pragma once

#include "IConVar.hpp"
#include "Color.hpp"

namespace SourceEngine
{
     class ConVar;
     class CCommand;
     class ConCommand;
     class ConCommandBase;

     class IConCommandBaseAccessor {
     public:
          virtual bool RegisterConCommandBase(ConCommandBase *pVar) = 0;
     };
     void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor *pAccessor);

     class ConCommandBase {
     public:
          ConCommandBase(void);
          ConCommandBase(const char *pName, const char *pHelpString = 0, int flags = 0);

          virtual                            ~ConCommandBase(void);
          virtual   bool                     IsCommand(void) const;
          virtual bool                       IsFlagSet(int flag) const;
          virtual void                       AddFlags(int flags);
          virtual void                       RemoveFlags(int flags);
          virtual int                        GetFlags() const;
          virtual const char*                GetName(void) const;
          virtual const char*                GetHelpText(void) const;
          virtual bool                       IsRegistered(void) const; 
          virtual int                        GetDLLIdentifier() const;
          virtual void                       Create(const char *pName, const char *pHelpString = 0, int flags = 0);
          virtual void                       Init();

     public:
          ConCommandBase*                    m_pNext;            //0x4
          bool                               m_bRegistered;      //0x8
          const char*                        m_pszName;          //0xC
          const char*                        m_pszHelpString;    //0x10
          int                                m_nFlags;           //0x14

     public:
          static ConCommandBase*             s_pConCommandBases;
          static IConCommandBaseAccessor*    s_pAccessor;
     };

     class ConVar : public ConCommandBase, public IConVar {
     public:
          typedef ConCommandBase BaseClass;

          ConVar(const char *pName, const char *pDefaultValue, int flags = 0);

          ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString);
          ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax);
          ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, FnChangeCallback_t callback);
          ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback);

          virtual                            ~ConVar(void);
          virtual bool                       IsFlagSet(int flag) const;
          virtual const char*                GetName(void) const;
          virtual const char*                GetBaseName(void) const;
          virtual int                        GetSplitScreenPlayerSlot() const;
          virtual   bool                     IsCommand(void) const;
          virtual void                       SetValue(const char *value);
          virtual void                       SetValue(float value);
          virtual void                       SetValue(int value);
          virtual void                       SetValue(Color value);
          virtual void                       InternalSetValue(const char *value);
          virtual void                       InternalSetFloatValue(float fNewValue);
          virtual void                       InternalSetIntValue(int nValue);
          virtual void                       InternalSetColorValue(Color value);
          virtual bool                       ClampValue(float& value);
          virtual void                       ChangeStringValue(const char *tempVal, float flOldValue);
          virtual void                       Create( const char *pName, const char *pDefaultValue, int flags = 0,
                                                            const char *pHelpString = 0, bool bMin = false, float fMin = 0.0,
                                                            bool bMax = false, float fMax = 0.0f, FnChangeCallback_t callback = 0);


          //----------------------------
          // Non-virtual helper methods
          //----------------------------
          float                              GetFloat(void) const;
          int                                GetInt(void) const;
          Color                              GetColor(void) const;
          const char*                        GetString(void) const;
          const char*                        GetDefault(void) const;

          ConVar*                            m_pParent;          //0x1C
          const char*                        m_pszDefaultValue;  //0x20
          char*                              m_pszString;        //0x24
          int                                m_StringLength;     //0x28
          float                              m_fValue;           //0x2C
          int                                m_nValue;           //0x30
          bool                               m_bHasMin;          //0x34
          float                              m_fMinVal;          //0x38
          bool                               m_bHasMax;          //0x3C
          float                              m_fMaxVal;          //0x40
          FnChangeCallback_t                 m_fnChangeCallback; //0x44
     };
}
