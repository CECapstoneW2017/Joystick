
int motorPin = 3;
int delayTime = 50;
int delayHigh = 1;
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
}
