#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include "Servo.h"

#define srvo 3
#define locker 8 
#define RST_PIN 9       
#define SS_PIN 10

Servo myservo;
LiquidCrystal_I2C lcd(0x27,16,2);  
MFRC522 mfrc522(SS_PIN, RST_PIN);
RTC_DS3231 rtc;

int pos = 0;

byte fathy[4] = {0x8A, 0x51, 0xDB, 0x80};
byte azmy[4] = {0x6B, 0x7A, 0xEF, 0x12}; 
byte omar[4] = {0x7B, 0xF7, 0x5C, 0x13};
byte mansour[4] = {0x7A, 0x25, 0x05, 0x80};        
 
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//                                     Math3   Digital Logic Design   Electric Circuits   Law and human rights   Technical Reports   Introduction to AI
int AI_courses_lectures_Mansour[6] = {0,      0,                     0,                  0,                     0,                  0};
int AI_courses_sections_Mansour[6] = {0,      0,                     0,                  0,                     0,                  0};
//                                          Math3   Digital Logic Design   Electric Circuits   Law and human rights   Technical Reports   Introduction to AI
int AI_courses_lectures_check_Mansour[6] = {0,      0,                     0,                  0,                     0,                  0};
int AI_courses_sections_check_Mansour[6] = {0,      0,                     0,                  0,                     0,                  0};
//                                 Math3   Digital Logic Design          Electric Circuits             Statistics             english 2           Material properties
int MT_courses_lectures_Azmy[6] = {0,      0,                            0,                            0,                     0,                  0};
int MT_courses_sections_Azmy[6] = {0,      0,                            0,                            0,                     0,                  0};
//                                       Math3       Digital Logic Design     Electric Circuits        Statistics             english 2           Material properties                                         
int MT_courses_lectures_check_Azmy[6] = {0,          0,                       0,                       0,                     0,                  0};
int MT_courses_sections_check_Azmy[6] = {0,          0,                       0,                       0,                     0,                  0};


void setup() {
  lcd.init();              
	Serial.begin(9600);
  myservo.attach(srvo);
  pinMode(locker, OUTPUT);
	while (!Serial);	
	SPI.begin();			
	mfrc522.PCD_Init();		
	delay(4);				
	mfrc522.PCD_DumpVersionToSerial();
  
  if (! rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  } 

    if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    //If you want to change the start time  
    //If the power of the battery has gone out when the power comes back the time will start from the times stored down here
    rtc.adjust(DateTime(2023, 12, 18, 12, 0, 0));
  }	
}


void loop() {
DateTime now = rtc.now();

if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

if(mfrc522.uid.uidByte[0] == fathy[0] && mfrc522.uid.uidByte[1] == fathy[1] && mfrc522.uid.uidByte[2] == fathy[2] && mfrc522.uid.uidByte[3] == fathy[3])
  {
    lcd_print("Fathy");
  }

else if(mfrc522.uid.uidByte[0] == azmy[0] && mfrc522.uid.uidByte[1] == azmy[1] && mfrc522.uid.uidByte[2] == azmy[2] && mfrc522.uid.uidByte[3] == azmy[3])
  {
    attendance(now.dayOfTheWeek(), now.hour(), now.minute(), "Azmy");
  }

else if(mfrc522.uid.uidByte[0] == omar[0] && mfrc522.uid.uidByte[1] == omar[1] && mfrc522.uid.uidByte[2] == omar[2] && mfrc522.uid.uidByte[3] == omar[3])
  {
    lcd_print("Omar");
  }

else if(mfrc522.uid.uidByte[0] == mansour[0] && mfrc522.uid.uidByte[1] == mansour[1] && mfrc522.uid.uidByte[2] == mansour[2] && mfrc522.uid.uidByte[3] == mansour[3])
  {
   attendance(now.dayOfTheWeek(), now.hour(), now.minute(), "Mansour");
  }

else
  {
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Access Denied");
    digitalWrite(locker, LOW);
    delay(5000);
    lcd.noBacklight();
    lcd.clear();

  }


mfrc522.PICC_HaltA();

}


