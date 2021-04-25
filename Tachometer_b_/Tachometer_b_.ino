#define BLYNK_PRINT Serial    
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SoftwareSerial.h>
SoftwareSerial s(D6,D5);
char auth[] = "xxxxxxxxx"; //Blynk authentication token
char ssid[] = "xxxxx";  //WIFI Name
char pass[] = "xxxxxxx";  //WIFI Password

#define DHTPIN 2          // Digital pin 4 to read temperature
#define hallSensorPin D7 //To read output of hall-effect sensor

unsigned long sampleTime = 1000;
long rpm;   float t,h;  

#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;

// This function sends ESP8266's up time every second to Virtual Pin (5) and (6).
//  This means that we define how often to send data to Blynk App.

void setup()
{
  Serial.begin(9600);
  s.begin(9600);
  pinMode(hallSensorPin,INPUT);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  delay(1000);
}
void loop()
{
    int flag=0;
    Blynk.run(); // Initiates Blynk
    t = dht.readTemperature();
    h=dht.readHumidity();
    rpm = getRPM();

    Serial.print("Temperature=");
    Serial.print(t);
    Serial.print("Humidity=");
    Serial.print(h);
    Serial.print("RPM = ");
    Serial.println(rpm);
 
    Blynk.virtualWrite(V5, rpm);  //V5 is for rpm of motor
    Blynk.virtualWrite(V6, t);  //V6 is for Temperature 
    Blynk.virtualWrite(V7,h); //V7 is for Humidity 
    if (rpm>1200)
    {
      Blynk.notify("Warning!Speed limit exceeded, motor will be stopped");
      flag=1;
    }
    if (rpm<50)
    {
      Blynk.notify("Warning!Speed too low.Motor about to stop working");
     
    }
    if (t>50)
    {
      Blynk.notify("Warning!Motor too much heated, will be stopped");
      flag=1;
    }
    if (h>90)
    {
       Blynk.notify("Warning!Motor too much heated, will be stopped");
      flag=1;
    }
    if (flag==1)
    {
      s.write(1); //Sending 1 to Arduino to stop motor
      flag=0;
    }
    delay(100);
}

long getRPM()
{
  long count = 0;
  boolean countFlag = LOW;
  unsigned long currentTime = 0;
  unsigned long startTime = millis();
  while (currentTime <= sampleTime)
  
  {
    if (digitalRead(hallSensorPin) == HIGH)
    {
      countFlag = HIGH;
    }
    if (digitalRead(hallSensorPin) == LOW && countFlag == HIGH)
    {
      count++;
      countFlag=LOW;
      
    }
    currentTime = millis() - startTime;
  }
  long countRpm = long(60000/sampleTime)*count;

  count=0;
  return countRpm;
}
    
