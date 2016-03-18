#pragma once

#include "QAngle.hpp"
#include "CRC.hpp"

#define IN_ATTACK        (1 << 0)
#define IN_JUMP               (1 << 1)
#define IN_DUCK               (1 << 2)
#define IN_FORWARD       (1 << 3)
#define IN_BACK               (1 << 4)
#define IN_USE           (1 << 5)
#define IN_CANCEL        (1 << 6)
#define IN_LEFT               (1 << 7)
#define IN_RIGHT         (1 << 8)
#define IN_MOVELEFT      (1 << 9)
#define IN_MOVERIGHT     (1 << 10)
#define IN_ATTACK2       (1 << 11)
#define IN_RUN           (1 << 12)
#define IN_RELOAD        (1 << 13)
#define IN_ALT1               (1 << 14)
#define IN_ALT2               (1 << 15)
#define IN_SCORE         (1 << 16)   // Used by client.dll for when scoreboard is held down
#define IN_SPEED         (1 << 17) // Player is holding the speed key
#define IN_WALK               (1 << 18) // Player holding walk key
#define IN_ZOOM               (1 << 19) // Zoom key for HUD zoom
#define IN_WEAPON1       (1 << 20) // weapon defines these bits
#define IN_WEAPON2       (1 << 21) // weapon defines these bits
#define IN_BULLRUSH      (1 << 22)
#define IN_GRENADE1      (1 << 23) // grenade 1
#define IN_GRENADE2      (1 << 24) // grenade 2
#define   IN_LOOKSPIN         (1 << 25)

namespace SourceEngine {
     class bf_read;
     class bf_write;

     class CUserCmd {
     public:
          virtual ~CUserCmd() {};

          CRC32_t GetChecksum(void) const {
               CRC32_t crc;
               CRC32_Init(&crc);

               CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
               CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
               CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
               CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
               CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
               CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
               CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
               CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
               CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
               CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
               CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
               CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
               CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
               CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));

               CRC32_Final(&crc);
               return crc;
          }

          int       command_number;     // 0x04
          int       tick_count;         // 0x08
          QAngle    viewangles;         // 0x0C
          Vector    aimdirection;       // 0x18
          float     forwardmove;        // 0x24
          float     sidemove;           // 0x28
          float     upmove;             // 0x2C
          int       buttons;            // 0x30
          byte      impulse;            // 0x34
          int       weaponselect;       // 0x38
          int       weaponsubtype;      // 0x3C
          int       random_seed;        // 0x40
          short     mousedx;            // 0x44
          short     mousedy;            // 0x46
          bool      hasbeenpredicted;   // 0x48
          char      pad_0x4C[0x18];     // 0x4C

     };

     class CVerifiedUserCmd {
     public:
          CUserCmd  m_cmd;
          CRC32_t   m_crc;
     };

     class CInput {
     public:
          void*               pvftable;                     //0x00
          bool                m_fTrackIRAvailable;          //0x04
          bool                m_fMouseInitialized;          //0x05
          bool                m_fMouseActive;               //0x06
          bool                m_fJoystickAdvancedInit;      //0x07
          char                pad_0x08[0x2C];               //0x08
          void*               m_pKeys;                      //0x34
          char                pad_0x38[0x64];               //0x38
          bool                m_fCameraInterceptingMouse;   //0x9C
          bool                m_fCameraInThirdPerson;       //0x9D
          bool                m_fCameraMovingWithMouse;     //0x9E
          Vector              m_vecCameraOffset;            //0xA0
          bool                m_fCameraDistanceMove;        //0xAC
          int                 m_nCameraOldX;                //0xB0
          int                 m_nCameraOldY;                //0xB4
          int                 m_nCameraX;                   //0xB8
          int                 m_nCameraY;                   //0xBC
          bool                m_CameraIsOrthographic;       //0xC0
          QAngle              m_angPreviousViewAngles;      //0xC4
          QAngle              m_angPreviousViewAnglesTilt;  //0xD0
          float               m_flLastForwardMove;          //0xDC
          int                 m_nClearInputState;           //0xE0
          char                pad_0xE4[0x8];                //0xE4
          CUserCmd*           m_pCommands;                  //0xEC
          CVerifiedUserCmd*   m_pVerifiedCommands;          //0xF0
     };
}