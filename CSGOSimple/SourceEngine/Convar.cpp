#include "Convar.hpp"
#include "SDK.hpp"

#define ALIGN_VALUE( val, alignment ) ( ( val + alignment - 1 ) & ~( alignment - 1 ) ) 
#define stackalloc( _size )		_alloca( ALIGN_VALUE( _size, 16 ) )

namespace SourceEngine
{

    ConCommandBase			*ConCommandBase::s_pConCommandBases = NULL;
    IConCommandBaseAccessor	*ConCommandBase::s_pAccessor = NULL;
    static int s_nCVarFlag = 0;
    static int s_nDLLIdentifier = -1;	// A unique identifier indicating which DLL this convar came from
    static bool s_bRegistered = false;

    class CDefaultAccessor : public IConCommandBaseAccessor
    {
    public:
        virtual bool RegisterConCommandBase(ConCommandBase *pVar)
        {
            // Link to engine's list instead
            Interfaces::CVar()->RegisterConCommand(pVar);
            return true;
        }
    };

    static CDefaultAccessor s_DefaultAccessor;

    //-----------------------------------------------------------------------------
    // Called by the framework to register ConCommandBases with the ICVar
    //-----------------------------------------------------------------------------
    void ConVar_Register(int nCVarFlag, IConCommandBaseAccessor *pAccessor)
    {
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

    void ConVar_Unregister()
    {
        if(!Interfaces::CVar() || !s_bRegistered)
            return;

        Interfaces::CVar()->UnregisterConCommands(s_nDLLIdentifier);
        s_nDLLIdentifier = -1;
        s_bRegistered = false;
    }


    //-----------------------------------------------------------------------------
    // Purpose: Default constructor
    //-----------------------------------------------------------------------------
    ConCommandBase::ConCommandBase(void)
    {
        m_bRegistered = false;
        m_pszName = NULL;
        m_pszHelpString = NULL;

        m_nFlags = 0;
        m_pNext = NULL;
    }

    //-----------------------------------------------------------------------------
    // Purpose: The base console invoked command/cvar interface
    // Input  : *pName - name of variable/command
    //			*pHelpString - help text
    //			flags - flags
    //-----------------------------------------------------------------------------
    ConCommandBase::ConCommandBase(const char *pName, const char *pHelpString /*=0*/, int flags /*= 0*/)
    {
        Create(pName, pHelpString, flags);
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    //-----------------------------------------------------------------------------
    ConCommandBase::~ConCommandBase(void)
    {
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Output : Returns true on success, false on failure.
    //-----------------------------------------------------------------------------
    bool ConCommandBase::IsCommand(void) const
    {
        //	Assert( 0 ); This can't assert. . causes a recursive assert in Sys_Printf, etc.
        return true;
    }


    //-----------------------------------------------------------------------------
    // Returns the DLL identifier
    //-----------------------------------------------------------------------------
    CVarDLLIdentifier_t ConCommandBase::GetDLLIdentifier() const
    {
        return s_nDLLIdentifier;
    }


    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : *pName - 
    //			callback - 
    //			*pHelpString - 
    //			flags - 
    //-----------------------------------------------------------------------------
    void ConCommandBase::Create(const char *pName, const char *pHelpString /*= 0*/, int flags /*= 0*/)
    {
        static const char *empty_string = "";

        m_bRegistered = false;

        // Name should be static data
        m_pszName = pName;
        m_pszHelpString = pHelpString ? pHelpString : empty_string;

        m_nFlags = flags;

    #ifdef ALLOW_DEVELOPMENT_CVARS
        m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
    #endif

        if(!(m_nFlags & FCVAR_UNREGISTERED)) {
            m_pNext = s_pConCommandBases;
            s_pConCommandBases = this;
        } else {
            // It's unregistered
            m_pNext = NULL;
        }

        // If s_pAccessor is already set (this ConVar is not a global variable),
        //  register it.
        //if(s_pAccessor) {
        //    Init();
        //}
        Interfaces::CVar()->RegisterConCommand(this);
    }


    //-----------------------------------------------------------------------------
    // Purpose: Used internally by OneTimeInit to initialize.
    //-----------------------------------------------------------------------------
    void ConCommandBase::Init()
    {
        if(s_pAccessor) {
            s_pAccessor->RegisterConCommandBase(this);
        }
    }

    void ConCommandBase::Shutdown()
    {
        if(Interfaces::CVar()) {
            Interfaces::CVar()->UnregisterConCommand(this);
        }
    }


    //-----------------------------------------------------------------------------
    // Purpose: Return name of the command/var
    // Output : const char
    //-----------------------------------------------------------------------------
    const char *ConCommandBase::GetName(void) const
    {
        return m_pszName;
    }


    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : flag - 
    // Output : Returns true on success, false on failure.
    //-----------------------------------------------------------------------------
    bool ConCommandBase::IsFlagSet(int flag) const
    {
        return (flag & m_nFlags) ? true : false;
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : flags - 
    //-----------------------------------------------------------------------------
    void ConCommandBase::AddFlags(int flags)
    {
        m_nFlags |= flags;

    #ifdef ALLOW_DEVELOPMENT_CVARS
        m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
    #endif
    }

    void ConCommandBase::RemoveFlags(int flags)
    {
        m_nFlags &= ~flags;
    }

    int ConCommandBase::GetFlags(void) const
    {
        return m_nFlags;
    }


    //-----------------------------------------------------------------------------
    // Purpose: 
    // Output : const ConCommandBase
    //-----------------------------------------------------------------------------
    const ConCommandBase *ConCommandBase::GetNext(void) const
    {
        return m_pNext;
    }

    ConCommandBase *ConCommandBase::GetNext(void)
    {
        return m_pNext;
    }


    //-----------------------------------------------------------------------------
    // Purpose: Copies string using local new/delete operators
    // Input  : *from - 
    // Output : char
    //-----------------------------------------------------------------------------
    char *ConCommandBase::CopyString(const char *from)
    {
        int		len;
        char	*to;

        len = strlen(from);
        if(len <= 0) {
            to = new char[1];
            to[0] = 0;
        } else {
            to = new char[len + 1];
            strncpy_s(to, len + 1, from, len + 1);
        }
        return to;
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Output : const char
    //-----------------------------------------------------------------------------
    const char *ConCommandBase::GetHelpText(void) const
    {
        return m_pszHelpString;
    }

    //-----------------------------------------------------------------------------
    // Purpose: Has this cvar been registered
    // Output : Returns true on success, false on failure.
    //-----------------------------------------------------------------------------
    bool ConCommandBase::IsRegistered(void) const
    {
        return m_bRegistered;
    }

    //-----------------------------------------------------------------------------
    // Default console command autocompletion function 
    //-----------------------------------------------------------------------------
    int DefaultCompletionFunc(const char *partial, char commands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH])
    {
        return 0;
    }


    //-----------------------------------------------------------------------------
    // Purpose: Constructs a console command
    //-----------------------------------------------------------------------------
    //ConCommand::ConCommand()
    //{
    //	m_bIsNewConCommand = true;
    //}

    ConCommand::ConCommand(const char *pName, FnCommandCallbackV1_t callback, const char *pHelpString /*= 0*/, int flags /*= 0*/, FnCommandCompletionCallback completionFunc /*= 0*/)
    {
        // Set the callback
        m_fnCommandCallbackV1 = callback;
        m_bUsingNewCommandCallback = false;
        m_bUsingCommandCallbackInterface = false;
        m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
        m_bHasCompletionCallback = completionFunc != 0 ? true : false;

        // Setup the rest
        BaseClass::Create(pName, pHelpString, flags);
    }

    ConCommand::ConCommand(const char *pName, FnCommandCallback_t callback, const char *pHelpString /*= 0*/, int flags /*= 0*/, FnCommandCompletionCallback completionFunc /*= 0*/)
    {
        // Set the callback
        m_fnCommandCallback = callback;
        m_bUsingNewCommandCallback = true;
        m_fnCompletionCallback = completionFunc ? completionFunc : DefaultCompletionFunc;
        m_bHasCompletionCallback = completionFunc != 0 ? true : false;
        m_bUsingCommandCallbackInterface = false;

        // Setup the rest
        BaseClass::Create(pName, pHelpString, flags);
    }

    ConCommand::ConCommand(const char *pName, ICommandCallback *pCallback, const char *pHelpString /*= 0*/, int flags /*= 0*/, ICommandCompletionCallback *pCompletionCallback /*= 0*/)
    {
        // Set the callback
        m_pCommandCallback = pCallback;
        m_bUsingNewCommandCallback = false;
        m_pCommandCompletionCallback = pCompletionCallback;
        m_bHasCompletionCallback = (pCompletionCallback != 0);
        m_bUsingCommandCallbackInterface = true;

        // Setup the rest
        BaseClass::Create(pName, pHelpString, flags);
    }

    //-----------------------------------------------------------------------------
    // Destructor
    //-----------------------------------------------------------------------------
    ConCommand::~ConCommand(void)
    {
    }


    //-----------------------------------------------------------------------------
    // Purpose: Returns true if this is a command 
    //-----------------------------------------------------------------------------
    bool ConCommand::IsCommand(void) const
    {
        return true;
    }


    //-----------------------------------------------------------------------------
    // Purpose: Invoke the function if there is one
    //-----------------------------------------------------------------------------
    void ConCommand::Dispatch(const CCommand &command)
    {
        if(m_bUsingNewCommandCallback) {
            if(m_fnCommandCallback) {
                (*m_fnCommandCallback)(command);
                return;
            }
        } else if(m_bUsingCommandCallbackInterface) {
            if(m_pCommandCallback) {
                m_pCommandCallback->CommandCallback(command);
                return;
            }
        } else {
            if(m_fnCommandCallbackV1) {
                (*m_fnCommandCallbackV1)();
                return;
            }
        }

        // Command without callback!!!
        //AssertMsg(0, ("Encountered ConCommand without a callback!\n"));
    }


    //-----------------------------------------------------------------------------
    // Purpose: Calls the autocompletion method to get autocompletion suggestions
    //-----------------------------------------------------------------------------
    int	ConCommand::AutoCompleteSuggest(const char *partial, CUtlVector< CUtlString > &commands)
    {
        if(m_bUsingCommandCallbackInterface) {
            if(!m_pCommandCompletionCallback)
                return 0;
            return m_pCommandCompletionCallback->CommandCompletionCallback(partial, commands);
        }

        if(!m_fnCompletionCallback)
            return 0;

        char rgpchCommands[COMMAND_COMPLETION_MAXITEMS][COMMAND_COMPLETION_ITEM_LENGTH];
        int iret = (m_fnCompletionCallback)(partial, rgpchCommands);
        for(int i = 0; i < iret; ++i) {
            CUtlString str = rgpchCommands[i];
            commands.AddToTail(str);
        }
        return iret;
    }


    //-----------------------------------------------------------------------------
    // Returns true if the console command can autocomplete 
    //-----------------------------------------------------------------------------
    bool ConCommand::CanAutoComplete(void)
    {
        return m_bHasCompletionCallback;
    }



    //-----------------------------------------------------------------------------
    //
    // Console Variables
    //
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Various constructors
    //-----------------------------------------------------------------------------
    ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags /* = 0 */)
    {
        Create(pName, pDefaultValue, flags);
    }

    ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString)
    {
        Create(pName, pDefaultValue, flags, pHelpString);
    }

    ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax)
    {
        Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax);
    }

    ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, FnChangeCallback_t callback)
    {
        Create(pName, pDefaultValue, flags, pHelpString, false, 0.0, false, 0.0, callback);
    }

    ConVar::ConVar(const char *pName, const char *pDefaultValue, int flags, const char *pHelpString, bool bMin, float fMin, bool bMax, float fMax, FnChangeCallback_t callback)
    {
        Create(pName, pDefaultValue, flags, pHelpString, bMin, fMin, bMax, fMax, callback);
    }


    //-----------------------------------------------------------------------------
    // Destructor
    //-----------------------------------------------------------------------------
    ConVar::~ConVar(void)
    {
        //if(IsRegistered())
        //    Interfaces::CVar()->UnregisterConCommand(this);
        if(m_Value.m_pszString) {
            delete[] m_Value.m_pszString;
            m_Value.m_pszString = NULL;
        }
    }


    //-----------------------------------------------------------------------------
    // Install a change callback (there shouldn't already be one....)
    //-----------------------------------------------------------------------------
    void ConVar::InstallChangeCallback(FnChangeCallback_t callback, bool bInvoke)
    {
        if(callback) {
            if(m_fnChangeCallbacks.Find(callback) != -1) {
                m_fnChangeCallbacks.AddToTail(callback);
                if(bInvoke)
                    callback(this, m_Value.m_pszString, m_Value.m_fValue);
            } else {
                //Warning("InstallChangeCallback ignoring duplicate change callback!!!\n");
            }
        } else {
            //Warning("InstallChangeCallback called with NULL callback, ignoring!!!\n");
        }
    }

    bool ConVar::IsFlagSet(int flag) const
    {
        return (flag & m_pParent->m_nFlags) ? true : false;
    }

    const char *ConVar::GetHelpText(void) const
    {
        return m_pParent->m_pszHelpString;
    }

    void ConVar::AddFlags(int flags)
    {
        m_pParent->m_nFlags |= flags;

    #ifdef ALLOW_DEVELOPMENT_CVARS
        m_pParent->m_nFlags &= ~FCVAR_DEVELOPMENTONLY;
    #endif
    }

    int ConVar::GetFlags(void) const
    {
        return m_pParent->m_nFlags;
    }

    bool ConVar::IsRegistered(void) const
    {
        return m_pParent->m_bRegistered;
    }

    const char *ConVar::GetName(void) const
    {
        return m_pParent->m_pszName;
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Output : Returns true on success, false on failure.
    //-----------------------------------------------------------------------------
    bool ConVar::IsCommand(void) const
    {
        return false;
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : 
    //-----------------------------------------------------------------------------
    void ConVar::Init()
    {
        BaseClass::Init();
    }

    const char *ConVar::GetBaseName(void) const
    {
        return m_pParent->m_pszName;
    }

    int ConVar::GetSplitScreenPlayerSlot(void) const
    {
        return 0;
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : *value - 
    //-----------------------------------------------------------------------------
    void ConVar::InternalSetValue(const char *value)
    {
        float fNewValue;
        char  tempVal[32];
        char  *val;

        float flOldValue = m_Value.m_fValue;

        val = (char *)value;
        fNewValue = (float)atof(value);

        if(ClampValue(fNewValue)) {
            snprintf(tempVal, sizeof(tempVal), "%f", fNewValue);
            val = tempVal;
        }

        // Redetermine value
        m_Value.m_fValue = fNewValue;
        m_Value.m_nValue = (int)(fNewValue);

        if(!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
            ChangeStringValue(val, flOldValue);
        }
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : *tempVal - 
    //-----------------------------------------------------------------------------
    void ConVar::ChangeStringValue(const char *tempVal, float flOldValue)
    {
        char* pszOldValue = (char*)stackalloc(m_Value.m_StringLength);
        memcpy(pszOldValue, m_Value.m_pszString, m_Value.m_StringLength);

        int len = strlen(tempVal) + 1;

        if(len > m_Value.m_StringLength) {
            if(m_Value.m_pszString) {
                delete[] m_Value.m_pszString;
            }

            m_Value.m_pszString = new char[len];
            m_Value.m_StringLength = len;
        }

        memcpy(m_Value.m_pszString, tempVal, len);

        // Invoke any necessary callback function
        for(int i = 0; i < m_fnChangeCallbacks.Count(); i++) {
            m_fnChangeCallbacks[i](this, pszOldValue, flOldValue);
        }

        if(Interfaces::CVar())
            Interfaces::CVar()->CallGlobalChangeCallbacks(this, pszOldValue, flOldValue);
    }

    //-----------------------------------------------------------------------------
    // Purpose: Check whether to clamp and then perform clamp
    // Input  : value - 
    // Output : Returns true if value changed
    //-----------------------------------------------------------------------------
    bool ConVar::ClampValue(float& value)
    {
        if(m_bHasMin && (value < m_fMinVal)) {
            value = m_fMinVal;
            return true;
        }

        if(m_bHasMax && (value > m_fMaxVal)) {
            value = m_fMaxVal;
            return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : *value - 
    //-----------------------------------------------------------------------------
    void ConVar::InternalSetFloatValue(float fNewValue)
    {
        if(fNewValue == m_Value.m_fValue)
            return;

        ClampValue(fNewValue);

        // Redetermine value
        float flOldValue = m_Value.m_fValue;
        *(DWORD*)&m_Value.m_fValue = *(DWORD*)&fNewValue ^ (DWORD)this;
        *(DWORD*)&m_Value.m_nValue = *(DWORD*)&fNewValue ^ (DWORD)this;

        if(!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
            char tempVal[32];
            snprintf(tempVal, sizeof(tempVal), "%f", m_Value.m_fValue);
            ChangeStringValue(tempVal, flOldValue);
        } else {
            //Assert(m_fnChangeCallbacks.Count() == 0);
        }
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : *value - 
    //-----------------------------------------------------------------------------
    void ConVar::InternalSetIntValue(int nValue)
    {
        if(nValue == m_Value.m_nValue)
            return;

        float fValue = (float)nValue;
        if(ClampValue(fValue)) {
            nValue = (int)(fValue);
        }

        // Redetermine value
        float flOldValue = m_Value.m_fValue;
        *(DWORD*)&m_Value.m_fValue = *(DWORD*)&fValue ^ (DWORD)this;
        *(DWORD*)&m_Value.m_nValue = *(DWORD*)&nValue ^ (DWORD)this;

        if(!(m_nFlags & FCVAR_NEVER_AS_STRING)) {
            char tempVal[32];
            snprintf(tempVal, sizeof(tempVal), "%d", m_Value.m_nValue);
            ChangeStringValue(tempVal, flOldValue);
        } else {
            //Assert(m_fnChangeCallbacks.Count() == 0);
        }
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : *value - 
    //-----------------------------------------------------------------------------
    void ConVar::InternalSetColorValue(Color cValue)
    {
        int color = (int)cValue.GetARGB();
        InternalSetIntValue(color);
    }

    //-----------------------------------------------------------------------------
    // Purpose: Private creation
    //-----------------------------------------------------------------------------
    void ConVar::Create(const char *pName, const char *pDefaultValue, int flags /*= 0*/,
        const char *pHelpString /*= NULL*/, bool bMin /*= false*/, float fMin /*= 0.0*/,
        bool bMax /*= false*/, float fMax /*= false*/, FnChangeCallback_t callback /*= NULL*/)
    {
        static const char *empty_string = "";

        m_pParent = this;

        // Name should be static data
        m_pszDefaultValue = pDefaultValue ? pDefaultValue : empty_string;

        m_Value.m_StringLength = strlen(m_pszDefaultValue) + 1;
        m_Value.m_pszString = new char[m_Value.m_StringLength];
        memcpy(m_Value.m_pszString, m_pszDefaultValue, m_Value.m_StringLength);

        m_bHasMin = bMin;
        m_fMinVal = fMin;
        m_bHasMax = bMax;
        m_fMaxVal = fMax;

        if(callback)
            m_fnChangeCallbacks.AddToTail(callback);

        float value = (float)atof(m_Value.m_pszString);
        *(DWORD*)&m_Value.m_fValue = *(DWORD*)&value ^ (DWORD)this;

        // Bounds Check, should never happen, if it does, no big deal
        if(m_bHasMin && (m_Value.m_fValue < m_fMinVal)) {
            //Assert(0);
        }

        if(m_bHasMax && (m_Value.m_fValue > m_fMaxVal)) {
            //Assert(0);
        }

        *(DWORD*)&m_Value.m_nValue = *(DWORD*)&m_Value.m_fValue;

        BaseClass::Create(pName, pHelpString, flags);
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : *value - 
    //-----------------------------------------------------------------------------
    void ConVar::SetValue(const char *value)
    {
        ConVar *var = (ConVar *)m_pParent;
        var->InternalSetValue(value);
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : value - 
    //-----------------------------------------------------------------------------
    void ConVar::SetValue(float value)
    {
        ConVar *var = (ConVar *)m_pParent;
        var->InternalSetFloatValue(value);
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : value - 
    //-----------------------------------------------------------------------------
    void ConVar::SetValue(int value)
    {
        ConVar *var = (ConVar *)m_pParent;
        var->InternalSetIntValue(value);
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : value - 
    //-----------------------------------------------------------------------------
    void ConVar::SetValue(Color value)
    {
        ConVar *var = (ConVar *)m_pParent;
        var->InternalSetColorValue(value);
    }

    //-----------------------------------------------------------------------------
    // Purpose: Reset to default value
    //-----------------------------------------------------------------------------
    void ConVar::Revert(void)
    {
        // Force default value again
        ConVar *var = (ConVar *)m_pParent;
        var->SetValue(var->m_pszDefaultValue);
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : minVal - 
    // Output : true if there is a min set
    //-----------------------------------------------------------------------------
    bool ConVar::GetMin(float& minVal) const
    {
        minVal = m_pParent->m_fMinVal;
        return m_pParent->m_bHasMin;
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Input  : maxVal - 
    //-----------------------------------------------------------------------------
    bool ConVar::GetMax(float& maxVal) const
    {
        maxVal = m_pParent->m_fMaxVal;
        return m_pParent->m_bHasMax;
    }

    //-----------------------------------------------------------------------------
    // Purpose: 
    // Output : const char
    //-----------------------------------------------------------------------------
    const char *ConVar::GetDefault(void) const
    {
        return m_pParent->m_pszDefaultValue;
    }
}