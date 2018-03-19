#ifndef INTERBSDINP_H
#define INTERBSDINP_H

#ifdef COMPILE_CPP_NOT_ARDUINO                                                          /********** ADDED THIS **********/
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"       /********** ADDED THIS **********/
//#include <Arduino.h>
#else                                                                                   /********** ADDED THIS **********/
#include <Arduino.h>                                                                    /********** ADDED THIS **********/
#endif                                                                                  /********** ADDED THIS **********/



// The table below shows the available interrupt pins on various boards.
//	    interrupt:	int.0	int.1	int.2	int.3	int.4	int.5
// board
// Uno, Ethernet	2		3	 	 	 	 
// Mega2560			2		3		21		20		19		18



// Note: for "ArduinoUno" only int_0 and int_1 are supported
// if you want to restrict this to work with int_0 and int_1 only, uncomment the line below:
//#define USING_UNO 1


const int MAX_INTERRUPTS=6;
enum ArduinoInterruptNames{
	int_0=0, int_1, int_2, int_3, int_4, int_5,
	int_max_plus_1
};


// The following are the ISRs (Interrupt Service Routines). Arduino ISRs have limitations: 
// ISRs cannot have parameters and shouldn't return anything; ISRs should be short and fast.
// Only one ISR can run at a time, other interrupts are ignored until the current one is finished.
// Delay(), millis() and others will not work while an ISR is running.
// Typically global variables are used to pass data between an ISR and the main program. 
// To make sure variables used in an ISR are updated correctly, declare them as volatile.



// array modified by the ISRs: each ISR accesses only one element of the array 
// variable has to be declared (and set to zero) at the global scope level

// this array is used (and reset) by objects that measure speed
volatile unsigned long int inter_count_array[MAX_INTERRUPTS]={0, 0, 0, 0, 0, 0};

// this array is used (and reset) by objects that measure distance travelled (independently form the one above)
volatile unsigned long int inter_distancecount_array[MAX_INTERRUPTS]={0, 0, 0, 0, 0, 0};


//These are the ISRs considered for this class: they all lincrement a counter
void increase_count_int0()
{
  //noInterrupts();// not needed; ISR are inherently disabled inside an interrupt.
  inter_count_array[int_0]++;
  //
  inter_distancecount_array[int_0]++;
  //
  //interrupts();
}
void increase_count_int1()
{
  //noInterrupts();
  inter_count_array[int_1]++;
  //
  inter_distancecount_array[int_1]++;
  //
  //interrupts();
}
void increase_count_int2()
{
  //noInterrupts();
  inter_count_array[int_2]++;
  //
  inter_distancecount_array[int_2]++;
  //
  //interrupts();
}
void increase_count_int3()
{
  //noInterrupts();
  inter_count_array[int_3]++;
  //
  inter_distancecount_array[int_3]++;
  //
  //interrupts();
}
void increase_count_int4()
{
  //noInterrupts();
  inter_count_array[int_4]++;
  //
  inter_distancecount_array[int_4]++;
  //
  //interrupts();
}
void increase_count_int5()
{
  //noInterrupts();
  inter_count_array[int_5]++;
  //
  inter_distancecount_array[int_5]++;
  //
  //interrupts();
}



class InterruptCount{
protected:	
	ArduinoInterruptNames interrname;
	boolean initialized;
	
	
	// interrupt mode
	int interr_mode;

	int interrupt_pin;
	
	// this tracks when the count was last cheched 
	unsigned long last_check_time_ms;
	//
	
	// this will be used to check pin availability: still work in progress
	//pinchecker pincheck;
	
	void reset_interrupt_count()
	{
		if(isInitialized())
		{
			// reset the count for the correponding interrupt;
			inter_count_array[interrname]=0;	
			// update the time
			last_check_time_ms=millis();
		}
	}


		// attach interrupt
	void attach_my_interrupt(int mode)
	{
		if(isInitialized())
		{
			// make sure interrupts are enabled (this should be redundant, but does no harm)
			interrupts();
			
			//
			interr_mode=mode;

			// options: 
			//LOW to trigger the interrupt whenever the pin is low,
			//CHANGE to trigger the interrupt whenever the pin changes value
			//RISING to trigger when the pin goes from low to high,
			//FALLING for when the pin goes from high to low.
			switch(interrname){
			case int_0:
				attachInterrupt(int_0, increase_count_int0, mode);
				break;
			case int_1:
				attachInterrupt(int_1, increase_count_int1, mode);
				break;
			case int_2:
				attachInterrupt(int_2, increase_count_int2, mode);
				break;
			case int_3:
				attachInterrupt(int_3, increase_count_int3, mode);
				break;
			case int_4:
				attachInterrupt(int_4, increase_count_int4, mode);
				break;
			case int_5:
				attachInterrupt(int_5, increase_count_int4, mode); //possible typo
				break;
			default:
				break;
			}
		}
	}
	
	int associatePinNumToInterruptName(ArduinoInterruptNames in_interrname)
	{
		int required_pin=-1;
		switch(interrname){
		case int_0:
			required_pin=2;
			break;
		case int_1:
			required_pin=3;
			break;
		case int_2:
			required_pin=21;
			break;
		case int_3:
			required_pin=20;
			break;
		case int_4:
			required_pin=19;
			break;
		case int_5:
			required_pin=18;
			break;
		default:
			break;
		};

		
		return required_pin;
	}
	
