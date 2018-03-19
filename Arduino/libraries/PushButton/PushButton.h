#ifndef PushButton_h
#define PushButton_h


#ifdef COMPILE_CPP_NOT_ARDUINO
#include "..\All_Arduino_or_Cpp_symboldefines\All_Arduino_or_Cpp_symboldefines.h"
#include "..\IntervalCheckTimer\IntervalCheckTimer.h"
#else
#include <Arduino.h>
#include <IntervalCheckTimer.h>
#endif


//\\\\\\\\\\\\\\ IO \\\\\\\\\\\\\\\\\\\\\\\\\\\\\

enum command_list_enum
{
	// simple commands:
	simple_none=47,	
	
	//'0' '1'  '2'	'3'		'4'	 '5'   '6'   '7'    '8'    '9'			
	zero, one, two, three, four, five, six, seven, eight, nine,

	//	':'		  ';'		'<'		'='	    '>'		'?'		'@'		'A'  'B'  'C'
	switch_on, switch_off, dimmer, blink, brighter,  change_spin_dir,
	
	// these may e useful for the temperature
	up_fix_amount, down_fix_amount,

    // added for fan control
    toggle,

	//
	simple_last

};

class simple_command{
private:
	command_list_enum command;
public:
	simple_command(){set_simple_command(simple_none);}
	inline void set_simple_command(int in_command){command =  ( (isValidCommand(in_command)) ?  ((command_list_enum)in_command)  : simple_none) ;}
	inline command_list_enum get_simple_command(){return command;}
	inline boolean check_n_set_command(int inCmd){set_simple_command(inCmd); return isValidCommand(inCmd);}
	inline boolean isValidCommand(int inCmd) {return ( ( (inCmd>=simple_none)&&(inCmd<=simple_last) )   ? true : false );}	
};



class in_push_button{
private:
	simple_command simp_cmd;
	boolean new_command;
	int buttonPin;
	boolean initialized;
	int buttonState; 
	int lastButtonState, reset_buttonState; // LOW or HIGH;  current and previous reading from the input pin
	int buttonState_for_new_command;
	long lastDebounceTime;  // the last time the output pin was toggled
	long debounceDelay; //50 ms;
	

	// used to avoid reading a "long push" as multiple individual pushes
	bool avoid_longpush;
	IntervalCheckTimer longpush_check;


	//
	void setup_in_push_button()
	{
		new_command=false;		
		//buttonState_for_new_command=HIGH;
		buttonState_for_new_command=LOW;

		lastDebounceTime=0;  // the last time the output pin was toggled
		debounceDelay=25; //ms;
		initialized=false;
		//
		reset_buttonState=LOW;
		buttonState=reset_buttonState;
		lastButtonState=reset_buttonState; 

		//
		avoid_longpush=false;
		//
	}

	void setup_in_push_button(int in_buttonPin, command_list_enum in_simple_command)
	{
		setup_in_push_button();	
		buttonPin=in_buttonPin;		
		//pinMode(buttonPin,INPUT); 
		pinMode(buttonPin,INPUT_PULLUP); 

		initialized=true;
		// set the associated command
		simp_cmd.set_simple_command(in_simple_command);
	}

	void setup_in_push_button(int in_buttonPin, command_list_enum in_simple_command, unsigned long int longpush_ms)
	{
		setup_in_push_button(in_buttonPin, in_simple_command);
		//
		avoid_longpush=true;
		longpush_check.setInterCheck(longpush_ms);
		//
	}

public:
	in_push_button() {setup_in_push_button();}
	in_push_button(int in_buttonPin, command_list_enum in_simple_command) {setup_in_push_button(in_buttonPin, in_simple_command);}
	in_push_button(int in_buttonPin, command_list_enum in_simple_command, unsigned long int longpush_ms) {setup_in_push_button(in_buttonPin, in_simple_command, longpush_ms);}

	void assign_pin_command(int in_buttonPin, command_list_enum in_simple_command){setup_in_push_button(in_buttonPin, in_simple_command);}
	
	
	//
	boolean check_new_input(){
		boolean new_input_found=false;
		long curr_Time;
		int in_buttonState;
		new_command=false;

		if( initialized==true)
		{	
			in_buttonState=digitalRead(buttonPin);
			//test:
			buttonState=in_buttonState;
			//

			if(in_buttonState!=lastButtonState)
				lastDebounceTime=millis();

			curr_Time=millis();
			if( (curr_Time - lastDebounceTime) > debounceDelay )
			{
				//buttonState=in_buttonState;
				if(in_buttonState==buttonState_for_new_command)
				{
					new_input_found=true;
					new_command=true;
				}
			}
			lastButtonState=in_buttonState;
			buttonState=reset_buttonState;

			
			// fpr a if avoid_longpush is true:
			// ignore a positive detection if the last succeful reading was recent
			if(avoid_longpush && new_input_found)
				if( !longpush_check.isMinChekTimeElapsedAndUpdate() )
					new_input_found=false;
			

		}

		return new_input_found;
	}

