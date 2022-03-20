#include "mainHeader.h"

void setup()
{
#ifdef MAIN_DEBUG
  while (!MAIN_DEBUG)
    ; // Wait for Serial connection if debugging

  MAIN_DEBUG.begin(MAIN_DEBUG_BAUD);
  MAIN_DEBUG.println("Startup");
  TRACE();
#endif

  pinMode(LED_BUILTIN, OUTPUT);
  ELM_SERIAL_PORT.begin(ELM_BAUD);   // ELM Serial
  WLED_SERIAL_PORT.begin(WLED_BAUD); // WLED Serial

  ELM_OBD.begin(ELM_SERIAL_PORT, false, ELM_CONNECT_TIMEOUT);
  LED.Begin(WLED_SERIAL_PORT);
  LED.SetPower(true);
  LED.SetPreset(PS_DEFAULT_EFFECT);

  VehicleStatus.Begin(ELM_OBD);
  // Setup Input Pins
  pinMode(LIGHT_BRAKE, INPUT_PULLDOWN);
  pinMode(LIGHT_L_BLINKER, INPUT_PULLDOWN);
  pinMode(LIGHT_R_BLINKER, INPUT_PULLDOWN);

  // Set Output Pins
  pinMode(WLED_POWER, OUTPUT);
  digitalWrite(WLED_POWER, HIGH);

  // Setup interrups for important inputs
  attachInterrupt(digitalPinToInterrupt(LIGHT_BRAKE), HandleExternalInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LIGHT_L_BLINKER), HandleExternalInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LIGHT_R_BLINKER), HandleExternalInterrupt, CHANGE);

  delay(WLED_STARTUP_TIME); // Delay to give WLED time to Start
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
  static WLED_State desiredStateLED;
  static WLED_State currentStateLED;
  static uint16_t failedWrites;

  // HandleExternalInterrupt();
  currentStateLED = RefreshStatusLEDs(); // Updates the stored LED state

  if (currentStateLED.UpToDate && VehicleStatus.SafetyCritical())
  {
    if (currentStateLED.state_ps != desiredStateLED.state_ps) // WLED preset doesnt match desired preset
    {
      if (desiredStateLED.state_ps == 0) // Cant have a preset of 0
      {
        desiredStateLED.state_ps = PS_DEFAULT_EFFECT;
      }
      WriteNewStateLED(desiredStateLED, currentStateLED); // Attempt to write settings
      currentStateLED.UpToDate = false;                   // current and desired state no longer match
      failedWrites++;                                     // Keep count of failed writes
      // #ifdef MAIN_DEBUG
      // DUMP(failedWrites);
      // #endif
    }
    else
    {
      failedWrites = 0;
    }
    if (failedWrites >= WLED_SETPRESET_RETRY_COUNT)
    {
      LED.SetPower(false);
      digitalWrite(WLED_POWER, LOW);
      // #ifdef MAIN_DEBUG
      // MAIN_DEBUG.println("Preset Mismatch");
      // BREAK();
      // #endif
    }
  }

  if (VehicleStatus.StateChanged()) // If the interrupt fired, this will run
  {
    // #ifdef MAIN_DEBUG
    // DUMP(desiredStateLED.state_ps);
    // DUMP(currentStateLED.state_ps);
    // #endif
    desiredStateLED = DecidePresetLEDs(&VehicleStatus, currentStateLED); // Set the LEDs to the desired preset based on the state of the vehicle
    WriteNewStateLED(desiredStateLED, currentStateLED);
    delay(100);
  }

#ifdef MAIN_DEBUG
  HandleDebugInput();
#endif
}

void HandleExternalInterrupt()
{
  // Save the current pin state and get out
  VehicleStatus.SetBrakeState(digitalRead(LIGHT_BRAKE));
  VehicleStatus.SetDriversBlinkerState(digitalRead(LIGHT_L_BLINKER));
  VehicleStatus.SetPassBlinkerState(digitalRead(LIGHT_R_BLINKER));
}

