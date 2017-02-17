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
int theta1 = 0;
int theta2 = 0;
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


/* This function measures the potentiometers and calculates the angle*/
static int protothreadAngle(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) { // never stop 
    /* each time the function is called the second boolean
    *  argument "millis() - timestamp > interval" is re-evaluated
    *  and if false the function exits after that. */
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis(); // take a new timestamp
    
    pot1 = analogRead(POT1PIN); //Top potentiometer
    theta1 = ANGLE_FACTOR_1 * (pot1 - 512);
    pot2 = analogRead(POT2PIN); //Top potentiometer
    theta2 = ANGLE_FACTOR_2 * (pot2 - 512);
    
  }
  PT_END(pt);
}
/* Generate the motor Torque*/
static int protothreadMotor (struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    //Wait Until condition can also be changed to "when motorTorque value changed"
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval  ); 
    timestamp = millis(); // Set new timestamp
    /* The following can be used, but CPU will always be busy
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
static int protothreadInput(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, Serial.available() > 0 ); // What should the condition be?
    //Read from the serial input buffer
    inputString = Serial.readStringUntil('\n');
    //Process Strings to get each motorTorquePercent
    scanf("%4[^,]d,%4[^,]d", motorTorque1, motorTorque2);
    
  }
  PT_END(pt);
}

/* Send the Angle position over serial*/
static int protothreadOutput(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, Serial.available() > 0 ); // What should the condition be?
    //Read from the serial input buffer
    Serial.println(theta1 + ',' + theta2);
  }
  PT_END(pt);
}

void loop() {
  //Starting protothreads, and setting time interval (subject to change)
  protothreadAngle(&pt1, 10);     //Angle Proto-Thread
  protothreadMotor(&pt2, 1000);   //Motor Proto-Thread
  protothreadInput(&pt3, 10);     //Serial Input Proto-Thread
  protothreadOutput(&pt4, 1000);  //Serial Output Proto-Thread
}
