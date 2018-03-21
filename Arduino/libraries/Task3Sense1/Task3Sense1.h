#ifndef TASK3SENSE1_h
#define TASK3SENSE1_h


//The following is useful for use with the Visual Studio Projects provided.
#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#include "..\PushButton\PushButton.h"
#include "..\InterruptBasedSpeedMeasure\InterruptBasedSpeedMeasure.h"
#include "..\InterruptBasedInputs\InterruptBasedInputs.h"
#else
#include <Arduino.h>
#include <PushButton.h>
#include <InterruptBasedSpeedMeasure.h>
#include <InterruptBasedInputs.h>
#endif
//Have included libraries above which include classes of components we will use.
//The following library implements the sensing unit of the system.

class sense{
	protected:
        int ref_speed;                              //motor reference speed (rpm)
	public:
		InterruptSpeedMeasure rotation_counter;     //this object counts interrupts to measure speed
    
        sense(){                                    //default constructor
            ref_speed = 0;
		}
    
        void setup_sensing_unit(int speed, ArduinoInterruptNames interrupt, int magnets){     //sets up sensing unit components
            set_ref_speed(speed);
            setup_rotation_counter(interrupt, magnets);
        }
    
    
        void set_ref_speed(int speed){              //sets motor reference speed in rpm
            ref_speed = speed;
        }
		
    
        void setup_rotation_counter(ArduinoInterruptNames interrupt, int magnets){       //sets up interrupt pin (int0 = 0, int1 = 1)
            rotation_counter.setupSpeedMeasure(interrupt, magnets);
        }
    
    
        int return_ref_speed(){                     //returns motor reference speed in rpm
            return ref_speed;
        }
    
    
        double return_current_speed(){                                        //calculates the current motor speed sensed by the hall effect sensor
            double RPM = rotation_counter.getRPMandUpdate();
            if(RPM>=0){
                //Serial.print("Revs per min  = ");
                Serial.print(RPM);
            }
            else{
                //Serial.println("Reading speed failed     ");
            }
            
            return RPM;
        }
    
    
};

#endif


