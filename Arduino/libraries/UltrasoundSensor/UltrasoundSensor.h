#ifndef UltrasoundSensors_h
#define UltrasoundSensors_h


#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"

#else
#include <Arduino.h>

#endif






enum distance_unit
{
	cm=0, mm, inch,
	dummy_last_unit
};

// base (abstract) class for the ultrasound sensor
class AbstractUltrasoundSensor{
protected:	
	int microseconds_per_centimeter;
	unsigned long int  timeout_microsec;
	boolean sensor_enabled;
	//
	static const int default_microseconds_per_centimeter=29;
	static const unsigned long int default_timeout_microsec=9000;
	//
	distance_unit sens_dist_unit;
	
	//		
	int microsecondsToDistance(int duration_microsec)
	{
		switch(sens_dist_unit){
		case cm:
			return microsecondsToCentimeters(duration_microsec);
			break;
		case mm:
			return microsecondsToMillimeters(duration_microsec);
			break;
		case inch:
			return (int)( (double)(microsecondsToCentimeters(duration_microsec)) / 2.54);
			break;
		default:
			return 0;
		}
	}

	int microsecondsToCentimeters(int duration_microsec)
	{
		// The speed of sound is 340 m/s or 29 microseconds per centimeter.
		// The ping travels out and back, so to find the distance of the
		// object we take half of the distance travelled.
		return ( duration_microsec / microseconds_per_centimeter )/ 2;
	}	

	int microsecondsToMillimeters(int duration_microsec)
	{
		double microseconds_per_millimeter=microseconds_per_centimeter/10;
		// The speed of sound is 340 m/s or 29 microseconds per centimeter.
		// The ping travels out and back, so to find the distance of the
		// object we take half of the distance travelled.

		return (  (int)( ( ((double)duration_microsec) / microseconds_per_millimeter )/ 2 )   );
	}

	//
	virtual int PingTriggerAndGetPulseDuraton()=0;
	//
public:
	AbstractUltrasoundSensor()
	{	
		distance_unit default_sens_dist_unit=cm; 

		// default, can be changed later
		microseconds_per_centimeter=default_microseconds_per_centimeter;
		timeout_microsec=default_timeout_microsec;
		sens_dist_unit=default_sens_dist_unit;
		//
		sensor_enabled=false;
	}

	inline boolean isEnabled(){return sensor_enabled;}

	//	
	boolean SenseDistance(int& distance)
	{	
		int duration_microsec;

		if(sensor_enabled==false)
		{
			//Serial.println(" Sensor not enabled");
			
			distance = 0;
			return false;
		}

		// sense
		duration_microsec = PingTriggerAndGetPulseDuraton();


		if(duration_microsec>0)
		{
			// convert the time into a distance	
			distance = microsecondsToDistance(duration_microsec);
			return true;
		}
		else
		{
			//Serial.println(" Null duration for Sensor");
			
			distance = 0;
			return false;
		}

	}

	//
	void set_dist_unit(int unit){sens_dist_unit=(distance_unit)( (unit>=cm && unit<dummy_last_unit) ? unit : cm);}	
	void set_timeout_microsec(unsigned long int inp_timeout_microsec)
	{
		//overwrite the defalt value (if reasonable)
		//if(inp_timeout_microsec>0)
			timeout_microsec=inp_timeout_microsec;
	}
	//
	void set_timeout_distance_cm(int inp_timeout_cm)
	{
		unsigned long int inp_timeout_microsec=( (unsigned long int)inp_timeout_cm )*( (unsigned long int)default_microseconds_per_centimeter );
		// double it, for the return trip:
		inp_timeout_microsec*=2;
		set_timeout_microsec(inp_timeout_microsec);
	}
	//

};
//


// +++ derived classes for ultrsound sensors

class ParallaxPingProximitySensor : public AbstractUltrasoundSensor{
protected:
	//
	int signal_pin;	
	//
	int PingTriggerAndGetPulseDuraton()
	{
		int duration_of_high_pulse_microsec=5;
		int duration_of_cleaning_low_pulse_microsec=2;

		unsigned long duration_microsec;

		// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
		// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
		pinMode(signal_pin, OUTPUT);
		//
		digitalWrite(signal_pin, LOW);
		delayMicroseconds(duration_of_cleaning_low_pulse_microsec);
		//
		digitalWrite(signal_pin, HIGH);
		delayMicroseconds(duration_of_high_pulse_microsec);
		digitalWrite(signal_pin, LOW);
		//

		// The same pin is used to read the signal from the PING))): a HIGH
		// pulse whose duration is the time (in microseconds) from the sending
		// of the ping to the reception of its echo off of an object.
		pinMode(signal_pin, INPUT);
		duration_microsec = pulseIn(signal_pin, HIGH, timeout_microsec);

		/*
		Serial.print(" Pin is ");
		Serial.println(signal_pin);
		Serial.print(" Timeout us ");
		Serial.println(timeout_microsec);
		Serial.print(" Duration us ");
		Serial.println(duration_microsec);
		//*/

		return (int)duration_microsec;
	}
	//
public:
	ParallaxPingProximitySensor() : AbstractUltrasoundSensor() {signal_pin=-1;}
	