WLED_State RefreshStatusLEDs()
{
  static uint32_t lastRefreshTime = 0; // Store the last time we updated the LEDs

  WLED_State curStateLED; // Current LEDs state

  curStateLED.UpToDate = false; // LED State no longer should match our desired state

  // Check if its been long enough to refresh
  if (lastRefreshTime + WLED_REFRESH_WAIT_TIME <= millis()) //(curStateLED != prevStateLED && (lastRefreshTime + WLED_REFRESH_WAIT_TIME <= millis()))
  {
#ifdef MAIN_DEBUG
    TRACE();
#endif
    LED.RefreshStatus();         // Read LEDs status from the controller
    lastRefreshTime = millis();  // Store this run time for next time
    curStateLED.UpToDate = true; // LED State is now current
  }

  curStateLED.state_bri = LED.GetBrightness();
  curStateLED.state_on = LED.GetPower();
  curStateLED.state_pl = LED.GetPlaylist();
  curStateLED.state_ps = LED.GetPreset();
  curStateLED.state_transition = LED.GetTransition();

  return curStateLED;
}

WLED_State DecidePresetLEDs(VehicleState *CurVehicleState, WLED_State curState)
{
#ifdef MAIN_DEBUG
  TRACE();
#endif
  // This might look like a massive if/else statement,
  // Thats because it is('nt anymore*↴)
  // Update: Not anymore, hopefully it works!

  // All this does is check the state of the brake and blinker lights and decides which effect to show

  // Error from passing in a copy of VehicleState
  // I need to pass in a pointer to VehicleState instead,
  // Once I figure out how pointers work in C++. The VehicleStatus.StateChanged wasn't updating because of this.
  // For now Im just using the glogal VehicleState instance instead of CurVehicleState.
  // I know its not right, its just what I know to do atm because I dont have internet access atm.
  // Fix ASAP
  // ** Update: Fixed, Pointers aren't that bad. Its just remembering the syntax.

  // This also needs to be changed to some sort of binary mask system to clean this up.
  // Update: Done, but I'm keeping the old code here for a bit until i know this works well.
  u_int8_t bitVehicleState = 0b00000000;
  bitVehicleState += CurVehicleState->GetBrakeState() << 0;
  bitVehicleState += CurVehicleState->GetDriversBlinkerState() << 1;
  bitVehicleState += CurVehicleState->GetPassBlinkerState() << 2;
  // bitVehicleState += CurVehicleState->GetWhatever() << 3;
  // bitVehicleState += CurVehicleState->GetWhatever() << 4;
  // bitVehicleState += CurVehicleState->GetWhatever() << 5;
  // bitVehicleState += CurVehicleState->GetWhatever() << 6;
  // bitVehicleState += CurVehicleState->GetWhatever() << 7;
  // ^ Future Proofing

  curState.state_ps = bitVehicleState + 1; // We have to offset by one because there is no preset 0

#ifdef MAIN_DEBUG
  DUMP(bitVehicleState);
  MAIN_DEBUG.print("main.cpp:164: bitVehicleState = 0b");
  MAIN_DEBUG.println(bitVehicleState, BIN);
  DUMP(curState.state_ps);
  BREAK();
#endif

  // if (!CurVehicleState->GetPassBlinkerState() && !CurVehicleState->GetDriversBlinkerState() && !CurVehicleState->GetBrakeState())
  // { // Default Effect
  //   curState.state_ps = PS_DEFAULT_EFFECT;
  // }
  // else if (!CurVehicleState->GetPassBlinkerState() && !CurVehicleState->GetDriversBlinkerState() && CurVehicleState->GetBrakeState())
  // { // Drivers Blinker
  //   curState.state_ps = PS_BRAKE_EFFECT;
  // }
  // else if (!CurVehicleState->GetPassBlinkerState() && CurVehicleState->GetDriversBlinkerState() && !CurVehicleState->GetBrakeState())
  // { // Drivers Blinker
  //   curState.state_ps = PS_DRIVERS_BLINKER_EFFECT;
  // }
  // else if (!CurVehicleState->GetPassBlinkerState() && CurVehicleState->GetDriversBlinkerState() && CurVehicleState->GetBrakeState())
  // { // Drivers Blinker and Brake
  //   curState.state_ps = PS_DRIVERS_BLINKER_BRAKE_EFFECT;
  // }
  // else if (CurVehicleState->GetPassBlinkerState() && !CurVehicleState->GetDriversBlinkerState() && !CurVehicleState->GetBrakeState())
  // { // Passangers Blinker
  //   curState.state_ps = PS_PASS_BLINKER_EFFECT;
  // }
  // else if (CurVehicleState->GetPassBlinkerState() && !CurVehicleState->GetDriversBlinkerState() && CurVehicleState->GetBrakeState())
  // { // Passangers Blinker and Brake
  //   curState.state_ps = PS_PASS_BLINKER_BRAKE_EFFECT;
  // }
  // else if (CurVehicleState->GetPassBlinkerState() && CurVehicleState->GetDriversBlinkerState() && !CurVehicleState->GetBrakeState())
  // { // Hazard Lights
  //   curState.state_ps = PS_HAZARD_LIGHTS_EFFECT;
  // }
  // else if (CurVehicleState->GetPassBlinkerState() && CurVehicleState->GetDriversBlinkerState() && CurVehicleState->GetBrakeState())
  // { // Hazard Lights and Brakes (Just Hazard Lights)
  //   curState.state_ps = PS_HAZARD_LIGHTS_EFFECT;
  // }

  return curState;
}

