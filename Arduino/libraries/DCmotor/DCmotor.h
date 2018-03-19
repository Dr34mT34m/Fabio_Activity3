#ifndef DCMOTOR_H
#define DCMOTOR_H


#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif

#include "..\PushButton\PushButton.h"
#include "..\ProperLED\ProperLED.h"
#include "..\IntervalCheckTimer\IntervalCheckTimer.h"



class BasicDCmotor{
protected:	
	// operate the PWM
	an_LED motor_drive;

	int fixed_delta;
	static const int default_fixed_delta=1;

	// used to see target speed
	bool echo_pwm;

	// the min speed sustainable
	int min_sustainable_pwm;
	static const int min_sustainable_pwm_default=60;

	boolean useStartupWhenBelowMinPWM;
	
	void speedUp(int steps)
	{
		
		// kick-start if it is too slow
		if(useStartupWhenBelowMinPWM)
			startUpSequence();
		//
		for(int step=0; step<steps; step++)
			motor_drive.brighter();
	}

	void speedDown(int steps)
	{
		for(int step=0; step<steps; step++)
			motor_drive.dimmer();
	}
	

public:
	BasicDCmotor()
	{
		min_sustainable_pwm=min_sustainable_pwm_default;
		
		fixed_delta=default_fixed_delta;
		
		echo_pwm=false;
		
		//
		min_sustainable_pwm=min_sustainable_pwm_default;

		// by deafult this is not enabled;
		useStartupWhenBelowMinPWM=false;		
	}

	void setup_motor(int motorpin)
	{
		// enable PWM pin
		motor_drive.setup_LED(motorpin); 
		//switch on and set zero speed
		motor_drive.switch_on(); 
		motor_drive.set_brightness(0);

		//
		if(echo_pwm)
		{
			Serial.begin(9600);
		}
	}
	void enableEcho()
	{
		if(!echo_pwm)
		{
			Serial.begin(9600);
			echo_pwm=true;
		}
	}
	
	void set_fixed_delta(int delta_steps) {fixed_delta=max(delta_steps, 1);}

	virtual boolean check_enabled(){return motor_drive.isEnabled() ;}
	void setup_min_speed(int min_speed)
	{
		min_sustainable_pwm=max(min_sustainable_pwm_default,min_speed);
	}
	void startUpSequence()
	{
		int startup_pwm_ms=200;
		
		// bootstrap sequence needed if (sufficiently) below the min sustainable speed
		if(motor_drive.isEnabled() && (motor_drive.get_brightness()<(min_sustainable_pwm-4)) )
		{
			 //motor_drive.set_brightness(startup_pwm);
			 motor_drive.set_max_bright();
			 delay(startup_pwm_ms);
			 //motor_drive.set_brightness(startup_pwm);
			 //delay(startup_pwm_ms/2);
			 
			 //
			 motor_drive.set_brightness(min_sustainable_pwm);

		}
	}
	void setSpeedPWM(int inp_pwm)
	{
		// force inp_pwm to be in the range [0 , 255]
		inp_pwm=max(inp_pwm,0);
		inp_pwm=min(inp_pwm,255);
		
		// set pwm if different from current
		if( inp_pwm != motor_drive.get_brightness() )
			motor_drive.set_brightness(inp_pwm);

		if(echo_pwm)
		{
			Serial.print("PWM  = ");
			Serial.print(motor_drive.get_brightness());
			Serial.println("");
		}
	}

	void speedUpFixedAmount()
	{
		speedUp(fixed_delta);
		if(echo_pwm)
		{
			Serial.print("PWM  = ");
			Serial.print(motor_drive.get_brightness());
			Serial.println("");
		}
	}
	void speedDownFixedAmount()
	{
		speedDown(fixed_delta);
		if(echo_pwm)
		{
			Serial.print("PWM  = ");
			Serial.print(motor_drive.get_brightness());
			Serial.println("");
		}
	}
	
	int getSpeedPWM() {return motor_drive.get_brightness();}
	
	//void set_delta_PWM(int delta_PWM){fixed_delta=max(1, delta_PWM);}
	//void enableStartupSequence(bool enable){useStartupWhenBelowMinPWM=enable;}
};


