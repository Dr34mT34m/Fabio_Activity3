#include <Arduino.h>
#include <Task1SystemAdditional.h>
#include <Task1SenseAdditional.h>
#include <Task1ControlMapChange.h>
#include <Task1ActionAdditional.h>


full_system the_system;

void setup() {
  int potentiometer_pin = A0;
  int push_button_pin = 2;
  int LED_indicator = 4;
  int fan_pin = 6;
  int LED1_pin = 8;
  int LED2_pin = 9;
  int LED3_pin = 10;
  int LED4_pin = 11;
  int LED5_pin = 12;
  int check_interval = 500;

  the_system.setup_sensing(potentiometer_pin, push_button_pin, LED_indicator);  //set up sensing unit

  the_system.setup_action(fan_pin, LED1_pin, LED2_pin, LED3_pin, LED4_pin, LED5_pin);  //set up action unit

  the_system.setup_control(check_interval);  //set up control unit
  
 Serial.begin(9600);
}

void loop() {
  the_system.execute_system_task();  //execute task
}
