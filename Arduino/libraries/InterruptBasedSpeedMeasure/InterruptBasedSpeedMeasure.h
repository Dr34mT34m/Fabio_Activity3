#ifndef INTERBSDMSR_H
#define INTERBSDMSR_H

#ifdef COMPILE_CPP_NOT_ARDUINO                                  /********** ADDED THIS **********/
#include "..\InterruptBasedInputs\InterruptBasedInputs.h"       /********** ADDED THIS **********/
//#include <..\InterruptBasedInputs\InterruptBasedInputs.h>
#else                                                           /********** ADDED THIS **********/
#include <InterruptBasedInputs.h>                               /********** ADDED THIS **********/
#endif                                                          /********** ADDED THIS **********/


// this object counts interrupts to derive speed (RPM)
class InterruptSpeedMeasure{
protected:	
	// this is used to count the interrupts
	InterruptCount rotation_counter;

	// number of magnets around the wheel
	int tot_interr_on_circle;

	// by default there are 6 magnets aroud the wheel
	static const int DEFAULT_INTER_ON_CIRCLE=6;
    //static const int DEFAULT_INTER_ON_CIRCLE=49;        //added for activity 3.
	//
	bool enabled;

public:
	InterruptSpeedMeasure()
	{
		tot_interr_on_circle=DEFAULT_INTER_ON_CIRCLE;
		enabled=false;
	}

	void setupSpeedMeasure(ArduinoInterruptNames in_interrname, int inp_interr_on_circle=DEFAULT_INTER_ON_CIRCLE)
	{
		// by default I assume falling-edge-triggered interrupts
		int mode=FALLING;
		
		// enable the pullup to the interrupt pin
		int pin_num=rotation_counter.checkRequiredPin(in_interrname);
		pinMode(pin_num, INPUT_PULLUP);

		// setup the interrupt
		rotation_counter.setup_InterruptCount(in_interrname, mode);

		// used to compute speed
		tot_interr_on_circle=max(inp_interr_on_circle,1);
		
		// now it's all enabled
		enabled=true;
	}

	// this function is defined "virtual" meaning that a derived class (see InterruptSpeedMeasure_SMA below) 
	// can re-define its own version (with the same name and input arguments).
	// You can ignore the word "virtual" here
	virtual double getRPMandUpdate()
	{
		if(enabled)
		{
			unsigned long int count;
			unsigned long int interval_ms;
			bool success;
			success=rotation_counter.checkNupdate(count, interval_ms);
			if(success)
			{
				//RPM = (count/tot_interr_on_circle) / (interval_ms/60000)
				
				// perform all the calculation with double 
				double revspermin_doub, factor_doub;
				
				revspermin_doub = (double)count;
				revspermin_doub/=tot_interr_on_circle;

				revspermin_doub/=((double)interval_ms);
				revspermin_doub*=60000;
				
				//factor_doub=((double)600000)/tot_interr_on_circle;
				//factor_doub/=((double)interval_ms);
				//revspermin_doub*=factor_doub;
				
				
				// return an int (the RPM will be a smallish number)
				//int revspermin=(int)revspermin_doub;

				return revspermin_doub;
			}
			else
				return -1;
		}
		else
			return -1;
	}

	// reset the second counter (used for distance)
	void reset_distancecount(){rotation_counter.reset_interrupt_distancecount();}

	int get_tot_interr_on_circle(){return tot_interr_on_circle;}
	bool checkDistanceMet(int target_count){return rotation_counter.checkDistanceMet(target_count);}
	int GetkDistanceCount(){return rotation_counter.GetkDistanceCount();}
};


// This is used as a compoment in the moving average below
// It stores elements into an array of size N. 
// The array is curcular: when you store the (N+1)th elemnt, the first one is overwritten 
// In a first instance, only worry about (understanding) the public functions
class intCircularArray{
protected:	
	
	// set to true when memory is allocated
	bool mem_allocated;
	
	// this holds the items, after allocaiton
	int *the_array;
	// size of the array;
	int array_size;

	// position to write to
	int curr_index;

	// tot elements written so far
	int tot_curr_elements;

	// this is to keep track of memory allocation failure
	bool mem_allocation_failed;

	static const int DEFAULT_SIZE=10;

	void setup_default()
	{
		mem_allocated=false;
		mem_allocation_failed=false;
		the_array=NULL;
		array_size=0;
		curr_index=-1;
		tot_curr_elements=0;
	}

