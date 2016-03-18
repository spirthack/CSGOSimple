#include "Convar.h"
#include "SDK.hpp"

namespace SourceEngine
{
     ConCommandBase*          ConCommandBase::s_pConCommandBases = NULL;
     static int               s_nCVarFlag = 0;
     static int               s_nDLLIdentifier = -1;
     static bool              s_bRegistered = false;

     class CDefaultAccessor : public IConCommandBaseAccessor {
     public:
          bool RegisterConCommandBase(ConCommandBase *pVar) {
               SourceEngine::Interfaces::CVar()->RegisterConCommand(pVar);
               return true;
          }
     };

     static CDefaultAccessor s_DefaultAccessor;
     IConCommandBaseAccessor  *ConCommandBase::s_pAccessor = &s_DefaultAccessor;

     void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor *pAccessor) {
          if(!Interfaces::CVar() || s_bRegistered)
               return;

          s_bRegistered = true;
          s_nCVarFlag = nCVarFlag;
          s_nDLLIdentifier = Interfaces::CVar()->AllocateDLLIdentifier();

          ConCommandBase *pCur, *pNext;

          ConCommandBase::s_pAccessor = pAccessor ? pAccessor : &s_DefaultAccessor;
          pCur = ConCommandBase::s_pConCommandBases;
          while(pCur) {
               pNext = pCur->m_pNext;
               pCur->AddFlags(s_nCVarFlag);
               pCur->Init();
               pCur = pNext;
          }

          ConCommandBase::s_pConCommandBases = NULL;
     }

     ConCommandBase::ConCommandBase(void) {
          m_bRegistered = false;
          m_pszName = NULL;
          m_pszHelpString = NULL;

          m_nFlags = 0;
          m_pNext = NULL;
     }
     ConCommandBase::ConCommandBase(const char *pName, const char *pHelpString /*=0*/, int flags /*= 0*/) {
          Create(pName, pHelpString, flags);
     }
     ConCommandBase::~ConCommandBase(void) {
     }

     bool ConCommandBase::IsCommand(void) const {
          return true;
     }
     int ConCommandBase::GetDLLIdentifier() const {
          return s_nDLLIdentifier;
     }
     void ConCommandBase::Create(const char *pName, const char *pHelpString /*= 0*/, int flags /*= 0*/) {
          static const char* empty_string = "";

          m_bRegistered = false;

          m_pszName = pName;
          m_pszHelpString = pHelpString ? pHelpString : empty_string;

          m_nFlags = flags;

          if(!(m_nFlags & FCVAR_UNREGISTERED)) {
               m_pNext = s_pConCommandBases;
               s_pConCommandBases = this;
          } else {
               m_pNext = NULL;
          }

          Init();
     }
     void ConCommandBase::Init() {
          if(s_pAccessor) {
               s_pAccessor->RegisterConCommandBase(this);
          }
     }
     const char* ConCommandBase::GetName(void) const {
          return m_pszName;
     }
     bool ConCommandBase::IsFlagSet(int flag) const {
          return (flag & m_nFlags) ? true : false;
     }
     void ConCommandBase::AddFlags(int flags) {
          m_nFlags |= flags;
     }
     void ConCommandBase::RemoveFlags(int flags) {
          m_nFlags &= ~flags;
     }
     int ConCommandBase::GetFlags(void) const {
          return m_nFlags;
     }
     const char* ConCommandBase::GetHelpText(void) const {
          return m_pszHelpString;
     }
     bool ConCommandBase::IsRegistered(void) const {
          return m_bRegistered;
     }

     ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags) {
          Create(pName, pDefaultValue, flags);
     }

     ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString) {
          Create(pName, pDefaultValue, flags, pHelpString);
     }
     ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax) {
          Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax);
     }
     ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, FnChangeCallback_t callback) {
          Create(pName, pDefaultValue, flags, pHelpString, false, 0.0f, false, 0.0f, callback);
     }
     ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback) {
          Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax, callback);
     }
     ConVar::~ConVar(void) {
          if(Interfaces::CVar() && IsRegistered())
               Interfaces::CVar()->UnregisterConCommand(this);
     }
     bool ConVar::IsCommand(void) const {
          return false;
     }
     void ConVar::SetValue(const char *pValue) {
          m_pParent->InternalSetValue(pValue);
     }
     void ConVar::SetValue(float flValue){
          m_pParent->InternalSetFloatValue(flValue);
     }
     void ConVar::SetValue(int nValue) {
          m_pParent->InternalSetIntValue(nValue);
     }
     const char* ConVar::GetName(void) const {
          return m_pParent->m_pszName;
     }
     bool ConVar::IsFlagSet(int nFlag) const {
          return (m_pParent->m_nFlags & nFlag) != 0;
     }
     const char* ConVar::GetBaseName(void) const {
          return GetName();
     }
     int ConVar::GetSplitScreenPlayerSlot() const {
          return 0;
     }
     void ConVar::SetValue(Color value) {
          m_pParent->InternalSetColorValue(value);
     }
     void ConVar::InternalSetValue(const char *value) {
          float flValue = (float)atof(value);
          char* szValue = (char*)value;
          ClampValue(flValue);
          char tempVal[32];
          sprintf_s(tempVal, 32, "%f", flValue);
          szValue = tempVal;
          if(!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
               ChangeStringValue(tempVal, m_fValue);
          }
          m_fValue = flValue;
          m_nValue = (int)flValue;
     }
     void ConVar::InternalSetFloatValue(float fNewValue) {
          ClampValue(fNewValue);
          char tempVal[32];
          sprintf_s(tempVal, 32, "%f", fNewValue);
          if(!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
               ChangeStringValue(tempVal, m_fValue);
          }
          m_fValue = fNewValue;
          m_nValue = (int)fNewValue;
     }
     void ConVar::InternalSetIntValue(int nValue) {
          float fNewValue = (float)nValue;
          ClampValue(fNewValue);
          char tempVal[32];
          sprintf_s(tempVal, 32, "%d", (int)fNewValue);
          if(!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
               ChangeStringValue(tempVal, m_fValue);
          }
          m_fValue = (float)fNewValue;
          m_nValue = (int)fNewValue;
     }
     void ConVar::InternalSetColorValue(Color value) {
          int color = value.GetARGB();
          InternalSetIntValue(color);
     }
     bool ConVar::ClampValue(float& value) {
          if(m_bHasMin && value < m_fMinVal) {
               value = m_fMinVal;
               return true;
          }
          if(m_bHasMax && value > m_fMaxVal) {
               value = m_fMaxVal;
               return true;
          }
          return false;
     }
     void ConVar::ChangeStringValue(const char *tempVal, float flOldValue) {
          char* pszOldValue = new char[m_StringLength];
          memcpy(pszOldValue, m_pszString, m_StringLength);

          int len = strlen(tempVal) + 1;

          if(len > m_StringLength) {
               if(m_pszString) {
                    delete[] m_pszString;
               }

               m_pszString = new char[len];
               m_StringLength = len;
          }

          memcpy(m_pszString, tempVal, len);

          // Invoke any necessary callback function
          if(m_fnChangeCallback) {
               m_fnChangeCallback(this, pszOldValue, flOldValue);
          }

          Interfaces::CVar()->CallGlobalChangeCallbacks(this, pszOldValue, flOldValue);

          delete[] pszOldValue;
     }
     void ConVar::Create(const char *pName, const char *pDefaultValue, int flags,
          const char *pHelpString, bool bMin, float fMin,
          bool bMax, float fMax, FnChangeCallback_t callback) {
          static char *empty_string = "";

          m_pParent = this;
          m_pszDefaultValue = pDefaultValue ? pDefaultValue : empty_string;

          m_StringLength = strlen(m_pszDefaultValue) + 1;
          m_pszString = new char[m_StringLength];
          memcpy(m_pszString, m_pszDefaultValue, m_StringLength);

          m_bHasMin = bMin;
          m_fMinVal = fMin;
          m_bHasMax = bMax;
          m_fMaxVal = fMax;

          m_fnChangeCallback = callback;

          m_fValue = (float)atof(m_pszString);
          m_nValue = (int)m_fValue;

          BaseClass::Create(pName, pHelpString, flags);
     }
     float ConVar::GetFloat(void) const { return m_fValue; }
     int ConVar::GetInt(void) const { return m_nValue; }
     Color ConVar::GetColor(void) const { return Color::FromARGB(m_nValue); }
     const char* ConVar::GetString(void) const { return m_pszString; }
     const char* ConVar::GetDefault(void) const { return m_pszDefaultValue; }

}