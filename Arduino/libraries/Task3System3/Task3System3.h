#ifndef TASK3SYSTEM3_h
#define TASK3SYSTEM3_h

//The following is useful for use with the Visual Studio Projects provided.
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#include "..\Task3System1\Task3System1.h"
#else
#include <Arduino.h>
#include <Task3System1.h>
#endif
//Have included the library that will be inherited for this class.


class circular_operation: public motorsystem{
	protected:
        double radiusR;
        double radiusL;
        double circumferenceC;
        double circumferenceR;
        double circumferenceL;
        double speedR;
        double speedL;
        int lap;
        int flag;
        int lap_counter;
        int direction_complete_counter;
        int save_wheel_side;
        int circle_number;
	public:
	
    
    circular_operation(int wheel_side){
        flag = 0;
        lap_counter = 1;
        direction_complete_counter = 0;
        save_wheel_side = wheel_side;
        circle_number = 0;
    }
    
    
    void execute_system_task_circular(double radius, double speed, int laps, double radius2, double speed2, int laps2, unsigned long stop_time_ms, int course_repetitions){ //wheel_side right = 1 left = 2
        if (circle_number % 2 == 0){
            radiusR = radius - 50;
            radiusL = radius + 50;
            circumferenceC = 2*3.14*radius;
            circumferenceR = 2*3.14*radiusR;
            circumferenceL = 2*3.14*radiusL;
            speedR = (speed*(radius-50))/radius;
            speedL = (speed*(radius+50))/radius;
            speedR = speedR*1.02;   //2% increase to account for lateral friction
            speedL = speedL*0.98;   //2% decrease to account for lateral friction
            lap = laps;
        }
        else if (circle_number % 2 == 1){
            radiusR = radius2 - 50;
            radiusL = radius2 + 50;
            circumferenceC = 2*3.14*radius2;
            circumferenceR = 2*3.14*radiusR;
            circumferenceL = 2*3.14*radiusL;
            speedR = (speed2*(radius2-50))/radius2;
            speedL = (speed2*(radius2+50))/radius2;
            speedR = speedR*1.02;   //2% increase to account for lateral friction
            speedL = speedL*0.98;   //2% decrease to account for lateral friction
            lap = laps2;
        }
        
        
        if (direction_complete_counter<(2*course_repetitions)){
            
            if (save_wheel_side == 1){
                //Serial.println();
                //Serial.print("Wheel_side = ");
                //Serial.print(save_wheel_side);
                //Serial.println();
                
                if (flag == 0){
                    sensing_unit.set_ref_speed((int)speedR);
                    set_distance_to_travel(circumferenceR);
                    action_unit.set_direction(save_dir);
                    flag++;
                }
                
                if(control_unit.isTimeToTakeMeasurementAndUpdate()){
                    action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
                }
                
                //distance control
                if(sensing_unit.rotation_counter.checkDistanceMet((int)noOfMagnets) == true){
                    if (lap_counter<lap){
                        lap_counter++;
                        //Serial.println();
                        //Serial.print("lap_counter right = ");
                        //Serial.print(lap_counter);
                        //Serial.println();
                        sensing_unit.rotation_counter.reset_distancecount();
                    }
                    else{
                        sensing_unit.set_ref_speed(0);
                        action_unit.switch_off_motor();
                        stationaryCount++;
                        //Serial.println();
                        //Serial.print("stationaryCount = ");
                        //Serial.print(stationaryCount);
                        //Serial.println();
                        if(stationaryCount > (stop_time_ms/control_unit.getInterCheck())){
                            //Serial.println();
                            //Serial.print("HERE!!");
                            //Serial.println();
                            sensing_unit.rotation_counter.reset_distancecount();
                            stationaryCount = 0;
                            flag = 0;
                            save_wheel_side = 2;
                            lap_counter = 1;
                            direction_complete_counter++;
                            circle_number++;
                            control_unit.resetPID();
                        }
                    }
                }
            }
            
            
            else if (save_wheel_side == 2){
                //Serial.println();
                //Serial.print("Wheel_side = ");
                //Serial.print(save_wheel_side);
                //Serial.println();
                
                if (flag == 0){
                    sensing_unit.set_ref_speed((int)speedL);
                    set_distance_to_travel(circumferenceL);
                    action_unit.set_direction(save_dir);
                    flag++;
                }
                
                if(control_unit.isTimeToTakeMeasurementAndUpdate()){
                    action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
                }
                
                //distance control
                if(sensing_unit.rotation_counter.checkDistanceMet((int)noOfMagnets) == true){
                    if (lap_counter<lap){
                        lap_counter++;
                        //Serial.println();
                        //Serial.print("lap_counter left = ");
                        //Serial.print(lap_counter);
                        //Serial.println();
                        sensing_unit.rotation_counter.reset_distancecount();
                    }
                    else{
                        sensing_unit.set_ref_speed(0);
                        action_unit.switch_off_motor();
                        stationaryCount++;
                        //Serial.println();
                        //Serial.print("stationaryCount = ");
                        //Serial.print(stationaryCount);
                        //Serial.println();
                        if(stationaryCount > (stop_time_ms/control_unit.getInterCheck())){
                            //Serial.println();
                            //Serial.print("HERE!!");
                            //Serial.println();
                            sensing_unit.rotation_counter.reset_distancecount();
                            stationaryCount = 0;
                            flag = 0;
                            save_wheel_side = 1;
                            lap_counter = 1;
                            direction_complete_counter++;
                            circle_number++;
                            control_unit.resetPID();
                        }
                    }
                }
            }
            
        }
    }
    
    
    void set_distance_to_travel(double dist){
        noOfMagnets = action_unit.convertDistanceToMag(dist);
        //Serial.println("Working?");
        //Serial.print("Magnets = ");
        //Serial.println(noOfMagnets);
        //Serial.println();
    }
    
    
};

#endif


