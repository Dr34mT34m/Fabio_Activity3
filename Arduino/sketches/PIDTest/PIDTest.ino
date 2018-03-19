#include <PID_v1.h>


#define AVERAGE_WINDOW 6    //size of averaging window

volatile unsigned long microcounter = 0;    //used in interrupt for microseconds-between-interrupts computation
volatile float TimerArray[AVERAGE_WINDOW];  //averaging array for microsecond measurements
volatile unsigned char nAverageIndex = 0;  //index into averaging array (for the ISR to keep track)
volatile char InterruptTrigFlag = 0;      //flag used to timeout if interrupt doesn't happen for a period of time

char nInterruptTimeout = 0;        //flag used in main program to indicate the interrupt timed out (so 0 RPM)


#define MAPPING (float)(5000.0/255.0)  //approx scale difference between RPM range and PWM range

double fRPM = 0;                    //computed RPM value in here (also serves as 'input' to PID)
double fPIDSetpoint, fPIDOutput;    //PID vars
PID myPID(&fRPM, &fPIDOutput, &fPIDSetpoint, 0.7/MAPPING, 1.0/MAPPING, 0.0/MAPPING, DIRECT);  //and instance of PID (taken from basic example)


//ISR for the motor tick - read time from last tick (microseconds), and put in array
void SpeedISR()
{
  sei();  //enable interrupts
  long nMicros = micros() - microcounter;
  microcounter = micros();  //nMicros is time from last interrupt
  
  InterruptTrigFlag = 1;  //signal an interrupt has occurred (this is periodically checked by main program then set back to 0 to test if interrupts are live, or if a timeout has occurred)
  
  TimerArray[nAverageIndex] = nMicros;  //add reading to average array
  nAverageIndex++;                      //forward array pointer
  if(nAverageIndex >= AVERAGE_WINDOW)
    nAverageIndex = 0;                  //but keep pointer within array
}


void setup()
{
   pinMode(2, INPUT_PULLUP);   //Interrupt to input pullup for A3144 hall sensor
   
   pinMode(12, OUTPUT);
   digitalWrite(12, LOW);      //PWM output
   pinMode(11, OUTPUT);
   digitalWrite(11, LOW);      //Direction control (not actually used in this code yet)
   
   attachInterrupt(0, SpeedISR, RISING);    //attach the ISR for speed computation

   Serial.begin(9600);
   Serial.setTimeout(10);      //start serial port, and set timeout really small
   
   fPIDSetpoint = 0;          //initial target RPM

   myPID.SetSampleTime(5);          //set update to 50 ms
   myPID.SetMode(AUTOMATIC);         //enable PID
}

unsigned long nResetRPMCounterTimeout = 0;  //this global is used to timeout the RPM computation if no interrupt seen for a while
#define RPM_TIMEOUT 100                    //RPM counter timeout. If no pulse seen in 100 ms, assume not spinning


//Serial port control related variables - not important for PID
#define SERIAL_OUTPUT_REPEAT 1000          //repetition of serial output
#define LOGGERSAMPLES 1024
unsigned long nRepeatMillis = 0;            //this global is used in loop() to dump data to the serial port every second
char countdowntoreadout = 0;  //when set to 1, this will trigger data log session in main loop
float values[LOGGERSAMPLES];           //1024 data samples to record
int nValuePointer = 0;        //pointer into above array
long dataloggermillis = 0;
int nLogSampleTime = 100;

