#pragma once

#include "../Misc/CUserCmd.hpp"

#define MULTIPLAYER_BACKUP 150

class bf_write;
class bf_read;

class CInput
{
public:
	char                pad_0x00[0x0C];  
	bool                m_trackir_available;  
	bool                m_mouse_initiated;  
	bool                m_mouse_active;  
	bool                m_fJoystickAdvancedInit;       
	char                pad_0x08[0x2C];                
	void*               m_pKeys;                       
	char                pad_0x38[0x6C];                
	bool                m_fCameraInterceptingMouse;    
	bool                m_fCameraInThirdPerson;        
	bool                m_fCameraMovingWithMouse;      
	Vector		    m_vecCameraOffset;             
	bool                m_fCameraDistanceMove;         
	int                 m_nCameraOldX;                 
	int                 m_nCameraOldY;                 
	int                 m_nCameraX;                    
	int                 m_nCameraY;                    
	bool                m_CameraIsOrthographic;        
	Vector              m_angPreviousViewAngles;       
	Vector              m_angPreviousViewAnglesTilt;   
	float               m_flLastForwardMove;           
	int                 m_nClearInputState;            
	char                pad_0xE4[0x8];                 
	CUserCmd*           m_pCommands;                   
	CVerifiedUserCmd*   m_pVerifiedCommands;           

	inline CUserCmd* GetUserCmd(int sequence_number);
	inline CUserCmd * GetUserCmd(int nSlot, int sequence_number);
	inline CVerifiedUserCmd* GetVerifiedCmd(int sequence_number);
};

CUserCmd* CInput::GetUserCmd(int sequence_number)
{
	using OriginalFn = CUserCmd * (__thiscall *)(void *, int, int);
	return CallVFunction<OriginalFn>(this, 8)(this, 0, sequence_number);
}

CUserCmd *CInput::GetUserCmd(int nSlot, int sequence_number)
{
	typedef CUserCmd*(__thiscall *GetUserCmd_t)(void*, int, int);
	return CallVFunction<GetUserCmd_t>(this, 8)(this, nSlot, sequence_number);
}

CVerifiedUserCmd* CInput::GetVerifiedCmd(int sequence_number)
{
	auto verifiedCommands = *(CVerifiedUserCmd **)(reinterpret_cast<uint32_t>(this) + 0xF8);
	return &verifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
}
