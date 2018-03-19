#ifndef INTERVALCHECKTIMER
#define INTERVALCHECKTIMER


#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif


class IntervalCheckTimer{
protected:	
	// 
	unsigned long int last_time_in_checked, min_inter_check_ms;
	static const unsigned long int  default_min_inter_check_ms=100;
public:
	IntervalCheckTimer()
	{
		last_time_in_checked=0;
		min_inter_check_ms=default_min_inter_check_ms;
	}
	void setInterCheck(int inp_inter_check_ms)
	{
		// avoid mistaken negative input
		inp_inter_check_ms=abs(inp_inter_check_ms);
		min_inter_check_ms=inp_inter_check_ms;
	}
	void setInterCheck(unsigned long int inp_inter_check_ms)
	{
		min_inter_check_ms=inp_inter_check_ms;
	}
	int getInterCheck()
	{
		return min_inter_check_ms;
	}
	boolean isMinChekTimeElapsed()
	{
		unsigned long int curr_time=millis();
		
		/*Serial.println("");
		Serial.print("last check time ");
		Serial.print(last_time_in_checked);
		Serial.println("");
		Serial.print("curr_time time ");
		Serial.print(curr_time);
		Serial.println("");
		Serial.print("min_inter_check_ms ");
		Serial.print(min_inter_check_ms);
		Serial.println("");
		delay(1000);*/
		
		if( (curr_time - last_time_in_checked) >= min_inter_check_ms )
			return true;
		
		// reset for the case of timer overflow
		if(curr_time<last_time_in_checked)
			updateCheckTime();

		return false;
	}
	void updateCheckTime()
	{
		last_time_in_checked=millis();
	}

	bool isMinChekTimeElapsedAndUpdate()
	{
		bool istime = isMinChekTimeElapsed();
		if(istime)
			updateCheckTime();
		return istime;
	}
};




#endif