void WriteNewStateLED(WLED_State desState, WLED_State curState)
{
#ifdef MAIN_DEBUG
  TRACE();
#endif

  if (desState.state_ps != curState.state_ps)
  {
    LED.SetPreset(desState.state_ps);
  }
  else
  {
    if (desState.state_bri != curState.state_bri)
    {
      LED.SetBrightness(desState.state_bri);
    }
    if (desState.state_on != curState.state_on)
    {
      LED.SetPower(desState.state_on);
    }
    if (desState.state_pl != curState.state_pl)
    {
      LED.SetPlaylist(desState.state_pl);
    }
    if (desState.state_transition != curState.state_transition)
    {
      LED.SetTransition(desState.state_transition);
    }
  }
}

// Beyond here is just a debugger serial interface ***************************************

#ifdef MAIN_DEBUG
String WaitOnGetSerialInput()
// This function just waits for serial input and removes extra char's
{
  while (true)
  {
    MAIN_DEBUG.setTimeout(30000); // Give the user time to input data

    if (MAIN_DEBUG.available()) // only read if theres something to read
    {
      String rawInputString = MAIN_DEBUG.readStringUntil('\n'); // Wait for new line char

      // Remove the last char from the string (\n) and return
      if (rawInputString.length() > 1)
      {
        String inputString = rawInputString.substring(0, rawInputString.length() - 1);
        return inputString;
      }
    }
  }
}

