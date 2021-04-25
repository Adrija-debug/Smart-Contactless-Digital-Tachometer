int in1 = 8;        //Declaring where our module is wired  
int in2 = 9;
int ConA = 10;     // This is a PWM DI/DO   
int speed1;
int led=13;
#include <SoftwareSerial.h>
SoftwareSerial s(5,6);
void setup() 
{
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ConA, OUTPUT);
  pinMode(led,OUTPUT);  
  Serial.begin(9600);
  s.begin(9600);
  delay(1000);
}

void loop() 
{
  int flag=0;
  
  digitalWrite(in1, LOW);            //This HIGH and LOW can be switched to change direction
  digitalWrite(in2, HIGH);

  if(s.available()>0)
  {
    flag=s.read();
  }
  if(flag==1)
  {
    delay(5000);
    analogWrite(ConA,0);
    digitalWrite(led,HIGH);
    delay(100);
    flag=0;
    
  }
  else
  {
    speed1 = analogRead(A0);
    speed1 = speed1 * 0.2492668622;      //We read the analog value from the potentiometer to calibrate it
    analogWrite(ConA, speed1);
    digitalWrite(led,LOW);

  }
 
  delay(100);   
}
 
