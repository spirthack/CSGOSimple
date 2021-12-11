#pragma once

#include "../Misc/CUserCmd.hpp"

#define MULTIPLAYER_BACKUP 150

class bf_write;
class bf_read;

class CInput
{
public:
    char                pad_0000[12];
    bool                m_fTrackIRAvailable;
    bool                m_fMouseInitialized;
    bool                m_fMouseActive;
    bool                m_fJoystickAdvancedInit;
    char                pad_0010[44];
    char*               m_pKeys;
    char                pad_0040[48];
    int32_t             m_nCamCommand;
    char                pad_0074[76];
    bool                m_fCameraInterceptingMouse;
    bool                m_fCameraInThirdPerson;
    bool                m_fCameraMovingWithMouse;
    char                pad_00C3[1];
    Vector              m_vecCameraOffset;
    bool                m_fCameraDistanceMove;
    char                pad_00D1[19];
    bool                m_CameraIsOrthographic;
    bool                m_CameraIsThirdPersonOverview;
    char                pad_00E6[2];
    QAngle*             m_angPreviousViewAngles;
    QAngle*             m_angPreviousViewAnglesTilt;
    char                pad_00F0[16];
    float               m_flLastForwardMove;
    int32_t             m_nClearInputState;
    CUserCmd*           m_pCommands;
    CVerifiedUserCmd*   m_pVerifiedCommands;

	inline CUserCmd* GetUserCmd(int sequence_number);
	inline CUserCmd * GetUserCmd(int nSlot, int sequence_number);
	inline CVerifiedUserCmd* GetVerifiedCmd(int sequence_number);
};

CUserCmd* CInput::GetUserCmd(const int sequence_number)
{
    return &(*(CUserCmd**)((DWORD)this + 0x0108))[sequence_number % MULTIPLAYER_BACKUP];
}

CUserCmd *CInput::GetUserCmd(int nSlot, const int sequence_number)
{
	typedef CUserCmd*(__thiscall *GetUserCmd_t)(void*, int, int);
	return CallVFunction<GetUserCmd_t>(this, 8)(this, nSlot, sequence_number);
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(const int sequence_number)
{
    return &(*(CVerifiedUserCmd**)((DWORD)this + 0x010C))[sequence_number % MULTIPLAYER_BACKUP];
}
