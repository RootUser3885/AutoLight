// This class just stores the current state of each light and other vehicular elements
#include "ELMduino.h"

class VehicleState
{
private:
	bool _SafetyCritical = true; // Store if vehicle must act in a safe manner, Assume true unless proven otherwise
	bool _L_Blinker_State = false;
	bool _R_Blinker_State = false;
	bool _Brake_State = false;
	bool _StateChanged = false;
	ELM327* _CarOBD;
public:
	void Begin(ELM327 &CAROBD);

	bool SafetyCritical();
	bool StateChanged();
	
	bool GetDriversBlinkerState();
	void SetDriversBlinkerState(bool state);

	bool GetPassBlinkerState();
	void SetPassBlinkerState(bool state);

	bool GetBrakeState();
	void SetBrakeState(bool state);
};
