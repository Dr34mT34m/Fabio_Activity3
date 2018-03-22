//will test
#include <groupMember4.h>
#include <UltrasoundSensor.h>


#define FORWARDS  1
#define BACKWARDS 2
#define STOP_TIME_MS 2000

motorsystem Right_Wheel, Left_Wheel;
PingProximitySensor car_sensor;

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
  double ref_kp_Right = 0.4;  //0.6      //1, 0.4, 10
  double ref_ki_Right = 0.1;  //0.1
  double ref_kd_Right = 40;   //50
  int ref_control_interval_time_Right = 125; 
  int check_interval_Right = 250;
  int ref_speed_Right = 150;
  int dirRight = FORWARDS;
  int magnetsRight = 49;
  int distanceToTravelRight = 79;   //distance in mm //4000 //79MM = 90 degree turn

  Right_Wheel.setup_sensing(ref_speed_Right, interruptpinRight, magnetsRight);
  Right_Wheel.setup_action(motorpwmpinRight, motordirectionpinRight, dirRight, distanceToTravelRight);
  Right_Wheel.setup_control(check_interval_Right, ref_kp_Right, ref_ki_Right, ref_kd_Right, ref_control_interval_time_Right);


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
  int measured_distance;
  car_sensor.SenseDistance(measured_distance);

  Serial.println(measured_distance);
  if((measured_distance>50) & (measured_distance!=0)){ 
    if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){   //operates car in straight line at set speed
      Right_Wheel.execute_system_task_straight_line();
      Serial.print(",");
      Left_Wheel.execute_system_task_straight_line(); 
      Serial.println("");
    }else{

      
    }
  }
  
  /*
  if (Right_Wheel.isTimeToTakeMeasurement() && Left_Wheel.isTimeToTakeMeasurement()){     //drives car forward for set distance, stops then reverses for set distance
    Right_Wheel.execute_system_task_distance_return(STOP_TIME_MS);                        //car initially starts with a surge in speed when plugged into laptop and using plotter,as PID initially reads the zero's before the switch is turned on
    //Serial.print(",");                                                                    //ensure car is disconnected from laptop when demonstrating
    Left_Wheel.execute_system_task_distance_return(STOP_TIME_MS); 
    //Serial.println("");
    
  }*/
}

void turn90(){

}

