#ifndef BASIC_INPUT_h
#define BASIC_INPUT_h

// the following is useful for use with the Visual Studio Projects provided
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif



/// ==== analog input ========
class in_analog{
private:	
	int AnalogPin;
	boolean initialized;
	//		
public:
	in_analog(){AnalogPin=-1; initialized=false;}
	in_analog(int in_Pin) {AnalogPin=-1; initialized=false; setup_in_analog(in_Pin);} 
	void setup_in_analog(int in_Pin)
	{
		if(in_Pin>=0)
		{
			AnalogPin=in_Pin;
			initialized=true;
		}
	}
	inline boolean isInitialized() {return initialized;}
	inline boolean read_input(int& val)
	{
		boolean success=false;
		val=-1;
		if(initialized)
		{
			val=analogRead(AnalogPin);// range is: [0 , 1023]
			success=true;
		}
		return success;
	}
};



//
class in_digital{
protected:	
	int DigitalPin;
	boolean initialized;
	//		
public:
	in_digital()
    {
                DigitalPin=-1; 
                initialized=false;
    }
	in_digital(int in_Pin, boolean pullup=false) {DigitalPin=-1; initialized=false; setup_in_digital(in_Pin, pullup);} 
	virtual void setup_in_digital(int in_Pin, boolean pullup=false)
	{
		if((!initialized) && in_Pin>=0)
		{
			DigitalPin=in_Pin;
			if(pullup)
				pinMode(DigitalPin,INPUT_PULLUP);
			else
				pinMode(DigitalPin,INPUT); 
			initialized=true;
		}
	}
	inline boolean isInitialized() {return initialized;}
	virtual inline boolean read_input(int& val)
	{
		boolean success=false;
		val=-1;
		if(initialized)
		{
			val=digitalRead(DigitalPin);
			success=true;
		}
		return success;
	}
};



class in_analogWmapping : public in_analog{
private:	
	bool map_initialized;
	int min_x_val, max_x_val, min_y_val, max_y_val;
	//		
public:
	in_analogWmapping(){map_initialized=false;}
	void setup_map(int in_min_y_val, int in_max_y_val)
	{
		// se the in value as the lmits for the mapping of the tension
		min_x_val=0;
		max_x_val=1023;
		min_y_val=in_min_y_val;
		max_y_val=in_max_y_val;

		map_initialized=true;
	}
	virtual inline boolean read_input(int& val)
	{
		// call the parent function to do the reading
		boolean success=in_analog::read_input(val);
		if(success && map_initialized)
		{
			int mapped_val=map(val, min_x_val, max_x_val, min_y_val, max_y_val);
			
			int lower_end=min(min_y_val, max_y_val);
			int upper_end=max(min_y_val, max_y_val);
			mapped_val=constrain(mapped_val, lower_end, upper_end);
			
			val=mapped_val;
		}
		return success;
	}
};




class in_serial{
protected:		
	boolean initialized;
	String inString;
	//		
public:
	in_serial()
    {
		Serial.begin(9600); 
		initialized=true;
		inString = "";
    }	
	inline boolean isInitialized() {return initialized;}
	virtual inline boolean read_input(int& val)
	{
		boolean success=false;
		val=-1;
		if(initialized)
		{
			while (Serial.available() > 0)
			{
				int inChar = Serial.read();
				if (isDigit(inChar)) {
					// convert the incoming byte to a char
					// and add it to the string:
					inString += (char)inChar;
				}
				// if you get a newline, the string is over print the string,
				// then the string's value:
				if (inChar == '\n')
				{
					val=inString.toInt();
					success=true;
					// clear the string for new input:
					inString = "";
				}
			}
		}
		return success;
	}
};



/// ========================================





#endif
