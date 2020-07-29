/* Mahmudul Islam
 * EEE,Bangladesh University of Engineering and Technology (BUET)
 * Email: mahmudulislam299@gmail.com
 */

#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#define bz 10
#include <Servo.h>
Servo myservo;  
int pos = 0;
int Vsecond, Vminute, Vhour;


byte decToBcd(byte val){
  // Convert normal decimal numbers to binary coded decimal
  return( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val){
  // Convert binary coded decimal to normal decimal numbers
  return( (val/16*10) + (val%16) );
}

void setup(){
  Wire.begin();
  Serial.begin(9600);
  pinMode(bz, OUTPUT);
  myservo.attach(9);
  myservo.write(0);
  delay(5000);
  myservo.write(0);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  //setDS3231time(0,30,14,3,29,05,18);
}

/*void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}*/

void readDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte *dayOfMonth, byte *month, byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void food() {
  for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
    
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(3);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(3);                       // waits 15ms for the servo to reach the position
  }
}

void buzzer() {
  analogWrite(bz, 255);
  delay(100);
  analogWrite(bz,0);
  delay(100);
  analogWrite(bz, 255);
  delay(100);
  analogWrite(bz,0);
  }

void Time(){
  
  // retrieve data from DS3231
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;

  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
  Vsecond= second;
  Vminute=minute;
  Vhour=hour;
  /*Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.print(second);
  Serial.println();*/
  
}


void loop(){
  Time(); // display the real-time clock data on the Serial Monitor,
  Serial.print(Vhour);
  Serial.print(":");
  Serial.print(Vminute);
  Serial.print(":");
  Serial.println(Vsecond);

  
  if(Vhour>5 && Vhour<19 && Vhour%3==0 && Vminute==0 && Vsecond==0)
  {
    buzzer();
    food();
    delay(2000);
  }
}
