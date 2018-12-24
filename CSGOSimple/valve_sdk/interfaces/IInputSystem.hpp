#pragma once

class IInputSystem
{
public:
	void ResetInputState()
	{
		typedef void(__thiscall* tResetInputState)(void*);
		CallVFunction<tResetInputState>(this, 39)(this);
	}
};