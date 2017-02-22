#define MOTOR_PIN_1 5
#define MOTOR_PIN_2 6
#define POT_1 A0
#define POT_2 A1
#define POT_3 A2
#define POT_4 A3
#define ANGLE_FACTOR 0.26
#define ANGLE_STRING_1 "Angle 1: "
#define ANGLE_STRING_2 "Angle 2: "

int pot1, pot2, pot3, pot4;
//Motor 1
int in1 = 8;
int in2 = 7;
//Motor 2
int in3 = 10;
int in4 = 9;


double angle1, angle2;
double motorADirection;
double motorBDirection;
String angleString1 = "";
String angleString2 = "";

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(MOTOR_PIN_2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
  //Motor Direction determined by potentiometer
  pot1 = analogRead(POT_1);
  pot2 = analogRead(POT_2);
  pot3 = analogRead(POT_3);
  pot4 = analogRead(POT_4);
  
  motorADirection = ( pot1 - 512) / 2;
  motorBDirection = ( pot2 - 512) / 2;
  angle1 = ANGLE_FACTOR * (pot1 - 512);
  angle2 = ANGLE_FACTOR * (pot2 - 512);
  angleString1 = "Angle 1: ";
  angleString1.concat(angle1);

  angleString2 = "Angle 2: ";
  angleString2.concat(angle2);
  if(motorADirection > 0)
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
  
  if(motorBDirection > 0)
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
  analogWrite(MOTOR_PIN_1, abs(motorADirection));
  analogWrite(MOTOR_PIN_2, abs(motorBDirection));

  Serial.println(angleString1 + ' ' + angleString2);
  
  
  //Serial.println(motorADirection + ' ' + motorBDirection);
  delay(10);

}