void locker_open()
{
  digitalWrite(locker, HIGH);
  delay(500);
  for(pos = 90;pos <= 30;pos -= 1){
    myservo.write(pos);
    delay(10);
  }
  delay(6000);
    for(pos = 30;pos >= 90;pos++){
    myservo.write(pos);
    delay(10);
  }
  delay(500);
  digitalWrite(locker, LOW);
}


void lcd_print(char name[])
{
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Access Granted");
    lcd.setCursor(0,1);
    lcd.print("Welcome ");
    lcd.setCursor(8,1);
    lcd.print(name);
    locker_open();
    lcd.noBacklight();
    lcd.clear();
}


void attendance(int day, int hour, int minute, char name[])
{
  //Sunday
  if(daysOfTheWeek[day] == daysOfTheWeek[0] ){sunday(hour, minute, name);}
  //Monday
  else if(daysOfTheWeek[day] == daysOfTheWeek[1]){monday(hour, minute, name);}
  //Tuesday
  else if(daysOfTheWeek[day] == daysOfTheWeek[2]){Tuesday(hour, minute, name);}
  //Wednesday
  else if(daysOfTheWeek[day] == daysOfTheWeek[3]){lcd_free_days(name);}
  //Thursday
  else if(daysOfTheWeek[day] == daysOfTheWeek[4]){lcd_free_days(name);}
  //Friday
  else if(daysOfTheWeek[day] == daysOfTheWeek[5]){lcd_reset_day();lcd_free_days(name);}
  //Saturday
  else if(daysOfTheWeek[day] == daysOfTheWeek[6]){saturday(hour, minute, name);}   
}


void saturday(int hour, int minute, char name[])
{
if(hour < 9 && hour > 7){
before_time(name);
}
else if(hour == 9 && minute <= 30)
{
  //AI math3 Lecture
  if(name == "Mansour"){
    if(AI_courses_lectures_check_Mansour[0] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_lectures_Mansour[0]++;AI_courses_lectures_check_Mansour[0]++;lcd_sign_in(name, 0, 0);}}
}
else if(hour == 10 && minute >= 40 || hour == 11 && minute <= 10) 
{
 //AI math 3 section [0] 
 if(name == "Mansour"){
    if(AI_courses_sections_check_Mansour[0] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_sections_Mansour[0]++;AI_courses_sections_check_Mansour[0]++;lcd_sign_in(name, 1, 0);}
    }
  //MT math3 lecture [0]  
  else if(name == "Azmy"){
    if(MT_courses_lectures_check_Azmy[0] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_lectures_Azmy[0]++;MT_courses_lectures_check_Azmy[0]++;lcd_sign_in(name, 0, 0);}
  }
}
else if(hour == 12 && minute >= 40 || hour == 13 && minute <= 10)
{
 //AI elictrical curcuits section [2] 
 if(name == "Mansour"){
    if(AI_courses_sections_check_Mansour[2] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_sections_Mansour[2]++;AI_courses_sections_check_Mansour[2]++;lcd_sign_in(name, 1, 2);}}
  //MT statistics lecture [3]
  else if(name == "Azmy"){
    if(MT_courses_lectures_check_Azmy[3] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_lectures_Azmy[3]++;MT_courses_lectures_check_Azmy[3]++;lcd_sign_in(name, 0, 3);}
  }
}
else if(hour == 14 && minute >= 20 && minute <=50)
{
  //MT elictric section [2]
  if(name == "Azmy"){
    if(MT_courses_sections_check_Azmy[2] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_sections_Azmy[2]++;MT_courses_sections_check_Azmy[2]++;lcd_sign_in(name, 1, 2);}
  }
}
else if(hour == 12 && minute>=10 && minute <=40){
  lcd_break_time(name);
}
else{lcd_acsses_denied(name);}
}


