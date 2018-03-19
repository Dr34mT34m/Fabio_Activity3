#include <PushButton.h>
#include <Basic_Input.h>

// this is needed for the timers
#include <DCmotor.h>


// this object holds a collection of push buttons
// (input pullup enabled: no external resistor required)
inputs motor_pushbuttons;

// this is the potentiometer
in_analog motor_potentiometer;

// these help us read  inputs at appropriate intervals
IntervalCheckTimer buttons_check;
IntervalCheckTimer potentiom_check;


void setup()
{
  // these are temporary variables of type in_push_button,
  // the contructor is called with pin_number and "button label"
  // the "label" is one from the command_list_enum in 
  // file Basic_Input.h. more can be added (separate by comma)
  //
  // For each Button: connect a pin on a corner to ground
  // and the pin on diagonally-opposite corner to a digital pin
  //
  
  int start_pin=8;
  int stop_pin=10;
  int reverse_pin=12;
  
  in_push_button start_but(start_pin,switch_on);
  in_push_button stop_but(stop_pin,switch_off);
  in_push_button reverse_but(reverse_pin,change_spin_dir);
  
  // alternatively try this
  unsigned long int mininterval_ms=2000;
  //in_push_button start_but(start_pin,switch_on, mininterval_ms);
  //in_push_button stop_but(stop_pin,switch_off, mininterval_ms);
  //in_push_button reverse_but(reverse_pin,change_spin_dir, mininterval_ms);
  
  
  
  // the temp buttons are now copied into motor_pushbuttons
  motor_pushbuttons.add_in_push_button(start_but);
  motor_pushbuttons.add_in_push_button(stop_but);
  motor_pushbuttons.add_in_push_button(reverse_but);
  //
  
  // initialize the potentiometer pin to an Analog input pin
  motor_potentiometer.setup_in_analog(A3);
  
  // set the time (ms) between input checks
  //buttons
  buttons_check.setInterCheck(100);
  // potentiometer
  potentiom_check.setInterCheck(1000);

  
  // to print to screen
  Serial.begin(9600);
  
  
}


void loop()
{
  // check buttons as often as needed
  if(buttons_check.isMinChekTimeElapsedAndUpdate())
  {
    command_list_enum in_smpl_cmd;
    boolean success;
    
    //Serial.println("Checking buttons");
    
    
    // verify the  new input. 
    // This is the "label" of the button that was pressed;
    // it is written to "in_smpl_cmd"
    success=motor_pushbuttons.check_n_get_command(in_smpl_cmd);
    // success is false if buttons were not initialized 
    
    
    
    if(success)
    {
      //Serial.println("");
      switch (in_smpl_cmd){
        case switch_on:
        Serial.println(" Start button pressed");
        break;
        case switch_off:
        Serial.println("    Stop button pressed");  
        break;
        case change_spin_dir:
        Serial.println("        Reverse button pressed");
        break;
        default:
          Serial.println("Unknown button pressed");
          ;
      }
      //Serial.println("");
    }
    
  }
  
 // check the potentiometer when needed 
 if(potentiom_check.isMinChekTimeElapsedAndUpdate())
  {
    int readval;
    boolean success;
    
    //Serial.println("Checking potentiometer");
    
    // verify the  new input. This is written to "readval"
    success=motor_potentiometer.read_input(readval);
    // success = false if setup_in_analog() was not done 
    if(success)
    {
      Serial.print("Potentiometer Input value = ");
      Serial.println(readval);
    }
    
  }
  
}