	void release_array_mem()
	{
		if(mem_allocated && (the_array!=NULL) )
		{
			// release memory
			delete the_array;

			//
			array_size=0;
			the_array=NULL;
			mem_allocated=false;
		}
	}
	void updateIndexAndSize()
	{
		// increment the index
		curr_index++;
		
		// reset the index (get beack to the "beginning of the circle")
		if(curr_index>=array_size)
			curr_index=0;


		// increment count till it reaches the size
		if(tot_curr_elements<array_size)
			tot_curr_elements++;
	}
public:
	// constructor and distructor
	intCircularArray(){setup_default();}
	~intCircularArray(){release_array_mem();}

	// allocate an array of required size (use a default value otherwise)
	void allocate_array_mem(int inp_arraysize=DEFAULT_SIZE)
	{
		// only done if memory has not been allocated yet
		if(!mem_allocated && !mem_allocation_failed)
		{
			//Serial.print("Allocating array mem size ");
			//Serial.println(inp_arraysize);
			
			// ensure array size is at least 1.
			array_size=max(1, inp_arraysize);

			// allocate memory
			the_array=new int[array_size];

			// check there was enough memory: exit otherwise
			if(the_array==NULL)
			{
				array_size=0;
				mem_allocation_failed=true;
				return;
			}

			// ready for use
			curr_index=0;
			mem_allocated=true;
			mem_allocation_failed=false;
		}
	}

	void reset_array()
	{
		// only done if memory has not been allocated yet
		if(!mem_allocated && !mem_allocation_failed)
		{
			//Serial.println("Reset array content");
			
			// reset array to first index
			// and set to 0 the number of filled entries
			
			// ready for use
			curr_index=0;
			tot_curr_elements=0;
		}
	}


    // append an element to the (circular) array 
	bool append_item(int inp_item)
	{
		if(!mem_allocation_failed && !mem_allocated)
		{
			// the array has not been allocated yet;
			//allocate with default size
			allocate_array_mem();
		}

		if( mem_allocated && (getSize()>0) )
		{
			the_array[curr_index]=inp_item;
			updateIndexAndSize();
			return true;
		}
		else
			return false;
	}

	// sum of all elements in the array
	long int computeSum()
	{
		
		long int sum=0;
		// this does nothing if the size is 0
		for(int ind=0; ind<tot_curr_elements; ind++)
			sum+=the_array[ind];

		return sum;
	}

	int computeAvg()
	{
		double avg;
		long int sum=computeSum();
		
		if(tot_curr_elements>0)
			avg = ((double)sum)/tot_curr_elements;
		else
			avg = 0.0;

		return ( (int)avg );
	}

	// these are trivial to understand
	bool isAllocated(){return mem_allocated;}
	int getSize(){return array_size;}
	int getTotElem(){return tot_curr_elements;}


	/*// stuff used for debugging only
	//int getCurrIndex(){return curr_index;}
	void printcontent()
	{
		Serial.print("[");
		for(int ind=0; ind<tot_curr_elements; ind++)
		{
			Serial.print(the_array[ind]);
			Serial.print(" ");
		}
		Serial.print("]");
	}//*/

};

