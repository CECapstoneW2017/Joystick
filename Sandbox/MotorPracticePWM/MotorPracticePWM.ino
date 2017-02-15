#define MOTOR_PIN_1 5
#define MOTOR_PIN_2 6
#define POT_1 A0

//Motor 1
int enA = 10;
int in1 = 9;
int in2 = 8;
//Motor 2

double motorADirection1;


void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_PIN_1, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
  //Motor Direction determined by potentiometer
  motorADirection1 = ((analogRead(POT_1) - 512) / 2);

  if(motorADirection1 > 0)
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
  analogWrite(MOTOR_PIN_1, abs(motorADirection1));
 
  Serial.println(motorADirection1);
  delay(10);

}