void loop()
{
  
  //This bit of code timeouts when there's not been an interrupt for a while, and sets or resets the
  //nInterruptTimeout flag accordingly. IMPORTANT TO GET 0 RPM detection!
  
  if(millis() - nResetRPMCounterTimeout > RPM_TIMEOUT)  //every RPM_TIMEOUT ms, check that an interrupt has occurred for the motor sensor
  {
    nResetRPMCounterTimeout = millis();
    if(InterruptTrigFlag == 0)  //trigger flag still 0 since last 100 ms interval...
      nInterruptTimeout = 1;  //indicate interrupt timeout
    else
      nInterruptTimeout = 0;  //indicate no timeout
      
    InterruptTrigFlag = 0;    //this will be set back to 1 by next interrupt
  }



  //RPM Computation below: IMPORTANT
  
  
  //Only compute RPM if no interrupt timeout, otherwise set to 0
  if(nInterruptTimeout == 0)
  {
    float total = 0;                          //set up a variable to add up the entries in the averaging window
    for(char i = 0; i < AVERAGE_WINDOW; i++)  //loop through each entry
      total += TimerArray[i];                 //and add to total
 
    total /= AVERAGE_WINDOW;                  //then compute mean
         
    //total is time for 1/6th spin...
    float RotationTime = total*6;        //time for 1 rotation (micros/rotation)
    fRPM = (1000000.0/RotationTime)*60.0;    //RPM update
  }
  else          //if interrupt did timeout
    fRPM = 0;  //clamp RPM to 0



  //PID processing: IMPORTANT


  if(myPID.Compute())  //run PID computation. This will be true if PID computes a new value for output
  {
    unsigned char nOutput = (unsigned char)fPIDOutput;  //if it does, put value into a char
    analogWrite(12, nOutput);  //and write to pin
  }

  
  //everything below is not important to get PID working

  //Everything below here is concerned with implementing serial port I/O. it recognises integers. A single integer sent via serial
  //will change the RPM target value. 2 integers separated by a space will set the target RPM, then enter logging mode at an interval 
  //defined by the second integer.
  //so sending:
  //
  // 2000
  //
  //will set the RPM to 2000
  //sending:
  //
  // 2000 10
  //
  // will set the RPM to 2000, AND switch to data logging mode. Once the log is complete, the data will be dumped to the serial port
  


  //Serial port stuff (Not important for PID, but useful!)


  if(countdowntoreadout)  //this is the code that runs if we're logging response and dumping to serial afterwards
  {
    
    
    if(countdowntoreadout == 3)  //third mode of data logging: dump data to serial, and switch off motor
    {
      Serial.println("");
      Serial.println("");
      Serial.println("");
      Serial.println("");
      Serial.println("");
      Serial.println("");
      Serial.println("");  //empty space
      for(int i = 0; i < LOGGERSAMPLES; i++)
        Serial.println(values[i]);
      fPIDSetpoint = 0;

      countdowntoreadout = 4;  //next mode will be endless loop doing nothing
    }
    
    
    
    if(countdowntoreadout == 2)  //second mode of data logging: actual logging
    {
      if(millis()-dataloggermillis > nLogSampleTime)
      {
        dataloggermillis = millis();
        values[nValuePointer] = fRPM;
        nValuePointer++;
      }
      if(nValuePointer >= LOGGERSAMPLES)      //if we've fillerd our sample array
        countdowntoreadout = 3;  //next mode  
    }
    
    
    
    if(countdowntoreadout == 1)  //first mode of data logging: initialise
    {
       dataloggermillis = millis();  //time started (so next sample occurs at correct time)
       values[0] = fRPM;  //first value
       nValuePointer = 1;  //point to next value
       
       countdowntoreadout = 2; //next mode
    }
  }
  
  
  
  
  //If not doing a data logging session, execute following code for serial port I/O
  
  else  //this code runs for normal operation - monitors serial input, and outputs setpoint / current point to console periodically
  {
    //This bit of code monitors the serial interface for numbers.
    if(Serial.available() > 0)
    { //serial data available...
      int nVal = Serial.parseInt();          //get an int - this will be a target RPM
      int nVal2 = Serial.parseInt();          //get a second int. If present, then trigger countdown to readout...
    
      if(nVal2) 
      {
        countdowntoreadout = 1;    //if nonzero second value trigger readout
        nLogSampleTime = nVal2;    //and set sample time to second value (millis)
      }
   
      fPIDSetpoint = (float) nVal;
      while(Serial.read() != -1);            //and flush input buffer by reading until empty (gets rid of pesky CR or LF)
    }  
  
    //this bit of code deals with computing RPM and dumping it to serial monitor
    if(millis() - nRepeatMillis > SERIAL_OUTPUT_REPEAT)  //fire off this bit of code every SERIAL_OUTPUT_REPEAT milliseconds
    {
      nRepeatMillis = millis();          //this resets the trigger for the next 1000 milliseconds
        
      Serial.print("Current speed:");
      Serial.print(fRPM);                    //dump reading to console
      Serial.print(" Target speed:");
      Serial.println(fPIDSetpoint);          //dump reading to console
    }
  }
}




