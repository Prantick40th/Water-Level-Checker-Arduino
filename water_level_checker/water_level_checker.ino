#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
 
#define trigger 10
#define echo 11
#define motor 8
#define buzzer 12
#define MIN_DISTANCE 30
#define AVG_DISTANCE 200
#define MIN_DISTANCE_TO_SEND_MSG 80

bool isSent = false;
char userCommandToSystem = 'y'; // that is no 

SoftwareSerial gsm(9, 13); 
LiquidCrystal lcd(2,3,4,5,6,7);
 
float time=0,distance=0;
int temp=0; 
void setup()
{
  gsm.begin(9600);
 lcd.begin(16,2);
 lcd.print("Welcome");
 delay(1000);
 lcd.clear();
 pinMode(trigger,OUTPUT);
 pinMode(echo,INPUT);
 pinMode(motor, OUTPUT);
 pinMode(buzzer, OUTPUT);
 lcd.print("  Water Level ");
 lcd.setCursor(0,1);
 lcd.print("   Indicator  ");
 delay(2000);
}
 
void loop()
{
 lcd.clear();
 digitalWrite(trigger,LOW);
 delayMicroseconds(2);
 digitalWrite(trigger,HIGH);
 delayMicroseconds(10);
 digitalWrite(trigger,LOW);
 delayMicroseconds(2);
 time=pulseIn(echo,HIGH);
 distance=time*340/20000;
 lcd.clear();
 lcd.print("Water Space In  ");
 lcd.setCursor(0,1);
 lcd.print("Tank is: ");
 lcd.print(distance);
 lcd.print("Cm");
 delay(2000);

 // ======================== checking =================
 if(distance <= MIN_DISTANCE )
 {
     digitalWrite(motor, LOW);
     digitalWrite(buzzer, HIGH);
     lcd.clear();
     lcd.print("Water Tank Full ");
     lcd.setCursor(0,1);
     lcd.print("Motor Turned OFF");
     delay(2000);
     digitalWrite(buzzer, LOW);
     delay(3000);
//     temp=1;
 }
// 
//  else if(distance<12 && temp==1)
// {
//     digitalWrite(motor, LOW);
//     lcd.clear();
//     lcd.print("Water Tank Full ");
//     lcd.setCursor(0,1);
//     lcd.print("Motor Turned OFF");
//     delay(5000);
// }

 else if(distance > MIN_DISTANCE && distance <= MIN_DISTANCE_TO_SEND_MSG)
 {
  if(!isSent){
    SendMessage("Tank is getting filled. Do something if you want");
    isSent = true;
    lcd.clear();
    lcd.print("Message sent");
    lcd.setCursor(0,1);
    lcd.print("to user");
    delay(2000);
  }
 }
 
 else if(distance > MIN_DISTANCE && distance <= AVG_DISTANCE)
 {
   digitalWrite(motor, HIGH);
   lcd.clear();
   lcd.print("LOW Water Level");
   lcd.setCursor(0,1);
   lcd.print("Motor Turned ON");
   delay(3000);
   isSent = false;
//   temp=0;
 }

 else if(distance > AVG_DISTANCE)
 {
   digitalWrite(motor, HIGH);
   lcd.clear();
   lcd.print("Tank is filling");
   lcd.setCursor(0,1);
   lcd.print("Motor Turned ON");
   delay(3000);
//   temp=0;
 }
 
}

void SendMessage(String msg){
  gsm.println("AT+CMGF=1"); // set the gsm module in Text mode that is for sending message
  delay(500);
  gsm.println("AT+CMGS=\"+8801521429588\"\r"); // set the mobile number
  delay(500);
  gsm.println(msg);
  delay(100);
  gsm.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
