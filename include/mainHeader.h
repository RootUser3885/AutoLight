#include "Parameters.h"
#include <Arduino.h>
#ifdef USE_DEBUG_WLED_COMM
#include "DEBUG_WLED_SERIAL_COMM.h"
#else
#include "WLED_SERIAL_COMM.h"
#endif
#include "WLED_State.h"
#include "VehicleState.h"
#include "ELMduino.h"
#include "ArduinoTrace.h"

VehicleState VehicleStatus;
ELM327 ELM_OBD;
WLED_COMM LED;

void HandleExternalInterrupt();

WLED_State RefreshStatusLEDs();
WLED_State DecidePresetLEDs(VehicleState* CurVehicleState, WLED_State curState);
void WriteNewStateLED(WLED_State desState, WLED_State curState);
bool CheckIfPresetSaved(WLED_State desState, WLED_State curState);

// Debug Functions
#ifdef MAIN_DEBUG
String WaitOnGetSerialInput();
void HandleDebugInput();
#endif