	command_list_enum get_new_input()
	{
		command_list_enum current_command;

		if(new_command==true)
			current_command=simp_cmd.get_simple_command();
		else
			current_command=simple_none;

		return current_command;
	}

	command_list_enum check_n_get_new_input()
	{
		command_list_enum current_command=simple_none;

		if(check_new_input())
			current_command=get_new_input();
		
		return current_command;
	}

	boolean check_enabled() {return initialized;}
};




//*

//serial read_command OR read_value

class inputs{
private:
	static const int max_in_push_buttons=10;
	int tot_in_push_buttons;
	int curr_pbindex;
	in_push_button in_pb_array[max_in_push_buttons];
	//
	static const int max_out_serial=1;
	static const int default_baud_rate=9600;
	int tot_out_serial;
	int baud_rate;
	boolean serailenabled;
	//
	boolean inputs_enabled;
	//
	boolean check_n_get_command_push_buttons(command_list_enum& in_smpl_cmd)
	{
		int pbindex;

		if(tot_in_push_buttons==0) return false;

		// scan the push buttons
		for(pbindex=curr_pbindex+1;pbindex<tot_in_push_buttons; pbindex++)
		{
			if(in_pb_array[pbindex].check_new_input())
			{
				curr_pbindex=pbindex;
				in_smpl_cmd=in_pb_array[pbindex].get_new_input();
				return true;
			}
		}
		for(pbindex=0;pbindex<curr_pbindex+1; pbindex++)
		{
			if(in_pb_array[pbindex].check_new_input())
			{
				curr_pbindex=pbindex;
				in_smpl_cmd=in_pb_array[pbindex].get_new_input();
				return true;
			}
		}

		// nothing found
		return false;
	}

	boolean check_n_get_command_serial(command_list_enum& in_smpl_cmd)
	{
		int serialindex;
		int incomingCommandByte = 0;
		int incomingVal = 0; 
		simple_command simp_cmd;
		boolean got_command=false;

		if(tot_out_serial==0 || !serailenabled) return false;

		for(serialindex=0;serialindex<tot_out_serial; serialindex++)
		{
			if(Serial.available()>0)
			{
				incomingCommandByte = Serial.read();
				//test
				//Serial.println(incomingCommandByte, DEC);
				//Serial.println();
				//
				if(incomingCommandByte>0)
				{
					got_command=simp_cmd.check_n_set_command(incomingCommandByte);
					//if(in_cmplx_cmd.is_complex())
					//{
					//	incomingVal = Serial.read();
					//	got_command=in_cmplx_cmd.check_n_set_val(incomingVal);					
					//}
					return got_command;
				}
			}
		}

		// nothing found
		return got_command;
	}


	boolean check_enabled_push_buttons()
	{
		int pbindex;

		if(tot_in_push_buttons==0) return false;

		// scan the push buttons
		for(pbindex=0;pbindex<tot_in_push_buttons; pbindex++)
		{
			if(in_pb_array[pbindex].check_enabled())
				return true;
			
		}

		// none neabled 
		return false;
	}
	boolean check_enabled_serial(){return serailenabled;}


public:
	inputs()
	{
		tot_in_push_buttons=0;
		curr_pbindex=-1;
		tot_out_serial=0;
		baud_rate=default_baud_rate;
		serailenabled=false;
		inputs_enabled=false;
	}

	
	void add_in_push_button(in_push_button in_pb)
	{
		if(tot_in_push_buttons<max_in_push_buttons)
		{
			in_pb_array[tot_in_push_buttons]=in_pb;
			tot_in_push_buttons++;
		}
		inputs_enabled |= check_enabled_push_buttons();
	}

	void enable_setup_serial()
	{
		if(tot_out_serial<max_out_serial)
		{
			Serial.begin(baud_rate);
			tot_out_serial++;
			serailenabled=true;
			inputs_enabled = true;
		}
	}
	
	boolean check_n_get_command(command_list_enum& in_smpl_cmd)
	{
		boolean command_found=false;
		// scan the push buttons
		command_found=check_n_get_command_push_buttons(in_smpl_cmd);

		// then, if nothing is found, scan he serial 
		if(!command_found)
			command_found=check_n_get_command_serial(in_smpl_cmd);	

		return command_found;
	}
	
	boolean check_enabled(){return inputs_enabled;}

};

//*/

#endif