template <class T> class CircularArray{
protected:	
	
	// set to true when memory is allocated
	bool mem_allocated;
	
	// this holds the items, after allocaiton
	T *the_array;
	// size of the array;
	int array_size;

	// position to write to
	int curr_index;

	// tot elements written so far
	int tot_curr_elements;

	// this is to keep track of memory allocation failure
	bool mem_allocation_failed;

	static const int DEFAULT_SIZE=10;

	void setup_default()
	{
		mem_allocated=false;
		mem_allocation_failed=false;
		the_array=NULL;
		array_size=0;
		curr_index=-1;
		tot_curr_elements=0;
	}

	void release_array_mem()
	{
		if(mem_allocated && (the_array!=NULL) )
		{
			// release memory
			delete the_array;

			//
			array_size=0;
			the_array=NULL;
			mem_allocated=false;
		}
	}
	void updateIndexAndSize()
	{
		// increment the index
		curr_index++;
		
		// reset the index (get beack to the "beginning of the circle")
		if(curr_index>=array_size)
			curr_index=0;


		// increment count till it reaches the size
		if(tot_curr_elements<array_size)
			tot_curr_elements++;
	}
public:
	// constructor and distructor
	CircularArray(){setup_default();}
	~CircularArray(){release_array_mem();}

	// allocate an array of required size (use a default value otherwise)
	void allocate_array_mem(int inp_arraysize=DEFAULT_SIZE)
	{
		// only done if memory has not been allocated yet
		if(!mem_allocated && !mem_allocation_failed)
		{
			//Serial.print("Allocating array mem size ");
			//Serial.println(inp_arraysize);
			
			// ensure array size is at least 1.
			array_size=max(1, inp_arraysize);

			// allocate memory
			the_array=new T[array_size];

			// check there was enough memory: exit otherwise
			if(the_array==NULL)
			{
				array_size=0;
				mem_allocation_failed=true;
				return;
			}

			// ready for use
			curr_index=0;
			mem_allocated=true;
			mem_allocation_failed=false;
		}
	}

	void reset_array()
	{
		// only done if memory has not been allocated yet
		if(!mem_allocated && !mem_allocation_failed)
		{
			//Serial.println("Reset array content");
			
			// reset array to first index
			// and set to 0 the number of filled entries
			
			// ready for use
			curr_index=0;
			tot_curr_elements=0;
		}
	}


    // append an element to the (circular) array 
	bool append_item(T inp_item)
	{
		if(!mem_allocation_failed && !mem_allocated)
		{
			// the array has not been allocated yet;
			//allocate with default size
			allocate_array_mem();
		}

		if( mem_allocated && (getSize()>0) )
		{
			the_array[curr_index]=inp_item;
			updateIndexAndSize();
			return true;
		}
		else
			return false;
	}

	// sum of all elements in the array
	T computeSum()
	{
		
		T sum=0;
		// this does nothing if the size is 0
		for(int ind=0; ind<tot_curr_elements; ind++)
			sum+=the_array[ind];

		return sum;
	}

	T computeAvg()
	{
		double avg;
		T sum=computeSum();
		
		if(tot_curr_elements>0)
			avg = ((double)sum)/tot_curr_elements;
		else
			avg = 0.0;

		return ( (T)avg );
	}

	// these are trivial to understand
	bool isAllocated(){return mem_allocated;}
	int getSize(){return array_size;}
	int getTotElem(){return tot_curr_elements;}


	/*// stuff used for debugging only
	//int getCurrIndex(){return curr_index;}
	void printcontent()
	{
		Serial.print("[");
		for(int ind=0; ind<tot_curr_elements; ind++)
		{
			Serial.print(the_array[ind]);
			Serial.print(" ");
		}
		Serial.print("]");
	}//*/

};



// this object counts interrupts to derive speed (RPM) 
// using a Simple Moving Average (SMA) of fixed size
class InterruptSpeedMeasure_SMA : public InterruptSpeedMeasure{
protected:	
	//intCircularArray RPM_sma_array;
	CircularArray<double> RPM_sma_array;
public:
	// default consructor (implicitely calls the parent constructor)
	InterruptSpeedMeasure_SMA() {;}
	
	// this is to allocate an sma array of chosen size. 
	// If not done, a default size array will be allocated at first use 
	void setupSMAarray(int smasize){RPM_sma_array.allocate_array_mem(smasize);}

	virtual double getRPMandUpdate()
	{
		if(enabled)
		{
			// get the current RPM reading;
			// use the "parent" funciton 
			double current_RPM = InterruptSpeedMeasure::getRPMandUpdate();

			// debug stuff
			//Serial.print("curr_index before append ");
			//Serial.println( RPM_sma_array.getCurrIndex() );
			//Serial.print("tot_curr_elements before append ");
			//Serial.println( RPM_sma_array.getTotElem() );
			
			//Serial.println( "array content before append");
			//RPM_sma_array.printcontent();
			//Serial.println("");
			//

			//append this value to the sma array
			bool success = RPM_sma_array.append_item(current_RPM);

			//// debug stuff
			//Serial.print("curr_index after append ");
			//Serial.println( RPM_sma_array.getCurrIndex() );
			//Serial.print("tot_curr_elements after append ");
			//Serial.println( RPM_sma_array.getTotElem() );
			//Serial.println( "array content after append");
			//RPM_sma_array.printcontent();
			//Serial.println("");
			//



			if(success)
			{
				// debug stuff
				//Serial.println("moving average worked");
				//Serial.print("current_RPM = ");
				//Serial.println(current_RPM);
				

				// compute and return moving average;
				return RPM_sma_array.computeAvg();
				
			}
			else
			{
				
				Serial.println("moving average FAILED");
				
				// something failed. Return single value;
				return current_RPM;
			}

		}
		else
			return -1;
	}

	void reset_sma(){RPM_sma_array.reset_array();}

};


/// ========================================





#endif
