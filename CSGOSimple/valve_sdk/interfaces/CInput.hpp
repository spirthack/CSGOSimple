#pragma once

#include "../Misc/CUserCmd.hpp"

#define MULTIPLAYER_BACKUP 150

class bf_write;
class bf_read;

class CInput
{
public:
    std::byte			pad0[0xC];
    bool				m_fTrackIRAvailable;
    bool				m_fMouseInitialized;
    bool				m_fMouseActive;
    std::byte			pad1[0x9E];
    bool				m_fCameraInThirdPerson;
    std::byte			pad2[0x2];
    Vector				m_vecCameraOffset;
    std::byte			pad3[0x38];
    CUserCmd*           m_pCommands;
    CVerifiedUserCmd*   m_pVerifiedCommands;

    inline CUserCmd* GetUserCmd(const int sequence_number);
    inline CUserCmd* GetUserCmd(int nSlot, int sequence_number);
    inline CVerifiedUserCmd* GetVerifiedCmd(const int sequence_number);
};
static_assert(sizeof(CInput) == 0xFC);


CUserCmd* CInput::GetUserCmd(const int sequence_number)
{
    return &m_pCommands[sequence_number % MULTIPLAYER_BACKUP];
}

CUserCmd* CInput::GetUserCmd(int nSlot, const int sequence_number)
{
    typedef CUserCmd* (__thiscall* GetUserCmd_t)(void*, int, int);
    return CallVFunction<GetUserCmd_t>(this, 8)(this, nSlot, sequence_number);
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(const int sequence_number)
{
    return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
}