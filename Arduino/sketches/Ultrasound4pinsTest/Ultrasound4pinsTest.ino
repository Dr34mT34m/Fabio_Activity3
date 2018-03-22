#include <UltrasoundSensor.h>

// define an object of type ParallaxPingProximitySensor
PingProximitySensor test_sensor;



void setup()
{
  Serial.begin(9600);
  
  // the following initializes the sensor (only needed once) 
  int echosensorpin=9; 
  int triggersensorpin=10;
  test_sensor.setup_PingProximitySensor(echosensorpin, triggersensorpin);  
  
  // this initializes the serial comms (USB) used to print to screen

}

void loop()
{
  // the following performs ultrasound measurement and stores distance
  // in variable "measured_distance" (passed by reference); 
  // the returned boolean value can be used to verify that the sensor worked correctly    
  int waittimems=1000;
  int measured_distance; 
  boolean measurment_success; 
  
  Serial.println("Attempt measuremnt");
  measurment_success=test_sensor.SenseDistance(measured_distance);
    
  
    if(measurment_success)
    {
      //Serial.println("");
      Serial.print("Distance = ");
      Serial.println(measured_distance);
      Serial.println("");
    }
    else
      Serial.println("Measurement Failed");
    
    //prev_measured=measured_distance;
    
    //delay(waittimems);
 
}





//if(measurment_success && ((measured_distance<=close_distance) || (measured_distance<(prev_measured))) )