	// associate the required pin to the interrupt
	int getRequiredPin(ArduinoInterruptNames in_interrname)
	{
		int required_pin=associatePinNumToInterruptName(in_interrname);
		
		// this is work in progress:
		//int assigned_pin=pincheck.checkNset_digpin(required_pin);
		//return assigned_pin;
		
		return required_pin;
	}

	// reserves the required pin, if not in use already
	boolean PinIsAvailable(ArduinoInterruptNames in_interrname)
	{
		int required_pin;

		required_pin=checkRequiredPin(in_interrname);

		if(required_pin>=0)
			return true;
		else
			return false;

	}
	//
	
	
	void detach_my_Interrupt()
	{
		if(isInitialized())
		{	
			//int assigned_pin=checkRequiredPin(interrname);
			// release the pin 
			//pincheck.reset_digitpin(assigned_pin);
			// detach the interrupt

			detachInterrupt(interrname);
		}
	}

	void reattach_my_Interrupt()
	{
		if(isInitialized())
		{	
			attach_my_interrupt(interr_mode);
		}
	}

	// get interrupt count since last reset 
	unsigned long int get_interrupt_count()
	{
		if(isInitialized())
			return ( inter_count_array[interrname] );	
		else
			return 0;
	}

	

	// get interrupt count since last reset 
	unsigned long int get_interrupt_distancecount()
	{
		if(isInitialized())
			return ( inter_distancecount_array[interrname] );	
		else
			return 0;
	}


public:
	// default constructor
	InterruptCount(){initialized=false;}
	
	// construct + initialize
	InterruptCount(ArduinoInterruptNames in_interrname, int mode=FALLING)
	{
		initialized=false; 
		setup_InterruptCount(in_interrname, mode);
	}
	
	// destructor
	~InterruptCount()
	{
		if(isInitialized())
		{
			// detach the interrupt
			detach_my_Interrupt();

			// decrease the count
			//population_count_array[interrname]--;			
		}
	}
	//
	inline boolean isInitialized() {return initialized;}
	
	// initialize and attach interrupt
	void setup_InterruptCount(ArduinoInterruptNames in_interrname, int mode=FALLING)
	{
		int local_int_max_plus_1=int_max_plus_1;

#ifdef USING_UNO
		// for UNO board the max interrupt supported is int_1
		local_int_max_plus_1=(int_1 +1);
#endif

		// check the object is not intialized already (and that the interrupt name is valid)
		if(!isInitialized() && (in_interrname>=int_0) && (in_interrname<local_int_max_plus_1))
		{
			// check that the required interrupt pin is available;
			if(PinIsAvailable(in_interrname))
			{
				// set the name
				interrname=in_interrname;
				
				// reserve the pin
				interrupt_pin=getRequiredPin(interrname);

				// now it is initialized
				initialized=true;
				
				// reset interrupt count: ready for first use
				reset_interrupt_count();
				
				// attach interrupt (ISR) to the given function (and right pin)
				attach_my_interrupt(mode);
			}
		}
	}

	
	// get interrupt count and period since last reset, then reset 
	boolean checkNupdate(unsigned long int& count, unsigned long int& interval_ms)
	{
		boolean success=false;
		if(isInitialized())
		{
			// detach the interrupt:
			// so that the programis not interrupted while the computation is made 
			//detach_my_Interrupt();

			// read the count;
			count=get_interrupt_count();
			
			// caclulate the interval
			interval_ms = millis() - last_check_time_ms;

			// reset counter and update time
			reset_interrupt_count();

			//
			success=true;


			// reattach the interrupt:			
			//reattach_my_Interrupt();
		}

		return success;
	}
	
	// this is the count from the second (independent) counter 
	bool checkDistanceMet(int target_count)
	{
		unsigned long int curr_count_long;
		boolean ditance_met=true;
		unsigned long int target_count_long = (unsigned long int)target_count;
		if(isInitialized())
		{
			// read the count;
			curr_count_long=get_interrupt_distancecount();
			
			if(curr_count_long>=target_count_long)
			{
				// reset counter
				//reset_interrupt_distancecount();
				ditance_met=true;
			}
			else
				ditance_met=false;
		}

		return ditance_met;
	}

	int GetkDistanceCount()
	{
		int ditance=0;
		if(isInitialized())
		{
			// read the count;
			unsigned long int curr_count_long=get_interrupt_distancecount();
			
			// convert to int (this is temporary: it works for short distances)
			ditance=(int)curr_count_long;

		}

		return ditance;
	}

	void reset_interrupt_distancecount()
	{
		if(isInitialized())
		{
			// reset the count for the correponding interrupt;
			inter_distancecount_array[interrname]=0;
		}
	}
	//


	int checkRequiredPin(ArduinoInterruptNames in_interrname)
	{
		// this function should chek that the pin has not been reserved already (but not ask to reserve it, yet).
		// for the time being, I do not reserve pins anyway, so this will do (temporary)
		return associatePinNumToInterruptName(in_interrname);
	}

};


/// ========================================





#endif
