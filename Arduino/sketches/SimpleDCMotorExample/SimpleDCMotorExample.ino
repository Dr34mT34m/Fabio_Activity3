//#include <Arduino.h>
#include <IntervalCheckTimer.h>
#include <InterruptBasedSpeedMeasure.h>
#include <DCmotor.h>

HBridgeDCmotor hbdcmotor;

InterruptSpeedMeasure rotation_counter;
IntervalCheckTimer speed_check, speed_change, dir_change;


//FV
int target_index;
//

void setup()
{
  int motorpin=5;// PWM
  int directionpin=7;// digital ON/OFF for the direction
  hbdcmotor.setup_HBridgeDCmotor(motorpin,directionpin);
  //hbdcmotor.enableEcho();
  
  // jump start the motor
  bool jumpstartmotor=true;
  if(jumpstartmotor)
  {
    hbdcmotor. set_jumpstart(jumpstartmotor);
  }
    int initial_speed_pwm=120;
    hbdcmotor.start();
    hbdcmotor.setSpeedPWM(initial_speed_pwm);
    

  
  // enable the interrupt (int_0 works via pin2)
  // enable the interrupt (int_1 works via pin3)
  rotation_counter.setupSpeedMeasure(int_0);

  // timer to perform speed measurement and control at given interval:
  // set the time between speed measurements/control)
  int speed_measurement_ms=1000;  
  speed_check.setInterCheck(speed_measurement_ms);
  
  unsigned long int speed_change_ms=10000;
  speed_change.setInterCheck(speed_change_ms);
  
  unsigned long int dir_change_ms=90000;
  dir_change.setInterCheck(dir_change_ms);
    
  Serial.begin(9600);  
  
  target_index=-1;
  
  
}

void loop()
{
  const int tot_speed_vals=8;
  int pwmspeedval[tot_speed_vals]={55, 80, 90, 100, 105, 95, 75, 60};

  //const int tot_speed_vals=4;
  //int pwmspeedval[tot_speed_vals]={65, 65, 65, 65};
  
  int speed_pwm;
  
  if(speed_check.isMinChekTimeElapsedAndUpdate())
  {
    double RPM=rotation_counter.getRPMandUpdate();
    if(RPM>=0)
    {
      Serial.print(" Revs per min  = ");
      Serial.println(RPM);
    }
    else
    {
      Serial.println(" Reading speed failed");
    } 
  }
  
  if(speed_change.isMinChekTimeElapsedAndUpdate())
  {
    target_index++;
    if(target_index>=tot_speed_vals)
      target_index=0;

    speed_pwm=pwmspeedval[target_index];
    
    Serial.print("Setting PWM to ");
    Serial.println(speed_pwm);
    hbdcmotor.setSpeedPWM(speed_pwm);
    //Serial.print("  PWM set to ");
    //Serial.println(hbdcmotor.getSpeedPWM());
  }
 
 
 
    if(dir_change.isMinChekTimeElapsedAndUpdate())
    {
        Serial.println("-->Changing direction");
        hbdcmotor.stop();
        delay(1500);
        hbdcmotor.changedir();
        hbdcmotor.start();
        hbdcmotor.setSpeedPWM(speed_pwm);
    }
    
  
}
