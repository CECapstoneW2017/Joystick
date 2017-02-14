//Motor 1
int enA = 10;
int in1 = 9;
int in2 = 8;
//Motor 2

int motorPin = 3;
int delayTime = 200;
int delayHigh = 10;
void setup() {
  // put your setup code here, to run once:
  pinMode(motorPin, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  digitalWrite(motorPin, HIGH);
  delayMicroseconds(delayHigh);
  digitalWrite(motorPin, LOW);
  delayMicroseconds(delayTime);
  */
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(motorPin, 200);

}
