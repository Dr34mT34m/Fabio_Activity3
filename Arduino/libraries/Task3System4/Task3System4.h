#ifndef TASK3SYSTEM4_h
#define TASK3SYSTEM4_h

//The following is useful for use with the Visual Studio Projects provided.
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#include "..\Task3System3\Task3System3.h"
#else
#include <Arduino.h>
#include <Task3System3.h>
#endif
//Have included the library that will be inherited for this class.


class closed_trajectory: public circular_operation{
	protected:
        int straight_count;
        int segment_count;
	public:
    
    closed_trajectory(int wheel_side): circular_operation(wheel_side){
        straight_count = 1;
        segment_count = 0;
    }
    
    
    void execute_system_task_closed_trajectory(double straight_speed, double turn_speed, double straight_dist, double radius){ //straight_dist = 125, radius = 250
        if (segment_count<18){ //18 segments equals 3 laps
            //Serial.print("Segment count = ");
            //Serial.println(segment_count);
            if (straight_count<2 && control_unit.isTimeToTakeMeasurementAndUpdate()){   //both wheels go straight at straight_speed for 12.5cm
                sensing_unit.set_ref_speed(straight_speed);
                set_distance_to_travel(straight_dist);
                action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
                //Serial.print("Straight count = ");
                //Serial.println(straight_count);
                if (sensing_unit.rotation_counter.checkDistanceMet((long int)noOfMagnets)/* == true*/){
                    straight_count++;
                    segment_count++;
                    sensing_unit.rotation_counter.reset_distancecount();
                    //Serial.println("RESET STRAIGHT DISTANCE");
                    //Serial.println();
                }
            }
            else if (straight_count == 2 && control_unit.isTimeToTakeMeasurementAndUpdate()){   //carry out a turn for half a circumference with radius 25cm
                radiusR = radius + 50;
                radiusL = radius - 50;
                circumferenceR = 2*3.14*radiusR;
                circumferenceL = 2*3.14*radiusL;
                speedR = (turn_speed*(radius+50))/radius;
                speedL = (turn_speed*(radius-50))/radius;
                speedR = speedR*1.02;   //2% increase to account for lateral friction
                speedL = speedL*0.98;   //2% decrease to account for lateral friction
                    
                if (save_wheel_side == 1){
                    sensing_unit.set_ref_speed((int)speedR);
                    set_distance_to_travel(0.5*circumferenceR);
                    action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
                    //Serial.println("Performing turn");
                    if(sensing_unit.rotation_counter.checkDistanceMet((int)noOfMagnets)/* == true*/){
                        straight_count = 0;
                        segment_count++;
                        sensing_unit.rotation_counter.reset_distancecount();
                        //Serial.println("RESET TURN DISTANCE");
                        //Serial.println();
                    }
                }
                
                if (save_wheel_side == 2){
                    sensing_unit.set_ref_speed((int)speedL);
                    set_distance_to_travel(0.5*circumferenceL);
                    action_unit.operate_motor(control_unit.return_PID_output(sensing_unit.return_ref_speed(), sensing_unit.return_current_speed()));
                    //Serial.println("Performing turn");
                    if(sensing_unit.rotation_counter.checkDistanceMet((int)noOfMagnets)/* == true*/){
                        straight_count = 0;
                        segment_count++;
                        sensing_unit.rotation_counter.reset_distancecount();
                        //Serial.println("RESET TURN DISTANCE");
                        //Serial.println();
                    }
                }
            }
        }
        else{   //stop the car
            sensing_unit.set_ref_speed(0);
            action_unit.switch_off_motor();
            //Serial.println("TURN MOTOR OFF");
        }
    }
    
};

#endif


