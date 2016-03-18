#pragma once

#include <Windows.h>
#include <Psapi.h>

#include "SourceEngine/SDK.hpp"
#include "CSGOStructs.hpp"

class Utils {
public:
     static SourceEngine::Vector GetEntityBone(SourceEngine::IClientEntity* pEntity, SourceEngine::ECSPlayerBones Bone) {
          SourceEngine::matrix3x4_t boneMatrix[128];

          if(!pEntity->SetupBones(boneMatrix, 128, 0x00000100, SourceEngine::Interfaces::Engine()->GetLastTimeStamp()))
               return SourceEngine::Vector();

          SourceEngine::matrix3x4_t hitbox = boneMatrix[Bone];

          return SourceEngine::Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
     }

     static bool ScreenTransform(const SourceEngine::Vector& point, SourceEngine::Vector& screen) {
          const SourceEngine::VMatrix& w2sMatrix = SourceEngine::Interfaces::Engine()->WorldToScreenMatrix();
          screen.x = w2sMatrix.m[0][0] * point.x + w2sMatrix.m[0][1] * point.y + w2sMatrix.m[0][2] * point.z + w2sMatrix.m[0][3];
          screen.y = w2sMatrix.m[1][0] * point.x + w2sMatrix.m[1][1] * point.y + w2sMatrix.m[1][2] * point.z + w2sMatrix.m[1][3];
          screen.z = 0.0f;

          float w = w2sMatrix.m[3][0] * point.x + w2sMatrix.m[3][1] * point.y + w2sMatrix.m[3][2] * point.z + w2sMatrix.m[3][3];

          if(w < 0.001f) {
               screen.x *= 100000;
               screen.y *= 100000;
               return true;
          }

          float invw = 1.0f / w;
          screen.x *= invw;
          screen.y *= invw;

          return false;
     }

     static bool WorldToScreen(const SourceEngine::Vector &origin, SourceEngine::Vector &screen) {
          if(!ScreenTransform(origin, screen)) {
               int iScreenWidth, iScreenHeight;
               SourceEngine::Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

               screen.x = (iScreenWidth / 2.0f) + (screen.x * iScreenWidth) / 2;
               screen.y = (iScreenHeight / 2.0f) - (screen.y * iScreenHeight) / 2;

               return true;
          }
          return false;
     }

     static uint64_t FindSignature(const char* szModule, const char* szSignature) {
          //CREDITS: learn_more
          #define INRANGE(x,a,b)  (x >= a && x <= b) 
          #define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
          #define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

          MODULEINFO modInfo;
          GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(szModule), &modInfo, sizeof(MODULEINFO));
          DWORD startAddress = (DWORD)modInfo.lpBaseOfDll;
          DWORD endAddress = startAddress + modInfo.SizeOfImage;
          const char* pat = szSignature;
          DWORD firstMatch = 0;
          for(DWORD pCur = startAddress; pCur < endAddress; pCur++) {
               if(!*pat) return firstMatch;
               if(*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat)) {
                    if(!firstMatch) firstMatch = pCur;
                    if(!pat[2]) return firstMatch;
                    if(*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?') pat += 3;
                    else pat += 2;    //one ?
               } else {
                    pat = szSignature;
                    firstMatch = 0;
               }
          }
          return NULL;
     }

     static bool Clamp(SourceEngine::QAngle &angles) {
          SourceEngine::QAngle a = angles;
          Normalize(a);
          ClampAngles(a);

          if(isnan(a.x) || isinf(a.x) ||
               isnan(a.y) || isinf(a.y) ||
               isnan(a.z) || isinf(a.z)) {
               return false;
          } else {
               angles = a;
               return true;
          }
     }
private:
     static void Normalize(SourceEngine::QAngle& angle) {
          while(angle.x > 89.0f) {
               angle.x -= 180.f;
          }
          while(angle.x < -89.0f) {
               angle.x += 180.f;
          }
          while(angle.y > 180.f) {
               angle.y -= 360.f;
          }
          while(angle.y < -180.f) {
               angle.y += 360.f;
          }
     }

     static void ClampAngles(SourceEngine::QAngle &angles) {
          if(angles.y > 180.0f)
               angles.y = 180.0f;
          else if(angles.y < -180.0f)
               angles.y = -180.0f;

          if(angles.x > 89.0f)
               angles.x = 89.0f;
          else if(angles.x < -89.0f)
               angles.x = -89.0f;

          angles.z = 0;
     }
};