int RelayPin = 8;//The signal of the relay is pinned to the digital output slot 8
int LightSensorPin=A0;//The light sensor is pinned to the analogic input A0
int SensorValue=0;//initial value of the amount of light sent by the sensor


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RelayPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
 SensorValue=analogRead(LightSensorPin);
 Serial.println(SensorValue);
 if (SensorValue < 100){
  Serial.println("Low light, NIGHT TIME");
  digitalWrite(RelayPin, LOW);//Turning on the blue LED when it is night
  }
 else {
  Serial.println("More light, DAY TIME");
  digitalWrite(RelayPin, HIGH);//Turning on the red LED when it is day
  }
 delay(1000);
}
