#ifndef SYSTEMCONTROLUNIT_H
#define SYSTEMCONTROLUNIT_H

#include <..\DCmotor\DCmotor.h>



//this is a simple proportional closed loop controller;
// it will be a compoment of the control unit
class SpeedControlSimpleProportional{
protected:
	// these are used to determine the PWM for a given target speed (RPM) 
	int max_RPM_abs_error;
	int min_RPM_abs_error;
public:
	// constructor; set the default paramters
	SpeedControlSimpleProportional() {max_RPM_abs_error=10000; min_RPM_abs_error=500;}
	
	// this function computes the RPM 
	int ComputeTargetPWM(int target_RPM, int measured_RPM, int curr_PWM)
	{
		
		int RPM_error, abs_RPM_error, error_sign, PWM_range, PWM_variation; 
		
		RPM_error=target_RPM - measured_RPM;
		abs_RPM_error=abs(RPM_error);

		// if the error is small, nothing is done
		if(abs_RPM_error<=min_RPM_abs_error)
			return curr_PWM;
		
		
		// otherwise take action 
		if(RPM_error>=0)
		{
			// not fast enough
			PWM_range=255-curr_PWM;
			error_sign=1;
		}
		else
		{
			// too fast
			PWM_range=curr_PWM;
			error_sign=-1;
		}

		// if the RPM difference is big, "give it all we have"
		if(abs_RPM_error>=max_RPM_abs_error)
			PWM_variation=PWM_range;
		else
		{
			// otehrwise we use a fractin of the range
			double range_fraction=(abs_RPM_error-min_RPM_abs_error) / ( (double) (max_RPM_abs_error-min_RPM_abs_error)) ;
			PWM_variation = ( ( (double)PWM_range )*range_fraction );

		}

		return (curr_PWM + error_sign*PWM_variation);

	}
};

class SystemControlSimpleProportional{
protected:
	// there are three timers, used to check all input valiables and actions to perform.
	// I will bunch them all in an array. 
	// The indexes of the array can be easily identified by the following "labels" (this is similar to using an "enum")
	static const int adjust_speed=0;
	static const int target_speed_check=1;
	static const int buttons_check=2;
	static const int TOT_TIMERS=3;

	IntervalCheckTimer timer_array[TOT_TIMERS];

	// this object determines the PWM for a given target RPM and current RPM 
	SpeedControlSimpleProportional closedLoopControl;
	
	// this checks a given timer (and restarts it when true)
	bool isTimeToCheckTimer(int timer_index)
	{
		if(timer_index>=0 && timer_index<TOT_TIMERS)
		{
			bool result = timer_array[timer_index].isMinChekTimeElapsed();
			// reset timer when true
			if(result)
				timer_array[timer_index].updateCheckTime();
			return result;
		}
		else
			return false;
	}

public:
	// constructor; set the default paramters
	SystemControlSimpleProportional()
	{
		// these are the default values for intervals between: 
		//two speed-adjustments; two taget-speed checks; two button checks; 
		int adjust_speed_interval_ms=1000;
		int target_speed_check_interval_ms=2000;
		int buttons_check_interval_ms=200;
		
		set_interval_vals(adjust_speed_interval_ms, target_speed_check_interval_ms, buttons_check_interval_ms);
	}
	// these allow setting values other than the default for each timer
	void set_interval_vals(int adjust_speed_interval_ms, int target_speed_check_interval_ms, int buttons_check_interval_ms)
	{
		timer_array[adjust_speed].setInterCheck(adjust_speed_interval_ms);
		timer_array[target_speed_check].setInterCheck(target_speed_check_interval_ms);
		timer_array[buttons_check].setInterCheck(buttons_check_interval_ms);
	}

	// these finctions singal that it's time perform an aciton 
	// when "true" is returned, the timer is restarted automatically 
	bool isTimeToCheckInputButtons() { return isTimeToCheckTimer(buttons_check);}
	bool isTimeToCheckTargetSpeedChange() { return isTimeToCheckTimer(target_speed_check);}
	bool isTimeToAdjustSpeed() { return isTimeToCheckTimer(adjust_speed);}

	int computePWMtoAchieveTarget(int target_RPM, int measured_RPM, int curr_PWM)
	{
		int PWM = closedLoopControl.ComputeTargetPWM(target_RPM, measured_RPM, curr_PWM);

		// clip it within the range (no smaller than zero, no bigger than 255)
		PWM=max(PWM, 0);
		PWM=min(PWM, 255);

		return PWM;
	}

};





#endif