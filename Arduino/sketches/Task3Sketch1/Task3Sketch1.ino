//will test
#include <Task3System1.h>

#define FORWARDS  1
#define BACKWARDS 2

motorsystem Right_Wheel, Left_Wheel;

// this is used to count the interrupts

void setup() {

  //Gain values from previous task for reference
  //double ref_kp = 1;     
  //double ref_ki = 0.4;   
  //double ref_kd = 110;   


  //Setup Right Motor
  int motorpwmpinRight = 5;      
  int motordirectionpinRight = 7;  
  ArduinoInterruptNames interruptpinRight = 0;
  double ref_kp_Right = 1;   //1, 0.4, 10
  double ref_ki_Right = 0.4;  
  double ref_kd_Right = 10; 
  int ref_control_interval_time_Right = 150; 
  int check_interval_Right = 250;
  int ref_speed_Right = 175;
  //int max_rpm_Right = 6000;
  int dirRight = FORWARDS;
  int magnetsRight = 49;
  //int Magnets_passed_Right = 1000;  //used to determine distance
  int distanceToTravelRight = 1000;

  Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
  Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
  Right_Wheel.setup_control(check_interval_Right, ref_kp_Right, ref_ki_Right, ref_kd_Right, ref_control_interval_time_Right/*, max_rpm_Right*/);


  //Setup Left Motor
  int motorpwmpinLeft = 6;
  int motordirectionpinLeft = 8;
  ArduinoInterruptNames interruptpinLeft = 1;
  double ref_kp_Left = 1.1;     //1, 0.4, 10
  double ref_ki_Left = 0.4;   
  double ref_kd_Left = 10;
  int ref_control_interval_time_Left = 150;
  int check_interval_Left = 250; //300
  int ref_speed_Left = 175;
  //int max_rpm_Left = 6000;
  int dirLeft = FORWARDS; 
  int magnetsLeft = 49;
  //int Magnets_passed_Left = 1000;  //used to determine distance
  int distanceToTravelLeft = 1000;
  Left_Wheel.setup_sensing(ref_speed_Left, interruptpinLeft, magnetsLeft);
  Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
  Left_Wheel.setup_control(check_interval_Left, ref_kp_Left, ref_ki_Left, ref_kd_Left, ref_control_interval_time_Left/*, max_rpm_Left*/);
  

  //Enable Serial Monitor
  Serial.begin(9600);
  
}

void loop() {
  /*if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){   //operates car in straight line at set speed
    Right_Wheel.execute_system_task_straight_line();
    Serial.print(",");
    Left_Wheel.execute_system_task_straight_line(); 
    Serial.println("");
  }*/

  if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement())    //drives car forward for set distance, stops then reverses for set distance
  {     //drives car forward for set distance, stops then reverses for set distance
    Right_Wheel.execute_system_task_distance_return();
    Serial.print(",");
    Left_Wheel.execute_system_task_distance_return(); 
    Serial.println("");
  }
}
