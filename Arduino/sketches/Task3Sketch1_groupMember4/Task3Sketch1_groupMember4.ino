//will test
//#include <groupMember4.h>
#include <Task3System1.h>
#include <UltrasoundSensor.h>


#define FORWARDS  1
#define BACKWARDS 2
#define STOP_TIME_MS 2000

motorsystem Right_Wheel, Left_Wheel;
PingProximitySensor car_sensor;

// this is used to count the interrupts

//Setup Left Motor
  int motorpwmpinLeft = 6;
  int motordirectionpinLeft = 8;
  ArduinoInterruptNames interruptpinLeft = 1;
  double ref_kp_Left = 0.4; //0.6        //1, 0.4, 10
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

  //distance measured by sensor
  int measured_distance;
  bool measure_sucessful;

void setup() {

  //Gain values from previous task for reference
  //double ref_kp = 1;     
  //double ref_ki = 0.4;   
  //double ref_kd = 110;   

  Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
  Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
  Right_Wheel.setup_control(check_interval_Right, ref_kp_Right, ref_ki_Right, ref_kd_Right, ref_control_interval_time_Right);


  
  
  Left_Wheel.setup_sensing(ref_speed_Left, interruptpinLeft, magnetsLeft);
  Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
  Left_Wheel.setup_control(check_interval_Left, ref_kp_Left, ref_ki_Left, ref_kd_Left, ref_control_interval_time_Left);

  // the following initializes the sensor (only needed once) 
  int echosensorpin=9; 
  int triggersensorpin=10;
  car_sensor.setup_PingProximitySensor(echosensorpin, triggersensorpin); 
  
  //Enable Serial Monitor
  Serial.begin(2000000);
  
}

void loop() {
  
  if (millis()%3000){
  measure_sucessful = car_sensor.SenseDistance(measured_distance);
  }

  if(measure_sucessful){
  Serial.println(measured_distance);
  }
  
  if((measured_distance<=100) & measure_sucessful & (measured_distance>=20)){ 

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
      //Serial.print(",");
      Left_Wheel.execute_system_task_straight_line(); 
      //Serial.println("");
    }
 
  }else  if ((measured_distance>=100) | !measure_sucessful){
   int ref_speed_Right = 200;
    int ref_speed_Left = 200;
    
    dirRight = FORWARDS;
    dirLeft = FORWARDS;
    Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
    Left_Wheel.setup_sensing(ref_speed_Left, interruptpinRight, magnetsRight);
    Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
    Left_Wheel.setup_action(motorpwmpinLeft, motordirectionpinLeft, dirLeft, distanceToTravelLeft);
    
    if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){   //operates car in straight line at set speed
      Right_Wheel.execute_system_task_straight_line();
      //Serial.print(",");
      Left_Wheel.execute_system_task_straight_line(); 
      //Serial.println("");
    }
    
  }else  if ((measured_distance<=50)){
    
    if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){   //operates car in straight line at set speed
      Serial.print("Motor OFF");
      Right_Wheel.switchOff();
      //Serial.print(",");
      Left_Wheel.switchOff();
      //Serial.println("");
    }
  

  }

}


