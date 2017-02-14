#define POTPIN1 A0 //Input pin to the potentiometer
#define POTPIN2 A1
#define ANGLE_FACTOR 0.25 //This will change once I get the pots
#define ANGLE_STRING_1 "Angle1: "
#define ANGLE_STRING_2 "Angle2: "

int pot1, pot2;
double angle1, angle2;
String angleString1 = "";
String angleString2 = "";

void setup() {
  // put your setup code here, to run once:
  //pinMode(ledPin,OUTPUT); //declare the ledPin as an Output
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pot1 = analogRead(POTPIN1); //Top potentiometer
  angle1 = ANGLE_FACTOR * (pot1 - 512);
  pot2 = analogRead(POTPIN2); //Top potentiometer
  angle2 = ANGLE_FACTOR * (pot2 - 512);
  angleString1 = "Angle 1: " ;
  angleString1.concat(angle1);
  
  angleString2 = "Angle 2: " ;
  angleString2.concat(angle2);
  
  Serial.println(angleString1);
  //Serial.println(angleString2);

  delay(500);
}
