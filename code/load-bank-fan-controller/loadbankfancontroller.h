#define FAN1_PWM 11
#define FAN1_FB 2
#define CT A2
#define HOLD_TIME 5000 
#define ADC_MID 503
#define RELAY 12

int rpm;
unsigned long lastEvent;
int commandRpm;
int lastCurrent;
bool rpmHoldStatus;
int rpmHoldValue;
unsigned long startHold;
int currentHoldValue;

void readRPM()
{
  unsigned long interruptEvent=millis();
  rpm=60000/(interruptEvent-lastEvent);
  lastEvent=interruptEvent;
}

void gpioInit()
{
  pinMode(FAN1_PWM,OUTPUT);
  pinMode(FAN1_FB,INPUT_PULLUP);
  pinMode(CT, INPUT);
  pinMode(RELAY, OUTPUT);
  //activate interrupt
  attachInterrupt(digitalPinToInterrupt(FAN1_FB),readRPM, FALLING);
}

void initialValue()
{
  lastCurrent=0;
  lastEvent=millis();
  startHold=millis();
  rpm=0;
  commandRpm=100;
  rpmHoldStatus = false;
  analogWrite(FAN1_PWM,commandRpm);
}

void pwmCalculation(int current)
{
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
        commandRpm=20;
        break;
      case 400 ... 512:
        commandRpm=255;
        break;
      default:
        commandRpm=map(current,50,400,20,255);
        break;
    }
  }
  analogWrite(FAN1_PWM,commandRpm);
  //save last current value
  lastCurrent=current;
}

void resetRPM()
{
  if((millis()-lastEvent)>3000)
  {
    rpm=0;
  }
}

void relayOperation()
{
  if(commandRpm!=0 && rpm==0)
  {
    digitalWrite(RELAY,HIGH);
    Serial.println("RELAY OPEN");
  }
  else
  {
    digitalWrite(RELAY,LOW);
    Serial.println("RELAY CLOSE");
  }
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