class SimpleDCmotor: public BasicDCmotor{
protected:	
	// input buttons
	inputs motor_input;
	// used to avoid checing the buttons too frequently
	IntervalCheckTimer inp_inter_check;
	static const unsigned long int  default_min_inter_check_ms=100;
	void executeSimpleCommand(command_list_enum in_smpl_cmd)
	{
		/*Serial.println("");
		Serial.print("simple command  = ");
		Serial.print(in_smpl_cmd);
		Serial.println("");*/
		
		switch (in_smpl_cmd){
		case up_fix_amount:
			speedUp(fixed_delta);
			break;
		case down_fix_amount:
			speedDown(fixed_delta);
			break;
		default:
			;
		}

		if(echo_pwm)
		{
			Serial.print("PWM  = ");
			Serial.print(motor_drive.get_brightness());
			Serial.println("");
		}
	}	
public:
	SimpleDCmotor():BasicDCmotor() {useStartupWhenBelowMinPWM=true;}	
	virtual boolean check_enabled(){return ( (motor_input.check_enabled()) && (motor_drive.isEnabled()) ) ;}
	void add_in_push_button(in_push_button in_pb){motor_input.add_in_push_button(in_pb);}
	void set_inter_check_ms(int inp_inter_check_ms)
	{
		inp_inter_check.setInterCheck(inp_inter_check_ms);
	}
	void check_n_execute_command()
	{
		
		if(inp_inter_check.isMinChekTimeElapsed())
		{
			
			/*Serial.println("");
			Serial.print("checking time");
			Serial.println("");*/
			
			command_list_enum in_smpl_cmd;
			
			// get a command from the input
			boolean success=motor_input.check_n_get_command(in_smpl_cmd);

			/*Serial.println("");
			Serial.print("success = ");
			Serial.print(success);
			Serial.println("");*/
			
			// execute
			if(success)
				executeSimpleCommand(in_smpl_cmd);


			//reset timer
			inp_inter_check.updateCheckTime();
		}
	}
};




class HBridgeDCmotor{
protected:	
	// motor
	BasicDCmotor motor;

	// binalry control for the direction
	di_LED direction;
	bool directionONtogoFWD;

	//start or stop
	bool started;

	// set this to true to jumpstart the motor
	bool jumpstartfromstill;

	//functions
	void setFWD()
	{
		if(directionONtogoFWD)
			direction.switch_on();
		else
			direction.switch_off();
	}
	void setBKW()
	{
		if(directionONtogoFWD)
			direction.switch_off();
		else
			direction.switch_on();
	}
	bool isFWD()
	{
		if(directionONtogoFWD)
			return direction.isOn();
		else
			return !direction.isOn();
	}
public:
	HBridgeDCmotor()
	{
		directionONtogoFWD=true;
		started=false;
		jumpstartfromstill=false;
	}
	void setup_HBridgeDCmotor(int motorpin, int directionpin)
	{
		motor.setup_motor(motorpin);
		direction.setup_LED(directionpin);
		motor.setSpeedPWM(0);
		setFWD();
	}
	
	void start()
	{
		if(!started)
		{
			started=true;
			if(jumpstartfromstill)
				motor.startUpSequence();
		}
	}

	void stop()
	{
		if(started)
		{
			setSpeedPWM(0);
			started=false;
		}
	}

	void changedir()
	{
		int prevspeed=motor.getSpeedPWM();
		
		// stop the motor
		stop();
		
		// change direction
		if(isFWD())
			setBKW();
		else
			setFWD();

		// start again at the same speed we had before
		start();
		setSpeedPWM(prevspeed);

	}

	void setSpeedPWM(int inp_pwm) 
	{
		if(started)
			motor.setSpeedPWM(inp_pwm);
	}
	int getSpeedPWM(){return motor.getSpeedPWM();}
	void set_jumpstart(bool performjumprtart){jumpstartfromstill=performjumprtart;}

	bool isStarted(){return started;}

	// quick fix to check the motor works
	void speedDownFixedAmount(){motor.speedDownFixedAmount();}
	void speedUpFixedAmount(){motor.speedUpFixedAmount();}
	void enableEcho(){motor.enableEcho();}
	//

};


#endif