	ParallaxPingProximitySensor(int inp_signal_pin) : AbstractUltrasoundSensor()
	{	
		signal_pin=-1;
		setup_ParallaxPingProximitySensor(inp_signal_pin);
	}

	//	
	void setup_ParallaxPingProximitySensor(int inp_signal_pin)
	{
		if(inp_signal_pin>=0)
		{		
			signal_pin=inp_signal_pin;
			sensor_enabled=true;
		}
		sens_dist_unit=cm;
	}
	
	//
};


class FourPinsPingProximitySensor : public AbstractUltrasoundSensor{
protected:
	//
	int signal_out_pin;
	int signal_in_pin;
	//
	int PingTriggerAndGetPulseDuraton()
	{
		int duration_of_high_pulse_microsec=10;
		int duration_of_cleaning_low_pulse_microsec=3;

		int duration_microsec;



		//// THIS NEEDS TO BE CONFIRMED FOR THE 4 PINS SENSOR

		// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
		// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
		//

		//Serial.println("Cleaning pulse.");
		//Serial.print("Read with timeout of ");
		//Serial.print(timeout_microsec);
		//Serial.println(" microsec");

		digitalWrite(signal_out_pin, LOW);
		delayMicroseconds(duration_of_cleaning_low_pulse_microsec);
		//
		digitalWrite(signal_out_pin, HIGH);
		delayMicroseconds(duration_of_high_pulse_microsec);
		digitalWrite(signal_out_pin, LOW);
		//

		// read the signal from the PING))): a HIGH
		// pulse whose duration is the time (in microseconds) from the sending
		// of the ping to the reception of its echo off of an object.
		duration_microsec = pulseIn(signal_in_pin, HIGH, timeout_microsec);

		////////
		/*
		Serial.println("");
		Serial.print(" Write Pin is ");
		Serial.println(signal_out_pin);
		Serial.print(" pulseIn Pin is ");
		Serial.println(signal_in_pin);
		Serial.print(" Timeout us ");
		Serial.println(timeout_microsec);
		Serial.print(" Duration us ");
		Serial.println(duration_microsec);
		Serial.println("");
		//*/

		return duration_microsec;
	}

	//
public:
	FourPinsPingProximitySensor() : AbstractUltrasoundSensor()	{signal_in_pin=-1; signal_out_pin=-1;}

	FourPinsPingProximitySensor(int inp_signal_in_pin, int inp_signal_out_pin) : AbstractUltrasoundSensor()
	{	
		signal_in_pin=-1;
		signal_out_pin=-1;
		setup_FourPinsPingProximitySensor(inp_signal_in_pin, inp_signal_out_pin);
	}
	//	
	void setup_FourPinsPingProximitySensor(int inp_signal_in_pin, int inp_signal_out_pin)
	{
		if( (inp_signal_out_pin>=0) && (inp_signal_in_pin>=0) && (inp_signal_in_pin!=inp_signal_out_pin) )
		{
			signal_out_pin=inp_signal_out_pin;
			pinMode(signal_out_pin, OUTPUT);
			//
			signal_in_pin=inp_signal_in_pin;
			pinMode(signal_in_pin, INPUT);
			//
			sensor_enabled=true;
			//Serial.println("Sensor enabled");
		}
		sens_dist_unit=cm;
	}
	//
};

// +++



//// ******************** ////
// generic item supporting 3 or 4 pins
// aborted

class PingProximitySensor{
protected:
	ParallaxPingProximitySensor threePinSensor;
	FourPinsPingProximitySensor fourPinSensor;
	bool use3pin;
public:
	PingProximitySensor(){use3pin=true;}
	
	void setup_PingProximitySensor(int echo_pin, int trigger_pin)
	{
		fourPinSensor.setup_FourPinsPingProximitySensor(echo_pin, trigger_pin);
		use3pin=false;
	}

	void setup_PingProximitySensor(int inp_signal_pin)
	{
		threePinSensor.setup_ParallaxPingProximitySensor(inp_signal_pin);
		use3pin=true;
	}

	inline boolean isEnabled()
	{
		if(use3pin)
			return threePinSensor.isEnabled();
		else
			return fourPinSensor.isEnabled();
	}

	//	
	boolean SenseDistance(int& distance)
	{	
		if(use3pin)
			return threePinSensor.SenseDistance(distance);
		else
			return fourPinSensor.SenseDistance(distance);
	}

	//
	void set_dist_unit(int unit)
	{
		if(use3pin)
			threePinSensor.set_dist_unit(unit);
		else
			fourPinSensor.set_dist_unit(unit);
	}	

	void set_timeout_microsec(unsigned long int inp_timeout_microsec)
	{
		if(use3pin)
			threePinSensor.set_timeout_microsec(inp_timeout_microsec);
		else
			fourPinSensor.set_timeout_microsec(inp_timeout_microsec);		
	}
	//
	void set_timeout_distance_cm(int inp_timeout_cm)
	{
		if(use3pin)
			threePinSensor.set_timeout_distance_cm(inp_timeout_cm);
		else
			fourPinSensor.set_timeout_distance_cm(inp_timeout_cm);
	}
};


//*/

//// ******************** ////




#endif