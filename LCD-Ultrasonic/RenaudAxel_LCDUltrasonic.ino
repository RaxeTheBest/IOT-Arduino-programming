#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
int trig = 2;
int echo = 3;
int Buzzer = 4;
int redLED = 7;
int blueLED = 12;
int count=0;
LiquidCrystal_I2C lcd(0x27,16,2);// set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

}

void loop()
{
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  lcd.clear();
  digitalWrite(trig, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trig, LOW);

  int distance = pulseIn(echo, HIGH) * 17 / 1000;
  
  if (distance <= 30) {
        count=count+1;   
  }
  else if (distance > 31 and distance <= 60){
        if (count<0){
            count=0;
          }
         else {        
            count=count-1;
         }
  }
  if (count<=10){
        digitalWrite(blueLED, HIGH);
  }
  else {
        digitalWrite(blueLED, LOW);
        digitalWrite(redLED, HIGH);
        digitalWrite(Buzzer,HIGH );
        delayMicroseconds(1000);
        digitalWrite(Buzzer,LOW );
        
  }
  lcd.setCursor (0,0);
  lcd.print("Counted people");
  lcd.setCursor (1,1);
  Serial.print(distance);
  Serial.println("cm");
  Serial.print("There are ");
  if (count<=0){
    lcd.print("0");
    Serial.print("0");
    }
  else{
    lcd.print(count);
    Serial.print(count);
  } 
  Serial.println(" people in the room.");
  delay(1000);

}
