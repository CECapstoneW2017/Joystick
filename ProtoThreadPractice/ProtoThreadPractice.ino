//This is a test program for learning protothreading
#include <pt.h>   // include protothread library

#define LEDPIN 13  // LEDPIN is a constant 
#define POT1PIN 11
#define POT2PIN 12
#define ANGLE_FACTOR_1 1 // Factor to calculate angle from pot signal
#define ANGLE_FACTOR_2 1 // Factor to calculate angle from pot signal

int pot1 = 0;
int pot2 = 0;
int angle1 = 0;
int angle2 = 0;

static struct pt pt1, pt2; // each protothread needs one of these
/*
 * Priorities of tasks:
 * First task: Read the serial input from CHAI3D
 * Second task: Generate Motor Torque
 * Third task: Read the angle 
 * Fourth task: Send serial output of angle. 
 */


void setup() {
  pinMode(LEDPIN, OUTPUT); // LED init
  PT_INIT(&pt1);  // initialise the two
  PT_INIT(&pt2);  // protothread variables
}

void toggleLED() {
  boolean ledstate = digitalRead(LEDPIN); // get LED state
  ledstate ^= 1;   // toggle LED state using xor
  digitalWrite(LEDPIN, ledstate); // write inversed state back
}

/* This function measures the potentiometers and calculates the angle*/
static int protothread1(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) { // never stop 
    /* each time the function is called the second boolean
    *  argument "millis() - timestamp > interval" is re-evaluated
    *  and if false the function exits after that. */
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    pot1 = analogRead(POT1PIN); //Top potentiometer
    angle1 = ANGLE_FACTOR_1 * (pot1 - 512);
    pot2 = analogRead(POT2PIN); //Top potentiometer
    angle2 = ANGLE_FACTOR_2 * (pot2 - 512);
    Serial.println("Angle 1: " + angle1);
    Serial.println("Angle 2: " + angle2);
  }
  PT_END(pt);
}
/* exactly the same as the protothread1 function */
static int protothread2(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis();
    toggleLED();
  }
  PT_END(pt);
}

void loop() {
  protothread1(&pt1, 900); // schedule the two protothreads
  protothread2(&pt2, 1000); // by calling them infinitely
}
