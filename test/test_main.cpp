#include <unity.h>
#include "mainHeader.h"

// NOTE!!!!!!!!!
// You MUST save this before running pio test, otherwiese the tests do not update!!!

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_compare_equal_WLED_States(void)
{
    const int ps = 2;
    const bool on = true;
    const uint8_t bri = 127;
    const int8_t pl = 2;
    const int8_t tr = 1;

    WLED_State LED_StateA;
    WLED_State LED_StateB;
    
    LED_StateA.state_on = on;
    LED_StateA.state_bri = bri;
    LED_StateA.state_ps = ps;
    LED_StateA.state_pl = pl;
    LED_StateA.state_transition = tr;

    LED_StateB.state_on = on;
    LED_StateB.state_bri = bri;
    LED_StateB.state_ps = ps;
    LED_StateB.state_pl = pl;
    LED_StateB.state_transition = tr;
    
    TEST_ASSERT_EQUAL(true, LED_StateA == LED_StateB);
}

void test_compare_notEqual_WLED_States(void)
{
    const int ps = 2;
    const bool on = true;
    const uint8_t bri = 127;
    const int8_t pl = 2;
    const int8_t tr = 1;

    WLED_State LED_StateA;
    WLED_State LED_StateB;
    
    LED_StateA.state_on = on;
    LED_StateA.state_bri = bri;
    LED_StateA.state_ps = ps;
    LED_StateA.state_pl = pl;
    LED_StateA.state_transition = tr;

    LED_StateB.state_on = 0;
    LED_StateB.state_bri = bri;
    LED_StateB.state_ps = ps;
    LED_StateB.state_pl = pl;
    LED_StateB.state_transition = tr;
    
    TEST_ASSERT_EQUAL(true, LED_StateA != LED_StateB);
}

void test_select_desired_status(void)
{
    WLED_State testLEDState;
    VehicleState testVehicleState;

    testVehicleState.SetBrakeState(false);
    testVehicleState.SetDriversBlinkerState(true);
    testVehicleState.SetPassBlinkerState(false);

    testLEDState = DecidePresetLEDs(testVehicleState, testLEDState);

    bool result = testLEDState.state_ps == PS_DRIVERS_BLINKER_EFFECT;

    TEST_ASSERT_EQUAL(true, result);
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    UNITY_BEGIN();    // IMPORTANT LINE!

    WLED_SERIAL_PORT.begin(WLED_BAUD);
    LED.Begin(WLED_SERIAL_PORT);

    RUN_TEST(test_compare_equal_WLED_States);
    RUN_TEST(test_compare_notEqual_WLED_States);
    RUN_TEST(test_select_desired_status);

    UNITY_END(); // stop unit testing
}

void loop() {
    
}