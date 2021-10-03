
int Relay = 3; 
int Pin1 = A0;
float value1 = 0;

void setup(){
  Serial.begin(9600);
  pinMode(Relay, OUTPUT);
  
  pinMode(Pin1, INPUT);

}
 void loop(){

  Serial.print("MOISTURE LEVEL:");
  value1 = analogRead(Pin1);
  Serial.println(value1);
  delay(500);
  
  if(value1<450)
  {
    Serial.println("LOW");
    digitalWrite(Relay, LOW);
  }
  else
  {
    Serial.println("HIGH");
    digitalWrite(Relay, HIGH);
  }
 
 }
