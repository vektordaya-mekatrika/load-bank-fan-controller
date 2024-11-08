#include "loadbankfancontroller.h"

void setup() {
  // declare pins that will be used
  gpioInit();
  //activate serial communication
  Serial.begin(115200);
  //Set initial values
  initialValue();
}

void loop() {
  //get current reading
  int current=getCurrent();
  //calculate pwm and decide when to hold
  pwmCalculation(current);
  //reset rpm to zero if no interrupt event
  resetRPM();
  // relay operation
  relayOperation(current);
  //print value to serial
  printValue(current, commandRpm, rpm);
}


