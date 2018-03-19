#ifndef TASK3ACTION1_h
#define TASK3ACTION1_h

// The following is useful for use with the Visual Studio Projects provided.
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#else
#include <Arduino.h>
#endif
// Have included libraries above which include classes of components we will use.
// The following library implements the action unit of the system.

class action{
	protected:
		int motor_pin;
        int direction_pin;
		int current_pwm;
    	int off_pwm;
    	boolean motorenabled;
        boolean directionenabled;
		///boolean motor_on;
        int current_direction;  //stopped = 0, forwards = 1, reverse = 2
    
		
		void set_motor_pin_on(){                //sets pin to current_pwm value
			analogWrite(motor_pin,current_pwm);
		}
		

		void set_motor_pin_off(){               //sets pin to off_pwm value
			analogWrite(motor_pin,off_pwm);
		}
	
    
        void set_direction_pin_forwards(){      //sets direction pin to HIGH
            current_direction = 1;
            digitalWrite(direction_pin, HIGH);
        }
    
    
        void set_direction_pin_backwards(){     //sets direction pin to LOW
            current_direction = 2;
            digitalWrite(direction_pin, LOW);
        }
    
		
    	void set_default_vals(){                //initialize variables to default values
			motor_pin = -1;
            direction_pin = -1;
			current_pwm = 0;
			off_pwm = 0; 
			motorenabled = false;
            directionenabled = false;
			motor_on = false;
            current_direction = 0;
		}
	
public:
	action(){                   //default constructor
		set_default_vals();
	}

	
	action(int mot_pin, int dir_pin, int dir){      //constructor with arguments
		set_default_vals();
		setup_motor(mot_pin);
        setup_direction(dir_pin, dir);
	}
	
    
	void setup_motor(int mot_pin){          //sets pin number for motor, and sets pin mode as OUTPUT
        if(isMotorEnabled())
            return;
        if(mot_pin>=0)
		{
			motor_pin = mot_pin;
			pinMode(mot_pin,OUTPUT);
			motorenabled = true;
		}
	}
	
    
    void setup_direction(int dir_pin, int dir){      //sets pin number for direction, and sets pin mode as OUTPUT
        current_direction = dir;
        if(isDirectionEnabled())
            return;
        if(dir_pin>=0)
        {
            direction_pin = dir_pin;
            pinMode(dir_pin,OUTPUT);
            directionenabled = true;
        }
    }
    
    
	boolean isMotorEnabled(){       //checks if motor is enabled
		return motorenabled;
	}
    
    
    boolean isDirectionEnabled(){   //checks if direction output is enabled
        return directionenabled;
    }
    
    
    boolean AreAllEnabled(){        //checks if all action unit components are enabled
        return (motorenabled && directionenabled);
    }
	
    
	boolean isMotorOn(){        //checks if motor is on
		return motor_on;
	}
    
    
    void operate_motor(int PID_output){
        if (current_direction == 0){
            switch_off_motor();
        }
        if (current_direction == 1){
            operate_motor_forwards(PID_output);
        }
        if (current_direction == 2){
            operate_motor_backwards(PID_output);
        }
    }
    
    
    void operate_motor_forwards(int PID_output){                //Activates motor in forward direction
        
        if (isMotorEnabled()){
            set_direction_pin_forwards();
            current_pwm = PID_output;
            set_motor_pin_on();
            motor_on = true;
        }
        
    }
    
    
    void operate_motor_backwards(int PID_output){                //Activates motor in backwards direction
        
        if (isMotorEnabled()){
            set_direction_pin_backwards();
            current_pwm = PID_output;
            set_motor_pin_on();
            motor_on = true;
        }
        
    }
    
    
	void switch_off_motor(){                        //turns motor off
		if( isMotorEnabled() && isMotorOn() )
		{
			set_motor_pin_off();
            current_direction = 0;
			motor_on = false;
			current_pwm = off_pwm;
		}
	}
    
    
    void return_current_motor_pwm(){                //returns current motor pwm
        return current_pwm;
    }
    
	
};

#endif
