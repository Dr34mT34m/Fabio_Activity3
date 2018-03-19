#ifndef TASK3SYSTEM1_h
#define TASK3SYSTEM1_h


//The following is useful for use with the Visual Studio Projects provided.
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#include "..\Task3Sense1\Task3Sense1.h"
#include "..\Task3Action1\Task3Action1.h"
#include "..\Task3Control1\Task3Control1.h"
#else
#include <Arduino.h>
#include <Task3Sense1.h>
#include <Task3Action1.h>
#include <Task3Control1.h>
#endif
//Have included libraries above which include classes of components we will use.
//The following library implements the full system of the project.

class motorsystem{
	int save_motorpwmpin;
	int save_motordirectionpin;
	int save_dir;
	
    protected:
        sense sensing_unit;
        action action_unit;
        control control_unit;
        boolean enabled;
    
    
        void set_enabled(){                         //sets the enabled status of the combined components in the system
            enabled = action_unit.AreAllEnabled();
        }
    public:
        motorsystem(){      //default constructor
            set_enabled();
        }
    
    
        void setup_sensing(int ref_speed, ArduinoInterruptNames interrupt_pin, int magnets){       //set up of sensing unit
            sensing_unit.setup_sensing_unit(ref_speed, interrupt_pin, magnets);
        }
    
    
        void setup_action(int motorpwmpin, int motordirectionpin, int dir){  //set up of action unit
        	save_motorpwmpin = motorpwmpin;
        	save_motordirectionpin = motordirectionpin;
        	save_dir = dir;
        
            action_unit.setup_motor(motorpwmpin);
            action_unit.setup_direction(motordirectionpin, dir);
            set_enabled();
        }
    
    
        void setup_control(unsigned long check_interval, double ref_kp, double ref_ki, double ref_kd, int ref_control_interval_time/*, int max_rpm*/){ //setup control unit parameters
            control_unit.setup_controller(check_interval, ref_kp, ref_ki, ref_kd, ref_control_interval_time/*, max_rpm*/);
        }
    
    
        void execute_system_task(){                                     //carries out continuous system operation (refreshes speed of motor and reads motor speed)
            if(control_unit.isTimeToTakeMeasurementAndUpdate()){
                action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
            }
            
            //speed control
            if(sensing_unit.rotation_counter.checkDistanceMet(1000) ==true){
            	Serial.print("speedy");
            	
            	if(save_dir==1){
            		setup_action(save_motorpwmpin, save_motordirectionpin, 2);
            	}else{
            		setup_action(save_motorpwmpin, save_motordirectionpin, 1);
            	}
            	sensing_unit.rotation_counter.reset_distancecount();
            }
        }
    
    bool isTimeToTakeMeasurement(){
        if (control_unit.isTimeToTakeMeasurement()){
            return true;
        }
        else{
            return false;
        }
    }
    
};
                
#endif
