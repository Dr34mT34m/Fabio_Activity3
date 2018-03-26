#include <Task3System4.h>

#define FORWARDS  1
#define BACKWARDS 2
#define STOP_TIME_MS 2000
#define RIGHT_WHEEL 1
#define LEFT_WHEEL 2

closed_trajectory Right_Wheel(RIGHT_WHEEL), Left_Wheel(LEFT_WHEEL);

void setup() {

  //Setup Right Motor
  int motorpwmpinRight = 5;      
  int motordirectionpinRight = 7;  
  ArduinoInterruptNames interruptpinRight = 0;
  double ref_kp_Right = .9;        
  double ref_ki_Right = .4;   
  double ref_kd_Right = 0;    
  int ref_control_interval_time_Right = 100; 
  int check_interval_Right = 100; //250
  int ref_speed_Right = 0;
  int dirRight = FORWARDS;
  int magnetsRight = 49;
  int distanceToTravelRight = 0;   

  Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
  Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
  Right_Wheel.setup_control(check_interval_Right, ref_kp_Right, ref_ki_Right, ref_kd_Right, ref_control_interval_time_Right);


  //Setup Left Motor
  int motorpwmpinLeft = 6;
  int motordirectionpinLeft = 8;
  ArduinoInterruptNames interruptpinLeft = 1;
  double ref_kp_Left = .8;        
  double ref_ki_Left = .35;           
  double ref_kd_Left = 0;             
  int ref_control_interval_time_Left = 100;
  int check_interval_Left = 100; 
  int ref_speed_Left = 0;
  int dirLeft = FORWARDS; 
  int magnetsLeft = 49;
  int distanceToTravelLeft = 0;
  
  Left_Wheel.setup_sensing(ref_speed_Left, interruptpinLeft, magnetsLeft);
  Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
  Left_Wheel.setup_control(check_interval_Left, ref_kp_Left, ref_ki_Left, ref_kd_Left, ref_control_interval_time_Left);
  

  //Enable Serial Monitor
  Serial.begin(9600);
  
}

void loop() {
  if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){     //drives car in a circular motion clockwise, then anti-clockwise
    Right_Wheel.execute_system_task_closed_trajectory(60, 60, 600, 250);                       
    Serial.print(",");                                                                   
    Left_Wheel.execute_system_task_closed_trajectory(60, 60, 600, 250);  
    Serial.println("");
  }
}
