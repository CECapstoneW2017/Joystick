int potPin = 2; //Input pin to the potentiometer
int ledPin = 13;
int val = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT); //declare the ledPin as an Output
}

void loop() {
  // put your main code here, to run repeatedly:
  val = analogRead(potPin);
  Serial.print(val);
  digitalWrite(ledPin, HIGH); //turn LED on
  delay(val);
  digitalWrite(ledPin, LOW);
  delay(val);
}
