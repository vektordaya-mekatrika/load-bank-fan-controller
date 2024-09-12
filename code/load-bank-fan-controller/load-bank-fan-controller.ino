#include "loadbankfancontroller.h"

void setup() {
  // declare pins that will be used
  pinMode(FAN1_PWM,OUTPUT);
  pinMode(FAN1_FB,INPUT_PULLUP);
  pinMode(CT, INPUT);
  //activate interrupt
  attachInterrupt(digitalPinToInterrupt(FAN1_FB),readRPM, FALLING);
  //activate serial communication
  Serial.begin(115200);
  //Set initial values
  lastCurrent=0;
  lastEvent=millis();
  startHold=millis();
  rpm=0;
  commandRpm=100;
  rpmHoldStatus = false;
  analogWrite(FAN1_PWM,commandRpm);
}

void loop() {
  //get current reading
  int current=getCurrent();

  //hold the rpm
  if(rpmHoldStatus)
  {
    if((millis()-startHold)>HOLD_TIME)
    {
      rpmHoldStatus=false;
    }
  }
  else
  {
    if(current<(lastCurrent-12))
    {
      rpmHoldStatus=true;
      startHold=millis();
      rpmHoldValue=commandRpm;
      currentHoldValue=current;
    }
  }

  //set rpm value
  if(rpmHoldStatus && current<=currentHoldValue)
  {
    commandRpm=rpmHoldValue;
  }
  else
  {
    switch(current)
    {
      case 0 ... 50:
        commandRpm=15;
        break;
      case 400 ... 512:
        commandRpm=255;
        break;
      default:
        commandRpm=map(current,50,400,15,255);
        break;
    }
    // if(current>400)
    // {
    //   commandRpm=255;
    // }
    // else
    // {
    //   commandRpm=map(current,0,400,0,255);
    // }
  }
  analogWrite(FAN1_PWM,commandRpm);
  lastCurrent=current;
  
  //print value to serial
  printValue(current, commandRpm, rpm);
  delay(1);
}

void readRPM()
{
  int interruptEvent=millis();
  rpm=60000/(interruptEvent-lastEvent);
  lastEvent=interruptEvent;
}

int getCurrent()
{
  int current=0;
  int currentRaw=0;
  int i,j;
  
  for(j=0;j<60;j++)
  {
    for(i=0;i<200;i++)
    {
      int adc=abs(analogRead(CT)-ADC_MID);
      if(adc>currentRaw)
      {
        currentRaw=adc;
      }
    }
    current+=currentRaw;
  }
  current=current/j;
  return current;
}

void printValue(int current, int commandRpm, int rpm)
{
  Serial.print(current);
  Serial.print("\t");
  Serial.print(commandRpm);
  Serial.print("\t");
  Serial.print(rpm);
  if(rpmHoldStatus)
  {
    Serial.print("\t");
    Serial.print("RPM ON HOLD");
  }
  Serial.print("\n");
}
