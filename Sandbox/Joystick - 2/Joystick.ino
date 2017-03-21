//This is a test program for learning protothreading
#include <pt.h>   // include protothread library

#define BAUD_RATE 250000 //Faster than 9600

#define POT_1 A0
#define POT_2 A1
#define POT_3 A2
#define POT_4 A3

#define YELLOW_LED 13
#define GREEN_LED 12
#define RED_LED 11

#define MOTOR_PIN_1 5 //Pin supports PWM
#define MOTOR_PIN_2 6 //Pin that supports PWM
#define ANGLE_FACTOR 0.26 // Factor to calculate angle from pot signal


int pot1 = 0;
int pot2 = 0;
double theta1 = 0;
double theta2 = 0;
double newtheta1 = 0;
double newtheta2 = 0;
int newTorque1 = 0;
int newTorque2 = 0;
int motorTorque1 = 0;
int motorTorque2 = 0;

String thetaString1 = "", thetaString2 = "";

int pot3, pot4;
//Motor 1
int in1 = 8;
int in2 = 7;
//Motor 2
int in3 = 10;
int in4 = 9;

char str[255];

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
  
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  inputString.reserve(200);

  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  PT_INIT(&pt1);  // initialise the two
  PT_INIT(&pt2);  // protothread variables
  PT_INIT(&pt3);  // initialise the two
  PT_INIT(&pt4);  // protothread variables
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
    
    newtheta1 = analogRead(POT_1); //Top potentiometer
    //newtheta1 = ANGLE_FACTOR * (pot1 - 512);
    newtheta2 = analogRead(POT_2); //Top potentiometer
    //newtheta2 = ANGLE_FACTOR * (pot2 - 512);
  }
  PT_END(pt);
}
/* Generate the motor Torque*/
static int protothreadMotor (struct pt *pt) {
  //static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    //Wait Until condition can also be changed to "when motorTorque value changed"
    PT_WAIT_UNTIL(pt, newTorque1 != motorTorque1 || newTorque2 != motorTorque2); 
    //Toggle the GREEN LED
    //digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
    digitalWrite(GREEN_LED, HIGH);
    //timestamp = millis(); // Set new timestamp
    
    motorTorque1 = newTorque1;
    motorTorque2 = newTorque2;
    
    if(motorTorque1 > 0)
    {
      //Motor A Clockwise
      digitalWrite(in2, LOW);
      digitalWrite(in1, HIGH);
    }
    else {
      //Motor A Counter-clockwise
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    
    if(motorTorque2 > 0)
    {
      //Motor B Clockwise
      digitalWrite(in4, LOW);
      digitalWrite(in3, HIGH);
    }
    else {
      //Motor B Counter-clockwise
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
    
    analogWrite(MOTOR_PIN_1, abs(motorTorque1));
    analogWrite(MOTOR_PIN_2, abs(motorTorque2));
    digitalWrite(GREEN_LED, LOW);
  }
  PT_END(pt);
}

/* Receive the String from the driver for motor*/
static int protothreadInput(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, (Serial.available() > 0) & (millis() - timestamp > interval) ); // What should the condition be?
    //Toggle RED_LED
    timestamp = millis();
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    //digitalWrite(RED_LED, HIGH);
    //Read from the serial input buffer
    inputString = Serial.readStringUntil('\n');
    inputString.toCharArray(str,255);
    //Process Strings to get each motorTorquePercent
    sscanf(str,"%d,%d", &newTorque1, &newTorque2);
    //digitalWrite(RED_LED, LOW);
  }
  PT_END(pt);
}

/* Send the Angle position over serial*/
static int protothreadOutput(struct pt *pt, int interval) {
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) {
    PT_WAIT_UNTIL(pt, newtheta1 != theta1 || newtheta2 != theta2 ); // What should the condition be?
    //Send the angle positions
    //Toggle YELLOW_LED
    //digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    digitalWrite(YELLOW_LED, HIGH);
    theta1 = newtheta1;
    theta2 = newtheta2;
    thetaString1.concat(theta1);
    thetaString1.concat(',');
    thetaString2.concat(theta2);
    //thetaString2.concat(',');
    Serial.println(thetaString1 + thetaString2);
    thetaString1 = "";
    thetaString2 = "";
    digitalWrite(YELLOW_LED, LOW);
  }
  PT_END(pt);
}

void loop() {
  //Starting protothreads, and setting time interval (subject to change)
  protothreadAngle(&pt1, 1);     //Angle Proto-Thread
  protothreadInput(&pt3, 1);     //Serial Input Proto-Thread
  protothreadMotor(&pt2);         //Motor Proto-Thread
  protothreadOutput(&pt4, 1);  //Serial Output Proto-Thread
}
