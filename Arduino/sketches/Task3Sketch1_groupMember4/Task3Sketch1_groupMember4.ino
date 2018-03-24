#include <groupMember4.h>

//#include <Task3System1.h>

// this is used to count the interrupts
groupMember4 avoidSystem;

void setup() {
  
  avoidSystem.avoid_setup();
  //Enable Serial Monitor
  Serial.begin(2000000);
  
}

void loop() {
  
  avoidSystem.avoid_update();

}


