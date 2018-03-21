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
        int save_ref_speed;
        //int save_motorpwmpin;
        //int save_motordirectionpin;
        int save_dir;
        double noOfMagnets;
        int stationaryCount;
        int opposite_dir;
    
        void set_enabled(){                         //sets the enabled status of the combined components in the system
            enabled = action_unit.AreAllEnabled();
        }
    public:
        motorsystem(){      //default constructor
            save_ref_speed = 0;
            //save_motorpwmpin = 0;
            //save_motordirectionpin = 0;
            save_dir = 0;
            noOfMagnets = 0;
            stationaryCount = 0;
            opposite_dir = 0;
            set_enabled();
        }
    
    
        void setup_sensing(int ref_speed, ArduinoInterruptNames interrupt_pin, int magnets){       //set up of sensing unit
            save_ref_speed = ref_speed;
            sensing_unit.setup_sensing_unit(ref_speed, interrupt_pin, magnets);
        }
    
    
        void setup_action(int motorpwmpin, int motordirectionpin, int dir, int dist){  //set up of action unit
        	//save_motorpwmpin = motorpwmpin;
        	//save_motordirectionpin = motordirectionpin;
        	save_dir = dir;
            noOfMagnets = action_unit.convertDistanceToMag(dist);
            
            action_unit.setup_motor(motorpwmpin);
            action_unit.setup_direction(motordirectionpin, dir);
            set_enabled();
        }
    
    
        void setup_control(unsigned long check_interval, double ref_kp, double ref_ki, double ref_kd, int ref_control_interval_time){ //setup control unit parameters
            control_unit.setup_controller(check_interval, ref_kp, ref_ki, ref_kd, ref_control_interval_time);
        }
    
    
        void execute_system_task_straight_line(){    //moves car in a straight line
            if(control_unit.isTimeToTakeMeasurementAndUpdate()){
                action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
            }
        }
    
    
        void execute_system_task_distance_return(unsigned long stop_time_ms){   //moves car forward for set distance, pauses then returns
            //static int flag = 0;
            if(control_unit.isTimeToTakeMeasurementAndUpdate()){
                /*if (flag == 0){
                    control_unit.resetPID();
                    sensing_unit.set_ref_speed(0);
                    action_unit.switch_off_motor();
                    sensing_unit.set_ref_speed(save_ref_speed);
                    action_unit.set_direction(save_dir);
                    flag++;
                }*/
                //if (sensing_unit.return_current_speed() == 0){
                    //control_unit.resetPID();
                //}
                action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
            }
            
            //distance control
            if(sensing_unit.rotation_counter.checkDistanceMet((long int)noOfMagnets) == true){
            	
            	if(save_dir==1){
            		opposite_dir = 2;
                    sensing_unit.set_ref_speed(0);
                    action_unit.switch_off_motor();
            		//setup_action(save_motorpwmpin, save_motordirectionpin, 0, noOfMagnets);
            		save_dir = 0;
            		//control_unit.resetPID();
            	}
                else if (save_dir==2){
            		opposite_dir = 1;
                    sensing_unit.set_ref_speed(0);
                    action_unit.switch_off_motor();
                    //setup_action(save_motorpwmpin, save_motordirectionpin, 0,save_mag);
                    save_dir = 0;
                    //control_unit.resetPID();
            	}
                else if (save_dir==0){
					if(stationaryCount >= (stop_time_ms/control_unit.getInterCheck())){
                        //control_unit.resetPID();
                        sensing_unit.rotation_counter.reset_distancecount();
                        sensing_unit.set_ref_speed(save_ref_speed);
                        action_unit.set_direction(opposite_dir);
						//setup_action(save_motorpwmpin, save_motordirectionpin, opposite_dir,save_mag);
						stationaryCount = 0;
                        save_dir = action_unit.return_current_direction();
						//control_unit.resetPID();
						
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
