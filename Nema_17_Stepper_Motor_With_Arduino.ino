#include <Bounce2.h>;

// Создаем объект 
Bounce debouncer = Bounce();

#define EN 8

//Direction pin
#define X_DIR 5

//Step pin
#define X_STP 2
#define x_endstop 3

unsigned long timing_steps;
bool stepper_state = 0;

//A498
int stps=6400;
int i =0;

unsigned long curMillis;
unsigned long prevStepMillis = 0;
unsigned long millisBetweenSteps = 1; // milliseconds
unsigned long home_timing;
unsigned long direction_vibration_timing;
bool X_state;
bool homing_state = false;
bool direction = 0;

void singleStep() {
  if (curMillis - prevStepMillis >= millisBetweenSteps) {
    digitalWrite(X_DIR, direction);
    prevStepMillis += millisBetweenSteps;
  }
}



void homingStep(int x_step_delay, bool home_direction) {
  digitalWrite(X_DIR, home_direction);
  if (curMillis - prevStepMillis >= x_step_delay) {
    i++;
    prevStepMillis += millisBetweenSteps;
    digitalWrite(X_STP, HIGH);
    digitalWrite(X_STP, LOW);
  }
  curMillis = millis();
}


void Read_X_Endstop(){
  if(X_state == 1){
    homing_state = true;
  }
}


void setup(){
  pinMode(x_endstop, INPUT);
  //pinMode(x_endstop, INPUT);
  pinMode(X_DIR, OUTPUT); pinMode(X_STP,OUTPUT);
  pinMode(EN, OUTPUT);
  digitalWrite(EN,LOW);
  Serial.begin(9600);
  
   // Даем бибилотеке знать, к какому пину мы подключили кнопку
  debouncer.attach(x_endstop);
  debouncer.interval(5); // Интервал, в течение которого мы не буем получать значения с пина
  while(true){
    debouncer.update();
    X_state = debouncer.read();
    Read_X_Endstop();
    if(!homing_state){
      Serial.println("HOMING");
      homingStep(8, true);
      home_timing = millis();
    }else{
      Serial.println("HOMING DONE");
      homingStep(5, false);
      if(millis() - home_timing > 2000){
        i = 0;
        return;
      } 
    }
  }
}


void loop(){
  debouncer.update();
  X_state = debouncer.read();
  if(!X_state){
    direction = !direction;
  }
  Read_X_Endstop();
  curMillis = millis();
  if(X_state && i < stps){
    homingStep(2, direction);
    Serial.println(i);
  }else{
    homingStep(2, direction);
    i = 0;
    Serial.println(i);
  }
 
}

