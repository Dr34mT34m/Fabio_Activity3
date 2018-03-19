#ifndef BEGINNER_LED_h
#define BEGINNER_LED_h

#include <Arduino.h>



// C++ version using Classes:

// digital led (ON/OFF)
class dig_LED{
protected:	
	
	// this is the PIN number
	int out_pin;	
	
	// set to TRUE after the LED has been initialized properly
	boolean LED_enabled;
	
	// set to TRUE when the LED is on. False otherwise
	boolean led_on;
	
	// private functions:
	
	// set the Enabled or On flag
	void setEnabled(boolean enabled){LED_enabled=enabled;}	
	void setOn(boolean isOn){led_on=isOn;}	
	
	// set the pin to the ON vlue
	void set_pin_on()
	{
		// set the pin to the ON vlue
		digitalWrite(out_pin, HIGH);
	}
	//	
	// set the pin to the OFF vlue
	void set_pin_off()
	{
		// set the pin to the OFF vlue
		digitalWrite(out_pin, LOW);
	}
	//
public:
	// constructor with no arguments (set defaults value)
    dig_LED()
	{
		out_pin=-1; 
		setEnabled(false);
	}
	// construct, set pin value and enable
	dig_LED(int out_pin_numb)
	{
		out_pin=-1; 
		setEnabled(false);
		setup_LED(out_pin_numb);
	}
	//
	boolean isEnabled(){return LED_enabled;}
	boolean isOn(){return led_on;}	
	void toggle() {isOn()?switch_off():switch_on();}
	
    // set pin value and enable
	void setup_LED(int out_pin_numb)
	{
		// This function is used only once        
        if( isEnabled() )
        return;
        
        
        // If pin number is out of range 
        // PinMode will fail with no warning.
        // This function does not enable the LED
        // for negative pin numbers.
        if(out_pin_numb>=0)
		{
			out_pin=out_pin_numb;


			// set the pin to be in output mode
			pinMode(out_pin,OUTPUT);


			setEnabled(true);
			//switch off by default
			setOn(true);
			switch_off();
			//
		}
	}
	// switches ON an enabled LED
	void switch_on()
	{
		if( isEnabled() && !isOn() )
		{
			// switch LED ON
			set_pin_on();
			setOn(true);
		}

	}
	// switches OFF an enabled LED
	void switch_off()
	{
		if( isEnabled() && isOn() )
		{
			// switch LED OFF
			set_pin_off();
			setOn(false);
		}	
	}
	//
	void blink(int waiting_ms)
	{
		if( isEnabled() && (waiting_ms>0) )
		{
			switch_on();
			delay(waiting_ms);
			//
			switch_off();
			delay(waiting_ms);
		}	
	}

};





// analog led (PWM)
class ana_LED{
protected:
	// this is the PIN number
	int out_pin;	
	// set to TRUE after the LED has been initialized properly
	boolean LED_enabled;	
	// set to TRUE when the LED is on. False otherwise
	boolean led_on;
	
	// brightness between 0 (off) and 255 (brightest).
	int current_pwm;
	
	// used to restore previous brightness level 
    int prev_pwm;
    
    // the discrete change in brightness
	int pwm_increment;
	
    //
	int pwm_off_value;
	
	//
	//
	void setEnabled(boolean enabled){LED_enabled=enabled;}	
	void setOn(boolean isOn){led_on=isOn;}	
	void set_pin_on() {analogWrite(out_pin,current_pwm);}
	void set_pin_off() {analogWrite(out_pin,pwm_off_value);}
	
