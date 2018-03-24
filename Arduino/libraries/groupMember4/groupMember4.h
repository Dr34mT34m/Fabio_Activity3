#ifndef GROUPMEMBER4_h
#define GROUPMEMBER4_h

//include ultrasound sensor

//The following is useful for use with the Visual Studio Projects provided.
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#include "..\Task3System1\Task3System1.h"
#include "..\UltrasoundSensor\UltrasoundSensor.h"

#else
#include <Arduino.h>
#include <Task3System1.h>
#include <UltrasoundSensor.h>

#endif

#define FORWARDS  1
#define BACKWARDS 2
#define STOP_TIME_MS 2000

//Have included libraries above which include classes of components we will use.
//The following library implements the full system of the project.

// Derived class
//class Child : public Parent


class groupMember4 : motorsystem{
	//motorsystem motor;
	motorsystem Right_Wheel, Left_Wheel;
    

    public:
    PingProximitySensor car_sensor;
    
//Setup Left Motor
  int motorpwmpinLeft = 6;
  int motordirectionpinLeft = 8;
  ArduinoInterruptNames interruptpinLeft = 1;
  double ref_kp_Left = 0.6; //0.6        //1, 0.4, 10
  double ref_ki_Left = 0.1; //0.1  
  double ref_kd_Left = 40;  //50
  int ref_control_interval_time_Left = 125;
  int check_interval_Left = 250; 
  int ref_speed_Left = 150;
  int dirLeft = FORWARDS; 
  int magnetsLeft = 49;
  int distanceToTravelLeft = 79; //4000

  //Setup Right Motor
  int motorpwmpinRight = 5;      
  int motordirectionpinRight = 7;  
  ArduinoInterruptNames interruptpinRight = 0;
  double ref_kp_Right = 0.4;  //0.6      //1, 0.4, 10
  double ref_ki_Right = 0.1;  //0.1
  double ref_kd_Right = 40;   //50
  int ref_control_interval_time_Right = 125; 
  int check_interval_Right = 250;
  int ref_speed_Right = 150;
  int dirRight = FORWARDS;
  int magnetsRight = 49;
  int distanceToTravelRight = 79;   //distance in mm //4000 //79MM = 90 degree turn
  
   // the following initializes the sensor (only needed once) 
  int echosensorpin=9; 
  int triggersensorpin=10;

  //distance measured by sensor
  int measured_distance;
  bool measure_sucessful;
  
  //flags for turning function
  bool isTurning = false;
    
    void avoid_setup(){
    
    	//right wheel setup
  		Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
 		Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
  		Right_Wheel.setup_control(check_interval_Right, ref_kp_Right, ref_ki_Right, ref_kd_Right, ref_control_interval_time_Right);

  		//left wheel setup
  		Left_Wheel.setup_sensing(ref_speed_Left, interruptpinLeft, magnetsLeft);
 		Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
  		Left_Wheel.setup_control(check_interval_Left, ref_kp_Left, ref_ki_Left, ref_kd_Left, ref_control_interval_time_Left);
  		
  		//setup sensor
  		car_sensor.setup_PingProximitySensor(echosensorpin, triggersensorpin); 
    
    }
    
    void avoid_update(){
    
    if (millis()%5000){
  		measure_sucessful = car_sensor.SenseDistance(measured_distance);
    }
    
    //if doing a 90 degree turn
    
    if(isTurning){
    
    	//check if we still want to keep turning
    
    	if(Right_Wheel.sensing_unit.rotation_counter.checkDistanceMet((long int)30) == true){
      				
      					Right_Wheel.switchOff();
      					Left_Wheel.switchOff();
      					isTurning = false;
      					Right_Wheel.sensing_unit.rotation_counter.reset_distancecount();
      					//Serial.println("turning met");
            	
            	}
    
    }else{

   if(measure_sucessful){
  		//Serial.println(measured_distance);
  		
  		if((measured_distance>=100)){ //if distance is greater than 100 set speed to 150
  			//Serial.println("greater than 100");

    		int ref_speed_Right = 125;
    		int ref_speed_Left = 125;
    
   			dirRight = FORWARDS;
			dirLeft = FORWARDS;
    		Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
    		Left_Wheel.setup_sensing(ref_speed_Left, interruptpinRight, magnetsRight);
    		Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
    		Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
    
    			if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){   //operates car in straight line at set speed
      				Right_Wheel.execute_system_task_straight_line();
      				Left_Wheel.execute_system_task_straight_line(); 
    				}
 
  			}
  		
  		if((measured_distance<100) && (measured_distance>=50)){ //if distance is between 100 and 50 set speed to 100
  			//Serial.println("100 - 50");

    		int ref_speed_Right = 100;
    		int ref_speed_Left = 100;
    
   			dirRight = FORWARDS;
			dirLeft = FORWARDS;
    		Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
    		Left_Wheel.setup_sensing(ref_speed_Left, interruptpinRight, magnetsRight);
    		Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
    		Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
    
    			if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){   //operates car in straight line at set speed
      				Right_Wheel.execute_system_task_straight_line();
      				Left_Wheel.execute_system_task_straight_line(); 
    				}
 
  			}
  			
  		if((measured_distance<=50)){ //if less than 50 stop
  			//Serial.println("stop");

    		int ref_speed_Right = 200;
    		int ref_speed_Left = 200;
    
   			dirRight = FORWARDS;
			dirLeft = FORWARDS;
    		Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
    		Left_Wheel.setup_sensing(ref_speed_Left, interruptpinRight, magnetsRight);
    		Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
    		Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
    
    			if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){   //operates car in straight line at set speed
      				
      				if(isTurning ==false){
      					Right_Wheel.switchOff();
      					Left_Wheel.switchOff();
      					
      					Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, FORWARDS, distanceToTravelRight);
    					Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, BACKWARDS, distanceToTravelLeft);
      				
      					Right_Wheel.sensing_unit.rotation_counter.reset_distancecount();
      					
      					Right_Wheel.execute_system_task_straight_line();
      					Left_Wheel.execute_system_task_straight_line();
      					isTurning = true;
      					//Serial.println("turning set to true");
      				}
 
  			}
  			
  			}
  			
  		
    }else{
    
    	//Serial.println("ping failed");
    	
    	int ref_speed_Right = 125;
    		int ref_speed_Left = 125;
    
   			dirRight = FORWARDS;
			dirLeft = FORWARDS;
    		Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
    		Left_Wheel.setup_sensing(ref_speed_Left, interruptpinRight, magnetsRight);
    		Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
    		Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
    
    			if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){   //operates car in straight line at set speed
      				Right_Wheel.execute_system_task_straight_line();
      				Left_Wheel.execute_system_task_straight_line(); 
    				}
    }
    
	}
	}
	
};
    
                
#endif
