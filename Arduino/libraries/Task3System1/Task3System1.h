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

    protected:
        sense sensing_unit;
        action action_unit;
        control control_unit;
        boolean enabled;
        int save_motorpwmpin;
        int save_motordirectionpin;
        int save_dir;
        int save_dist;
    
        int stationaryCount;
        int opposite_dir;
    
        void set_enabled(){                         //sets the enabled status of the combined components in the system
            enabled = action_unit.AreAllEnabled();
        }
    public:
        motorsystem(){      //default constructor
            stationaryCount =0;
            set_enabled();
        }
    
    
        void setup_sensing(int ref_speed, ArduinoInterruptNames interrupt_pin, int magnets){       //set up of sensing unit
            sensing_unit.setup_sensing_unit(ref_speed, interrupt_pin, magnets);
        }
    
    
        void setup_action(int motorpwmpin, int motordirectionpin, int dir, int dist){  //set up of action unit
        	save_motorpwmpin = motorpwmpin;
        	save_motordirectionpin = motordirectionpin;
        	save_dir = dir;
            save_dist = dist;
            
            action_unit.setup_motor(motorpwmpin);
            action_unit.setup_direction(motordirectionpin, dir);
            set_enabled();
        }
    
    
        void setup_control(unsigned long check_interval, double ref_kp, double ref_ki, double ref_kd, int ref_control_interval_time/*, int max_rpm*/){ //setup control unit parameters
            control_unit.setup_controller(check_interval, ref_kp, ref_ki, ref_kd, ref_control_interval_time/*, max_rpm*/);
        }
    
    
        void execute_system_task(){    //moves car forward for set distance, pauses then returns
            if(control_unit.isTimeToTakeMeasurementAndUpdate()){
                action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
            }
            
            //distance control
            if(sensing_unit.rotation_counter.checkDistanceMet(save_dist) ==true){
            	
            	if(save_dir==1){
            		opposite_dir = 2;
            		setup_action(save_motorpwmpin, save_motordirectionpin, 0,save_dist);
            		save_dir =0;
            		control_unit.resetPID();
            	}else if (save_dir==2){
            		opposite_dir = 1;
                    setup_action(save_motorpwmpin, save_motordirectionpin, 0,save_dist);
                    save_dir=0;
                    control_unit.resetPID();
                    
            	}else if (save_dir==0){
					if(stationaryCount ==10){
						setup_action(save_motorpwmpin, save_motordirectionpin, opposite_dir,save_dist);
						sensing_unit.rotation_counter.reset_distancecount();
						stationaryCount =0;
						control_unit.resetPID();
						
					}            	
            	
            		stationaryCount++;
            	}
            	
            	
            	
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
