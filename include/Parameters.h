// Debugging
#define MAIN_DEBUG 		Serial
#define MAIN_DEBUG_BAUD 115200
#define USE_DEBUG_WLED_COMM

#define ELM_SERIAL_PORT 	Serial1
#define ELM_BAUD 			38400
#define ELM_CONNECT_TIMEOUT 500

#define WLED_SERIAL_PORT 			Serial2
#define WLED_BAUD 					115200
#define WLED_REFRESH_WAIT_TIME 		250 //100
#define WLED_SETPRESET_RETRY_COUNT 	5
#define WLED_STARTUP_TIME 			1000

// Define LED Preset Effects
#define PS_DEFAULT_EFFECT 				1
#define PS_BRAKE_EFFECT 				2
#define PS_DRIVERS_BLINKER_EFFECT 		3
#define PS_DRIVERS_BLINKER_BRAKE_EFFECT 4
#define PS_PASS_BLINKER_EFFECT 			5
#define PS_PASS_BLINKER_BRAKE_EFFECT 	6
#define PS_HAZARD_LIGHTS_EFFECT 		7
#define PS_HAZARD_LIGHTS_BRAKE_EFFECT 	8

// Define I/O Pins for light states
#define WLED_POWER 			14 // Turns off LEDs in case of a comm issue
#define LIGHT_BRAKE 		15
#define LIGHT_L_BLINKER 	16
#define LIGHT_R_BLINKER 	17
#define LIGHT_HEADLIGHTS 	18
#define LIGHT_DRLS 			19
#define LIGHT_PARKING 		20