void sunday(int hour, int minute, char name[])
{
if(hour < 9 && hour > 7){
before_time(name);
}
else if(hour == 9 && minute <= 30)
{
 //AI elictrical curcuits lecture [2] 
 if(name == "Mansour"){
    if(AI_courses_lectures_check_Mansour[2] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_lectures_Mansour[2]++;AI_courses_lectures_check_Mansour[2]++;lcd_sign_in(name, 0, 2);}
    }
  //MT english 2 lecture[4]  
  else if(name == "Azmy"){
    if(MT_courses_lectures_check_Azmy[4] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_lectures_Azmy[4]++;MT_courses_lectures_check_Azmy[4]+
    +;lcd_sign_in(name, 0, 4);}
  }
}
else if(hour == 10 && minute >= 40 || hour == 11 && minute <= 10)
{
 //technical reportes lecture [4]
 if(name == "Mansour"){
    if(AI_courses_lectures_check_Mansour[4] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_lectures_Mansour[4]++;AI_courses_lectures_check_Mansour[4]++;lcd_sign_in(name, 0, 4);}}
}
else if(hour == 12 && minute >= 40 || hour == 13 && minute <= 10)
{
 //AIE AI lecture [5]
 if(name == "Mansour"){
    if(AI_courses_lectures_check_Mansour[5] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_lectures_Mansour[5]++;AI_courses_lectures_check_Mansour[5]++;lcd_sign_in(name, 0, 5);}
    }
  //MT electric lecture[2]
  else if(name == "Azmy"){
    if(MT_courses_lectures_check_Azmy[2] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_lectures_Azmy[2]++;MT_courses_lectures_check_Azmy[2]++;lcd_sign_in(name, 0, 2);}
  }
}
else if(hour == 12 && minute>=10 && minute <=40){
  lcd_break_time(name);
}
else{lcd_acsses_denied(name);}
}


void monday(int hour, int minute, char name[])
{
if(hour < 9 && hour > 7){
before_time(name);
}
else if(hour == 9 && minute <= 30)
{
   //MT Material properties lecture [5] 
   if(name == "Azmy"){
    if(MT_courses_lectures_check_Azmy[5] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_lectures_Azmy[5]++;MT_courses_lectures_check_Azmy[5]++;lcd_sign_in(name, 0, 5);}}
}
else if(hour == 10 && minute >= 40 || hour == 11 && minute <= 10)
{
 //MT Statistics section [3] 
 if(name == "Azmy"){
    if(MT_courses_sections_check_Azmy[3] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_sections_Azmy[3]++;MT_courses_sections_check_Azmy[3]++;lcd_sign_in(name, 1, 3);}}
}
else if(hour == 12 && minute >= 40 || hour == 13 && minute <= 10)
{
 //AIE AI section [5] 
 if(name == "Mansour"){
    if(AI_courses_sections_check_Mansour[5] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_sections_Mansour[5]++;AI_courses_sections_check_Mansour[5]++;lcd_sign_in(name, 1, 5);}
    }
  //MT math 3 section[0]
  else if(name == "Azmy"){
    if(MT_courses_sections_check_Azmy[0] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_sections_Azmy[0]++;MT_courses_sections_check_Azmy[0]++;lcd_sign_in(name, 1, 0);}}
}
else if(hour == 14 && minute >= 20 && minute <=50)
{
 //technical reports section [4] 
 if(name == "Mansour"){
    if(AI_courses_sections_check_Mansour[4] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_sections_Mansour[4]++;AI_courses_sections_check_Mansour[4]++;lcd_sign_in(name, 1, 4);}
    }
  //MT material properties section[5]
  else if(name == "Azmy"){
    if(MT_courses_sections_check_Azmy[5] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_sections_Azmy[5]++;MT_courses_sections_check_Azmy[5]++;lcd_sign_in(name, 1, 5);}}
}
else if(hour == 12 && minute>=10 && minute <=40){
  lcd_break_time(name);
}
else{lcd_acsses_denied(name);}
}

