#define BUTTON_PIN 4 //arduino nano D4
#define PHOTO_PIN 5  //arduino nano D5
#define PHOTO_LED_PIN 15 //arduino nano A1
#define BUTTON_LED_PIN 16 //arduino nano A2

unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;

struct touch { 
   byte wasPressed = LOW; 
   byte isPressed = LOW; 
}; 

touch touch; 

int photo_val = 0;

void setup() 
{
   pinMode(PHOTO_LED_PIN, OUTPUT);
   pinMode(BUTTON_LED_PIN, OUTPUT);
   pinMode(PHOTO_PIN, INPUT); 
   pinMode(BUTTON_PIN, INPUT); 
   Serial.begin(9600); 
   startMillis = millis();  //initial start time
  
} 
void loop() 
{ 

   currentMillis = millis(); //get the current "time" 
   
   touch.isPressed = isTouchPressed(BUTTON_PIN); 
   if (touch.wasPressed != touch.isPressed) { 
    if(touch.isPressed){
     Serial.println("Touch pressed"); 
     analogWrite(BUTTON_LED_PIN,255);
     //delay(1);
    }
   } 
   else {
     analogWrite(BUTTON_LED_PIN,0);   
   }
   
   touch.wasPressed = touch.isPressed; 

   photo_val=digitalRead(PHOTO_PIN);
   if(photo_val == HIGH) {
       Serial.println(photo_val); 
       analogWrite(PHOTO_LED_PIN,0);
    }
    else{
      Serial.println(photo_val); 
      analogWrite(PHOTO_LED_PIN,255);
    }
}
 
bool isTouchPressed(int pin) 
{ 
   return digitalRead(pin) == HIGH; 
} 
