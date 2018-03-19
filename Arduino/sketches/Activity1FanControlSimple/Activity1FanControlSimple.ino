#include <Task1SystemSimple.h>

#include <Task1SenseSimple.h>

#include <Task1ControlSimple.h>

#include <Task1ActionSimple.h>

#include <Arduino.h>

full_system the_system;

void setup() {
  int input_pin = A0;
  int output_pin = 11;
  int check_interval = 500;

  the_system.setup_sensing(input_pin);  //set up sensing unit

  the_system.setup_action(output_pin);  //set up action unit

  the_system.setup_control(check_interval);  //set up control unit
  
 Serial.begin(9600);
}

void loop() {
  the_system.execute_system_task();  //execute task
}
