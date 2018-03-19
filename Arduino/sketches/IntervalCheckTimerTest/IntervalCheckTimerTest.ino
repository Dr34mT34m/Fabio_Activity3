#include <Basic_Input.h>
#include "IntervalCheckTimer.h"

IntervalCheckTimer TimerTest;

in_serial serial_input;

void setup() {
  // put your setup code here, to run once:
  unsigned long inp_inter_check_ms=2000;
  TimerTest.setInterCheck(inp_inter_check_ms);
  
  Serial.begin(9600); 

}

void loop() {
  int new_val;
  bool input_found;
  
  if( TimerTest.isMinChekTimeElapsedAndUpdate() )
  {
    Serial.print("Time elapsed (ms): ");
    Serial.println(TimerTest.getInterCheck()); 
    Serial.println("Action performed as often as needed.");
    Serial.println();
  }
  
  // any code outside the if branch is executed 
  // during all loop interations.  
  // This is more efficient that using delay()
  
  // For example: check the serial input for a 
  // new value of inp_inter_check_ms
  input_found=serial_input.read_input(new_val);
  if(input_found)
      TimerTest.setInterCheck(new_val);
  
  
}