void HandleDebugInput()
{
  if (MAIN_DEBUG.available()) // Check it theres any serial data to read
  {
    // Instructions
    MAIN_DEBUG.println();
    MAIN_DEBUG.println("-- WLED Car Serial Interface --");
    MAIN_DEBUG.println("- Type 'vehiclestate' to view current state of the vehicle inputs");
    MAIN_DEBUG.println("- Type 'override' to manually override some vehicle states");
    MAIN_DEBUG.println("- Type 'wled' to view WLED status");
    MAIN_DEBUG.println("- Type 'exit' to exit debug interface and return to the program");

    // Clear the buffer
    while (MAIN_DEBUG.available())
    {
      MAIN_DEBUG.read();
    }

    // Sit in a loop until we want to exit
    bool exitFlag = false;
    while (!exitFlag)
    {
      String inputString = WaitOnGetSerialInput();

      // Input 'list'
      if (inputString == "vehiclestate")
      {
        MAIN_DEBUG.println();
        MAIN_DEBUG.println("- Current Vehicle State -");
        MAIN_DEBUG.print("OBD Status: ");
        MAIN_DEBUG.println(ELM_OBD.connected);
        MAIN_DEBUG.print("Safety Critical State: ");
        MAIN_DEBUG.println(VehicleStatus.SafetyCritical());
        MAIN_DEBUG.print("State Changed: ");
        MAIN_DEBUG.println(VehicleStatus.StateChanged());
        MAIN_DEBUG.print("Brake Light State: ");
        MAIN_DEBUG.println(VehicleStatus.GetBrakeState());
        MAIN_DEBUG.print("Left Blinker State: ");
        MAIN_DEBUG.println(VehicleStatus.GetDriversBlinkerState());
        MAIN_DEBUG.print("Right Blinker State: ");
        MAIN_DEBUG.println(VehicleStatus.GetPassBlinkerState());
      }
      // WLED Status menu
      else if (inputString == "wled")
      {
        MAIN_DEBUG.println("- Current WLED State -");
        MAIN_DEBUG.print("LED Power State: ");
        MAIN_DEBUG.println(LED.GetPower());
        MAIN_DEBUG.print("LED Preset State: ");
        MAIN_DEBUG.println(LED.GetPreset());
        MAIN_DEBUG.print("LED Playlist State: ");
        MAIN_DEBUG.println(LED.GetPlaylist());
        MAIN_DEBUG.print("LED Brightness State: ");
        MAIN_DEBUG.println(LED.GetBrightness());
        MAIN_DEBUG.print("LED Transition State: ");
        MAIN_DEBUG.println(LED.GetTransition());
      }
      // Override Menu
      else if (inputString == "override")
      {
        MAIN_DEBUG.println();
        MAIN_DEBUG.println("- Select an input to override -");
        MAIN_DEBUG.println("'brake'");
        MAIN_DEBUG.println("'lblinker'");
        MAIN_DEBUG.println("'rblinker'");
        MAIN_DEBUG.println();
        MAIN_DEBUG.print("Current Brake State: ");
        MAIN_DEBUG.println(VehicleStatus.GetBrakeState());
        MAIN_DEBUG.print("Current Left Blinker State: ");
        MAIN_DEBUG.println(VehicleStatus.GetDriversBlinkerState());
        MAIN_DEBUG.print("Current Right Blinker State: ");
        MAIN_DEBUG.println(VehicleStatus.GetPassBlinkerState());

        bool overrideExitFlag = false;
        while (!overrideExitFlag)
        {
          String overrideInputString = WaitOnGetSerialInput();
          // Brake Override
          if (overrideInputString == "brake")
          {
            MAIN_DEBUG.println("Enter Value to override to: 'true' or 'false'");
            String overrideString = WaitOnGetSerialInput();
            if (overrideString == "true")
            {
              MAIN_DEBUG.println("Brake set to true");
              VehicleStatus.SetBrakeState(true);
              overrideExitFlag = true;
            }
            else if (overrideString == "false")
            {
              MAIN_DEBUG.println("Brake set to false");
              VehicleStatus.SetBrakeState(false);
              overrideExitFlag = true;
            }
            else
            {
              MAIN_DEBUG.println("Invalid input");
            }
          }
          // Left Blinker Override
          else if (overrideInputString == "lblinker")
          {
            MAIN_DEBUG.println("Enter Value to override to: 'true' or 'false'");
            String overrideString = WaitOnGetSerialInput();
            if (overrideString == "true")
            {
              MAIN_DEBUG.println("Left Blinker set to true");
              VehicleStatus.SetDriversBlinkerState(true);
              overrideExitFlag = true;
            }
            else if (overrideString == "false")
            {
              MAIN_DEBUG.println("Left Blinker set to false");
              VehicleStatus.SetDriversBlinkerState(false);
              overrideExitFlag = true;
            }
            else if (overrideInputString == "exit")
            {
              overrideExitFlag = true;
            }
            else
            {
              MAIN_DEBUG.println("Invalid input");
              overrideExitFlag = true;
            }
          }
          // Right Blinker Override
          else if (overrideInputString == "rblinker")
          {
            MAIN_DEBUG.println("Enter Value to override to: 'true' or 'false'");
            String overrideString = WaitOnGetSerialInput();
            if (overrideString == "true")
            {
              MAIN_DEBUG.println("Right Blinker set to true");
              VehicleStatus.SetPassBlinkerState(true);
              overrideExitFlag = true;
            }
            else if (overrideString == "false")
            {
              MAIN_DEBUG.println("Right Blinker set to false");
              VehicleStatus.SetPassBlinkerState(false);
              overrideExitFlag = true;
            }
            else
            {
              MAIN_DEBUG.println("Invalid input");
            }
          }
        }
        exitFlag = true;
      }
      // Input 'exit'
      else if (inputString == "exit")
      {
        exitFlag = true; // leave loop
      }
    }
    MAIN_DEBUG.println("Exit");
  }
}
#endif