void Tuesday(int hour, int minute, char name[])
{                                                               
if(hour < 9 && hour > 7){
before_time(name);
}
else if(hour == 9 && minute <= 30)
{                                 
 //AI logical curcuits section [1]                                                                         
 if(name == "Mansour"){
    if(AI_courses_sections_check_Mansour[1] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_sections_Mansour[1]++;AI_courses_sections_check_Mansour[1]++;lcd_sign_in(name, 1, 1);}
    }
  // MT logical curcuits lecture [1]  
  else if(name == "Azmy"){
    if(MT_courses_lectures_check_Azmy[1] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_lectures_Azmy[1]++;MT_courses_lectures_check_Azmy[1]++;lcd_sign_in(name, 0, 1);}}
}
else if(hour == 10 && minute >= 40 || hour == 11 && minute <= 10)
{
 //AI logical curcuits lecture [1]
 if(name == "Mansour"){
    if(AI_courses_lectures_check_Mansour[1] == 1) {
      lcd_already_signed_in(name);
    }
    else{AI_courses_lectures_Mansour[1]++;AI_courses_lectures_check_Mansour[1]++;lcd_sign_in(name, 0, 1);}
    }
  //MT logical curcuits section [1]
  else if(name == "Azmy"){
    if(MT_courses_sections_check_Azmy[1] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_sections_Azmy[1]++;MT_courses_sections_check_Azmy[1]++;lcd_sign_in(name, 1, 1);}}
}
else if(hour == 12 && minute >= 40 || hour == 13 && minute <= 10)
{
  //MT material properties section[5]
  if(name == "Azmy"){
    if(MT_courses_sections_check_Azmy[5] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_sections_Azmy[5]++;MT_courses_sections_check_Azmy[5]++;lcd_sign_in(name, 1, 5);}
    }
}
else if(hour == 14 && minute >= 20 && minute <=50)
{
  //MT english 2 section[4]
  if(name == "Azmy"){
    if(MT_courses_sections_check_Azmy[4] == 1) {
      lcd_already_signed_in(name);
    }
    else{MT_courses_sections_Azmy[4]++;MT_courses_sections_check_Azmy[4]++;lcd_sign_in(name, 1, 4);}}
}
else if(hour == 12 && minute>=10 && minute <=40){
  lcd_break_time(name);
}
else{lcd_acsses_denied(name);}
}

void lcd_free_days(char name[])
{
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Welcome ");
    lcd.setCursor(8,0);
    lcd.print(name);
    lcd.setCursor(0,1);
    lcd.print("It's a free day");
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
}

void lcd_break_time(char name[]){
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Welcome ");
    lcd.setCursor(8,0);
    lcd.print(name);
    lcd.setCursor(0,1);
    lcd.print("It's a break");
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
}

void lcd_acsses_denied(char name[]){
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Welcome ");
    lcd.setCursor(8,0);
    lcd.print(name);
    lcd.setCursor(0,1);
    lcd.print("Too late");
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
}

void before_time(char name[]){
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Welcome ");
    lcd.setCursor(8,0);
    lcd.print(name);
    lcd.setCursor(0,1);
    lcd.print("start is at 9");
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
}

void lcd_already_signed_in(char name[]){
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Welcome ");
    lcd.setCursor(8,0);
    lcd.print(name);
    lcd.setCursor(0,1);
    lcd.print("You signed in");
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
}

void lcd_sign_in(char name[], int type, int subject){
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Welcome ");
    lcd.setCursor(8,0);
    lcd.print(name);
    lcd.setCursor(0,1);
    lcd.print("Attended ");
    lcd.setCursor(9,1);
    //lectures
    if(type == 0)
    {if(name == "Mansour"){
      lcd.print(AI_courses_lectures_Mansour[subject]);}
  
     else if(name == "Azmy"){
      lcd.print(MT_courses_lectures_Azmy[subject]);}
      
    }
    //sections
    else if(type == 1)
    {   
     if(name == "Mansour"){
      lcd.print(AI_courses_sections_Mansour[subject]);}
  
      else if(name == "Azmy"){
      lcd.print(MT_courses_sections_Azmy[subject]);}
      } 
    lcd.setCursor(11,1);
    lcd.print(" times");
    delay(2000);
    lcd.noBacklight();
    lcd.clear();
}

void lcd_reset_day(){
  int AI_courses_lectures_check_Mansour[6] = {0,      0,                     0,                  0,                     0,                  0};
  int AI_courses_sections_check_Mansour[6] = {0,      0,                     0,                  0,                     0,                  0};
  int MT_courses_lectures_check_Azmy[6] = {0,      0,                     0,                  0,                     0,                  0};
  int MT_courses_sections_check_Azmy[6] = {0,      0,                     0,                  0,                     0,                  0};
}