#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);
int led = 5; //PWD
int state;
int writeflag = 0;
void setup(){
    BTSerial.begin(9600);
    pinMode(led,OUTPUT);
    analogWrite(led,LOW);
}
void loop(){
    if (BTSerial.available()>0){
       state = BTSerial.read();
       writeflag=0;
    }
    if (writeflag == 0){
      analogWrite(led , state);
      writeflag=1;
    }
}