	// initialize variables to default values
    void set_default_vals()
	{
		out_pin=-1; 
		setEnabled(false);
		current_pwm=127;
		prev_pwm=127;
		pwm_increment=2;
		pwm_off_value=0;
	}
	
public:
	ana_LED()
	{
		set_default_vals();
	}
	//
	ana_LED(int out_pin_numb)
	{
		set_default_vals();
		setup_LED(out_pin_numb);
	}	
	//
	void switch_on()
	{
		//// temp test [ in setup() add this line: Serial.begin(9600); ]
		//Serial.print("Switching on ");	
		//Serial.println();		
		////

		if( isEnabled() && !isOn() )
		{
			// if PWM is zero, resume previous value
			//if(current_pwm==pwm_off_value)
			current_pwm=prev_pwm;

			// switch LED ON
			set_pin_on();
			setOn(true);
		}

	}
	//
	void switch_off()
	{
		//// temp test [ in setup() add this line: Serial.begin(9600); ]
		//Serial.print("Switching off ");	
		//Serial.println();		
		////

		if( isEnabled() && isOn() )
		{
			// store PWM previous value
			prev_pwm=current_pwm;

			// switch LED OFF
			set_pin_off();
			setOn(false);

			// update PWM value
			current_pwm=pwm_off_value;
		}

	}
	//
	void setup_LED(int out_pin_numb)
	{
		// This function is used only once        
        if( isEnabled() )
        return;
        
        if(out_pin_numb>=0)
		{
			out_pin=out_pin_numb;
			pinMode(out_pin,OUTPUT);
			setEnabled(true);
			//switch off by default
			setOn(true);
			switch_off();
			//
		}
	}
	//
	boolean isEnabled(){return LED_enabled;}
	boolean isOn(){return led_on;}
	
    // makes LED brighter (chenge only visible when LED is on)
	void brighter()
	{
		int incremented_pwm;
		
		if( isEnabled() )
		{	
			if( isOn() )
			{// led is on: make brighter
				if(current_pwm<255)
				{
					// store PWM previous value
					prev_pwm=current_pwm;		

					// set a higher (brighter) value
					incremented_pwm=current_pwm + pwm_increment;
					//incremented_pwm=current_pwm + 4;

					current_pwm=min( incremented_pwm , 255 );
					set_pin_on();
				}
			}
			else
			{	// led is off: ajust it so that it will be brighter next time it is on
				if(prev_pwm<255)
				{
					// set a higher (brighter) value
					incremented_pwm=prev_pwm + pwm_increment;
					prev_pwm=min( incremented_pwm , 255 );
				}
			}
		}
	}
	// makes it dimmer (chenge only visible when LED is on)
	void dimmer()
	{
		int decremented_pwm;

		if( isEnabled() )
		{	
			if( isOn() )
			{// led is on: make dimmer
				if(current_pwm>0)
				{
					// store PWM previous value
					prev_pwm=current_pwm;		

					// set a lower (dimmer) value
					decremented_pwm=current_pwm - pwm_increment; 
					current_pwm=max(decremented_pwm , 0 );
					set_pin_on();
				}
			}
			else
			{	// led is off: ajust it so that it will be dimmer next time it is on
				if(prev_pwm>0)
				{
					// set a lower (dimmer) value
					decremented_pwm=prev_pwm - pwm_increment;
					prev_pwm=min( decremented_pwm , 0 );
				}
			}
		}
	}
	
    // set the level of brigthness
	void set_brightness(int in_pwm)
	{
		if( isEnabled() )
		{
			boolean switch_back_off=false;
			in_pwm=abs(in_pwm);

			// switch on if needed
			if(!isOn())
			{
				current_pwm=pwm_off_value;
				prev_pwm=pwm_off_value;
				switch_on();
				switch_back_off=true;
			}

			// store PWM previous value
			prev_pwm=current_pwm;		

			// set a  value
			current_pwm=min( in_pwm , 255 );
			set_pin_on();


			// switch off if needed
			if(switch_back_off)
				switch_off();
		}
	}
	
    // set to the max and min brigthness 
	void set_max_bright() {set_brightness(255);}
	void set_min_bright() {set_brightness(0);}
	
    // returns the brightness of the LED
    int get_brightness() {return current_pwm;}
};





#endif
