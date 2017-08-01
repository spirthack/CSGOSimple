#pragma once

#include <cstdint>

// Created with ReClass.NET by KN4CK3R
#pragma pack(push, 1)
class INetChannel
{
public:
    char pad_0000[20];           //0x0000
    bool m_bProcessingMessages;  //0x0014
    bool m_bShouldDelete;        //0x0015
    char pad_0016[2];            //0x0016
    int32_t m_nOutSequenceNr;    //0x0018 last send outgoing sequence number
    int32_t m_nInSequenceNr;     //0x001C last received incoming sequnec number
    int32_t m_nOutSequenceNrAck; //0x0020 last received acknowledge outgoing sequnce number
    int32_t m_nOutReliableState; //0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
    int32_t m_nInReliableState;  //0x0028 state of incoming reliable data
    int32_t m_nChokedPackets;    //0x002C number of choked packets
    char pad_0030[1044];         //0x0030
}; //Size: 0x0444

class CClockDriftMgr
{
public:
    float m_ClockOffsets[17];   //0x0000
    uint32_t m_iCurClockOffset; //0x0044
    uint32_t m_nServerTick;     //0x0048
    uint32_t m_nClientTick;     //0x004C
}; //Size: 0x0050

class CClientState
{
public:
    void ForceFullUpdate()
    {
        m_nDeltaTick = -1;
    }

    char pad_0000[156];             //0x0000
    uint32_t m_NetChannel;          //0x009C
    uint32_t m_nChallengeNr;        //0x00A0
    char pad_00A4[100];             //0x00A4
    uint32_t m_nSignonState;        //0x0108
    char pad_010C[8];               //0x010C
    float m_flNextCmdTime;          //0x0114
    uint32_t m_nServerCount;        //0x0118
    uint32_t m_nCurrentSequence;    //0x011C
    char pad_0120[8];               //0x0120
    CClockDriftMgr m_ClockDriftMgr; //0x0128
    uint32_t m_nDeltaTick;          //0x0178
    bool m_bPaused;                 //0x017C
    char pad_017D[3];               //0x017D
    uint32_t m_nViewEntity;         //0x0180
    uint32_t m_nPlayerSlot;         //0x0184
    char m_szLevelName[260];        //0x0188
    char m_szLevelNameShort[40];    //0x028C
    char m_szGroupName[40];         //0x02B4
    char pad_02DC[52];              //0x02DC
    uint32_t m_nMaxClients;         //0x0310
    char pad_0314[18820];           //0x0314
    float m_flLastServerTickTime;   //0x4C98
    bool insimulation;              //0x4C9C
    char pad_4C9D[3];               //0x4C9D
    uint32_t oldtickcount;          //0x4CA0
    float m_tickRemainder;          //0x4CA4
    float m_frameTime;              //0x4CA8
    uint32_t lastoutgoingcommand;   //0x4CAC
    uint32_t chokedcommands;        //0x4CB0
    uint32_t last_command_ack;      //0x4CB4
    uint32_t command_ack;           //0x4CB8
    uint32_t m_nSoundSequence;      //0x4CBC
    char pad_4CC0[80];              //0x4CC0
    Vector viewangles;              //0x4D10
}; //Size: 0x4D1C

#pragma pack(pop)

static_assert(FIELD_OFFSET(CClientState, viewangles) == 0x4D10, "Wrong struct offset");