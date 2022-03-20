//#include "WLED_COMM\WLED_SERIAL_COMM.h"
#include "WLED_SERIAL_COMM.h"

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
	WLED_port->print("{\"on\":");
	if (v)
	{
		WLED_port->print("true");
	}
	else
	{
		WLED_port->print("false");
	}
	WLED_port->print("}");
}

uint8_t WLED_COMM::GetBrightness()
{
	return state_bri;
}

void WLED_COMM::SetBrightness(uint8_t v)
{
	WLED_port->print("{\"bri\":");
	WLED_port->print(v);
	WLED_port->print("}");
}

uint8_t WLED_COMM::GetTransition()
{
	return state_transition;
}

void WLED_COMM::SetTransition(uint8_t v)
{
	WLED_port->print("{\"transition\":");
	WLED_port->print(v);
	WLED_port->print("}");
}

int32_t WLED_COMM::GetPreset()
{
	return state_ps;
}

void WLED_COMM::SetPreset(int32_t v)
{
	WLED_port->print("{\"ps\":");
	WLED_port->print(v);
	WLED_port->print("}");
}

int8_t WLED_COMM::GetPlaylist()
{
	return state_pl;
}

void WLED_COMM::SetPlaylist(int8_t v)
{
	WLED_port->print("{\"pl\":");
	WLED_port->print(v);
	WLED_port->print("}");
}

void WLED_COMM::RefreshStatus()
{
	JSON_ErrorState = false;
	JSON_Error_Description = "";

  	delay(500);
  	WLED_port->print("{\"v\":true}");

  DynamicJsonDocument doc(3072);

  DeserializationError error = deserializeJson(doc, *WLED_port);

  if (error) {
    JSON_ErrorState = true;
	JSON_Error_Description = error.c_str();
    return;
  }

  JsonObject state = doc["state"];
  state_on = state["on"];
  state_bri = state["bri"];
  state_transition = state["transition"];
  state_ps = state["ps"];
  state_pl = state["pl"];
}
