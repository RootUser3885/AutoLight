#include <Arduino.h>
#include <ArduinoJson.h>

class WLED_COMM
{
public:

	Stream* WLED_port;

	bool JSON_ErrorState = false;
	String JSON_Error_Description;

	void Begin(Stream &Stream);

	void RefreshStatus();
	
	bool GetPower();
	void SetPower(bool v);

    uint8_t GetBrightness();
	void SetBrightness(uint8_t v);

	uint8_t GetTransition();
	void SetTransition(uint8_t v);
	
	int32_t GetPreset();
	void SetPreset(int32_t v);

	int8_t GetPlaylist();
	void SetPlaylist(int8_t v);

protected:

	// Nothing to see here

private:
	void WriteParam(String v);

	JsonObject state;
	bool state_on;
	int state_bri;
	int state_transition;
	int state_ps;
	int state_pl;
};