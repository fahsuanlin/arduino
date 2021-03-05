const int R_led_pin = 9;
const int G_led_pin = 10;
const int B_led_pin = 11;
int mapped_led_power = 0;
char led_color;

int color_pin = 0;
int mode = 0;

const int lightsensor_pin = A6;
int lightsensor_value = 0;

const int button_pin = 2;
int button_value = 0;
int count = 0;
void setup() {
  Serial.begin(9600);
  // Setting the pin mode for modules
  pinMode(R_led_pin,OUTPUT);
  pinMode(G_led_pin,OUTPUT);
  pinMode(B_led_pin,OUTPUT);
  //
  pinMode(lightsensor_pin,INPUT);
  pinMode(button_pin,INPUT);
}

void loop() {
  
  lightsensor_value = analogRead(lightsensor_pin);
  button_value = digitalRead(button_pin);
  Serial.print("raw value = ");
  Serial.println(lightsensor_value);
  //mapped_led_power = map(lightsensor_value, 100, 600, 255, 0);
  
  if(lightsensor_value >=600)
    mapped_led_power = 20;
  else if(lightsensor_value >=400 && lightsensor_value<600)
    mapped_led_power = 20;
  else if(lightsensor_value >=200 && lightsensor_value<400)
    mapped_led_power = 40;
  else if(lightsensor_value >=0 && lightsensor_value<200)
    mapped_led_power = 240;
  
  //Serial.print("mapped value = ");
  //Serial.println(mapped_led_power);
  
  if(button_value==1 && mode==0){
    mode++;
    color_pin = R_led_pin;
  }else if(button_value==1 && mode==1){
    mode++;
    color_pin = G_led_pin;
  }else if(button_value ==1 && mode==2){
    mode=0;
    color_pin = B_led_pin;
  }
  Serial.println(mode);
  Serial.print("color value = ");
  Serial.println(color_pin);
  
  digitalWrite(R_led_pin,LOW);
  digitalWrite(G_led_pin,LOW);
  digitalWrite(B_led_pin,LOW);
  analogWrite(color_pin, mapped_led_power);
  delay(100);

}
