#define FAN1_PWM 11
#define FAN1_FB 2
#define CT A2
#define HOLD_TIME 5000 
#define ADC_MID 503

int rpm;
unsigned long lastEvent;
int commandRpm;
int lastCurrent;
bool rpmHoldStatus;
int rpmHoldValue;
unsigned long startHold;
int currentHoldValue;