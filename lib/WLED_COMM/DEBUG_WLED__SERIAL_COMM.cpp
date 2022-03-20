#include "DEBUG_WLED_SERIAL_COMM.h"

void WLED_COMM::Begin(Stream &stream)
{
	WLED_port = &stream;
}

bool WLED_COMM::GetPower()
{
	return state_on;
}

void WLED_COMM::SetPower(bool v)
{
	delay(SIMULATED_SERIAL_DELAY);
	state_on = v;
}

uint8_t WLED_COMM::GetBrightness()
{
	return state_bri;
}

void WLED_COMM::SetBrightness(uint8_t v)
{
	delay(SIMULATED_SERIAL_DELAY);
	state_bri = v;
}

uint8_t WLED_COMM::GetTransition()
{
	return state_transition;
}

void WLED_COMM::SetTransition(uint8_t v)
{
	delay(SIMULATED_SERIAL_DELAY);
	state_transition = v;
}

int32_t WLED_COMM::GetPreset()
{
	return state_ps;
}

void WLED_COMM::SetPreset(int32_t v)
{
	delay(SIMULATED_SERIAL_DELAY);
	state_ps = v;
}

int8_t WLED_COMM::GetPlaylist()
{
	return state_pl;
}

void WLED_COMM::SetPlaylist(int8_t v)
{
	delay(SIMULATED_SERIAL_DELAY);
	state_ps = v;
}

void WLED_COMM::RefreshStatus()
{
	delay(SIMULATED_SERIAL_DELAY);
	delay(SIMULATED_SERIAL_DELAY);
	delay(SIMULATED_SERIAL_DELAY);
// 	JSON_ErrorState = false;
// 	JSON_Error_Description = "";

//   	delay(500);
//   	WLED_port->print("{\"v\":true}");

//   DynamicJsonDocument doc(3072);

//   DeserializationError error = deserializeJson(doc, *WLED_port);

//   if (error) {
//     JSON_ErrorState = true;
// 	JSON_Error_Description = error.c_str();
//     return;
//   }

//   JsonObject state = doc["state"];
//   state_on = state["on"];
//   state_bri = state["bri"];
//   state_transition = state["transition"];
//   state_ps = state["ps"];
//   state_pl = state["pl"];
}
