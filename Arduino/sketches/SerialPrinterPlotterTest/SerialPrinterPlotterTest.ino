#include "SerialPrinterPlotter.h"

int target, perctg;

SerialPrinterPlotter testprint;
void setup() {
  // put your setup code here, to run once:
  //*
  // bounds for the first variable to print
  testprint.set_bounds(0, 0, 50);
  // bounds for the second variable to print
  testprint.set_bounds(1, 0, 50);
  
  // bounds for the third variable to print
  testprint.set_bounds(2, 0, 50);

  // target: used to plot a line corresponding to the target
  target=40;
  
  
  // pctg: used print a band around the target value 
  // that represtns the required % error
  // upper band: target + approx_pctg/2;
  // lower band: target - approx_pctg/2;  
  perctg=10;
  //Serial.begin(9600);
  //*/

}

void loop() {
  // put your main code here, to run repeatedly:
  //*
  for(int i=0; i<50; i++)
  {
    // prepare to print the first variable
    testprint.appendval(0, i, target, perctg);   
    
    // prepare to print the second  variable
    testprint.appendval(1, i/2, target/2, perctg);    
    
    // prepare to print the third variable
    testprint.appendval(2, i/4, target/4, perctg);    
    
    //  print all variables
    testprint.print_the_string();
    delay(100);
  }
  for(int i=50; i>0; i--)
  {
    // prepare to print the first variable
    testprint.appendval(0, i, target, perctg);
    
    // prepare to print the second  variable
    testprint.appendval(1, i/2, target/2, perctg);
    
    // prepare to print the third variable
    testprint.appendval(2, i/4, target/4, perctg);
    
    //  print all variables
    testprint.print_the_string();
    delay(100);
  }
  //*/
}
