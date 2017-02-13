
int motorPin = 3;
int delayTime = 200;
int delayHigh = 10;
void setup() {
  // put your setup code here, to run once:
  pinMode(motorPin, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  digitalWrite(motorPin, HIGH);
  delayMicroseconds(delayHigh);
  digitalWrite(motorPin, LOW);
  delayMicroseconds(delayTime);
  
  //analogWrite(motorPin, 30);
}
