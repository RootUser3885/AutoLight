// Storage Container For WLED States

class WLED_State
{
private:
	
public:
	bool operator==(WLED_State &otherWLED_State);
	bool operator!=(WLED_State &otherWLED_State);

	bool state_on = false;
	int state_bri = 127;
	int state_transition = 0;
	int state_ps = 0;
	int state_pl = 0;

	bool UpToDate = false;
};

bool WLED_State::operator==(WLED_State& otherWLED_State)
{
	// Is this the right way to do this?
	if(	(state_on == otherWLED_State.state_on) &&
		(state_bri == otherWLED_State.state_bri) &&
		(state_transition == otherWLED_State.state_transition) &&
		(state_ps == otherWLED_State.state_ps) &&
		(state_pl == otherWLED_State.state_pl))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool WLED_State::operator!=(WLED_State& otherWLED_State)
{
	// Is this the right way to do this?
	if(	(state_on != otherWLED_State.state_on) ||
		(state_bri != otherWLED_State.state_bri) ||
		(state_transition != otherWLED_State.state_transition) ||
		(state_ps != otherWLED_State.state_ps) ||
		(state_pl != otherWLED_State.state_pl))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}