#define MIN_RPM 0
#define MAX_RPM 100
#define FAN1_PWM 2
#define FAN1_FB 3
#define CT A1
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