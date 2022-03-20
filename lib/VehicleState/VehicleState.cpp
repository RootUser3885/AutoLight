#include "VehicleState.h"

void VehicleState::Begin(ELM327 &CAROBD)
{
	_CarOBD = &CAROBD;
}

bool VehicleState::SafetyCritical()
{
	// Check OBD and other input and decide if were on the road

	// If the speed is under 10 mph and the engine is off, enable custom effects
	if(_CarOBD->connected && (_CarOBD->mph() < 10 && _CarOBD->rpm() < 200))
	{
		_SafetyCritical = false;
	}
	else
	{
		_SafetyCritical = true;
	}

	return _SafetyCritical;
}

 bool VehicleState::GetDriversBlinkerState()
{
	_StateChanged = false;
	return _L_Blinker_State;
}
void VehicleState::SetDriversBlinkerState(bool state)
{
	if (state != _L_Blinker_State)
	{
		_L_Blinker_State = state;
		_StateChanged = true;
	}
	
}

bool VehicleState::GetPassBlinkerState()
{
	_StateChanged = false;
	return _R_Blinker_State;
}
void VehicleState::SetPassBlinkerState(bool state)
{
	if (state != _R_Blinker_State)
	{
		_R_Blinker_State = state;
		_StateChanged = true;
	}
	
}

bool VehicleState::GetBrakeState()
{
	_StateChanged = false;
	return _Brake_State;
}
void VehicleState::SetBrakeState(bool state)
{
	if (state != _Brake_State)
	{
		_Brake_State = state;
		_StateChanged = true;
	}
	
}

bool VehicleState::StateChanged()
{
	return _StateChanged;
}