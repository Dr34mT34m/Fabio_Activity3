#include <Task3Sense1.h>
#include <Task3Control1.h>

sense speedometer;
control timer;

void setup() {
  ArduinoInterruptNames interruptpinRight = 0;
  int magnetsRight = 49;

  speedometer.setup_rotation_counter(interruptpinRight, magnetsRight);
  timer.setup_controller(300, 1, 0.5, 1, 300);

  Serial.begin(9600);
}

void loop() {
  if (timer.isTimeToTakeMeasurementAndUpdate())
  speedometer.return_current_speed();
  
}
