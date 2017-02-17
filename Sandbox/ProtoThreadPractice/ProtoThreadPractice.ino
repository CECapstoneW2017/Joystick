//This is a test program for learning protothreading
#include <pt.h>   // include protothread library

#define BAUD_RATE 115200 //Faster than 9600
#define LEDPIN 13  // LEDPIN is a constant 
#define POT1PIN 11
#define POT2PIN 12
#define MOTOR_PIN_1 5 //Pin supports PWM
#define MOTOR_PIN_2 6 //Pin that supports PWM
#define ANGLE_FACTOR_1 0.25 // Factor to calculate angle from pot signal
#define ANGLE_FACTOR_2 0.25 // Factor to calculate angle from pot signal

int pot1 = 0;
int pot2 = 0;
int angle1 = 0;
int angle2 = 0;
int motorTorque1 = 0;
int motorTorque2 = 0;

String inputString = "";
boolean stringComplete = false;


static struct pt pt1, pt2, pt3, pt4; // each protothread needs one of these
/*
 * Priorities of tasks:
 * First task: Read the serial input from CHAI3D
 * Second task: Generate Motor Torque
 * Third task: Read the angle 
 * Fourth task: Send serial output of angle. 
 */


void setup() {
  pinMode(LEDPIN, OUTPUT); // LED init
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  inputString.reserve(200);
  
  PT_INIT(&pt1);  // initialise the two
  PT_INIT(&pt2);  // protothread variables
  Serial.begin(BAUD_RATE);
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
    timestamp = millis(); // take a new timestamp
    
    pot1 = analogRead(POT1PIN); //Top potentiometer
    angle1 = ANGLE_FACTOR_1 * (pot1 - 512);
    pot2 = analogRead(POT2PIN); //Top potentiometer
    angle2 = ANGLE_FACTOR_2 * (pot2 - 512);
    
  }
  PT_END(pt);
}



/* Generate the motor Torque*/
static int protothread2(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval  );
    timestamp = millis(); // Set new timestamp
    /*
    digitalWrite(motorPin, HIGH);
    delayMicroseconds(delayHigh);
    digitalWrite(motorPin, LOW);
    delayMicroseconds(delayTime);
    */
    analogWrite(MOTOR_PIN_1, motorTorque1);
    analogWrite(MOTOR_PIN_2, motorTorque2);
  }
  PT_END(pt);
}

/* Receive the String from the driver for motor*/
static int protothread3(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, Serial.available() ); // What should the condition be?
    //Read from the serial input buffer
    //Process Strings to get each motorTorquePercent
  }
  PT_END(pt);
}

/* Send the Angle position over serial*/
static int protothread4(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, Serial.available() ); // What should the condition be?
    //Read from the serial input buffer
    Serial.println(angle1 + ' ' + angle2);
  }
  PT_END(pt);
}

void loop() {
  //Left these two threads like this for now.
  protothread1(&pt1, 10);     //Angle Thread
  protothread2(&pt2, 1000);   //Motor Thread
  protothread3(&pt3, 10);     //Serial Input Thread
  protothread4(&pt4, 1000);
}
