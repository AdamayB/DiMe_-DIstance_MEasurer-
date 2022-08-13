#include<LiquidCrystal.h>
#define LED_E_PIN 11
#define LED_RS_PIN 12
#define LED_DR4_PIN 5
#define LED_DR5_PIN 4
#define LED_DR6_PIN 3
#define LED_DR7_PIN 10
#define TRIG_PIN 8
#define ECHO_PIN 2
unsigned long lastTimeUltrasonicTrigger = millis();
unsigned long ultrasonicTriggerDelay = 60;
LiquidCrystal lcd(LED_RS_PIN,LED_E_PIN,LED_DR4_PIN,LED_DR5_PIN,LED_DR6_PIN,LED_DR7_PIN);
volatile unsigned long timeBegin;
volatile unsigned long timeEnd;
double maxLim = 400.0;
bool newDistAvailable = false;
void triggerUS(){
  digitalWrite(TRIG_PIN,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN,LOW);
}
double getDistance(){
  double duration = timeEnd-timeBegin;
  double distance = duration/58.0;
  if (distance>400.0){
    return maxLim;
  }
  return distance;
}
void echoPinTrigger(){
  if(digitalRead(ECHO_PIN) == HIGH){
    timeBegin=micros();
  }
  else{
    timeEnd = micros();
    newDistAvailable = true;
  }
}
void setup(){
  lcd.begin(16,2);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),
                  echoPinTrigger,
                  CHANGE);
}

void loop(){
  unsigned long timeNow = millis();
  
  if (timeNow - lastTimeUltrasonicTrigger > ultrasonicTriggerDelay) {
    lastTimeUltrasonicTrigger += ultrasonicTriggerDelay;
    triggerUS();
  }
  if (newDistAvailable){
    newDistAvailable = false;
    double l=getDistance();
    lcd.setCursor(0,0);
    lcd.print("Distance ");
    lcd.print(l);
    lcd.print('cm');
  }

  
}
