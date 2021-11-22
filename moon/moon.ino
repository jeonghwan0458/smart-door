#include <DFPlayer_Mini_Mp3.h>
#include <Adafruit_GFX.h>
#include <SoftwareSerial.h>
#include "DM_G_I2C.h"
#include <SPI.h>
#include <Wire.h>

#include <Adafruit_SSD1306.h>

#define DOORLOCK 12
#define PRESS A0
#define RX 7
#define TX 8
#define TRIG 10 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO 9 //ECHO 핀 설정 (초음파 받는 핀)
SoftwareSerial bluetooth(RX, TX);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  mp3_set_serial(Serial);

  delay(1);
  mp3_set_volume(30);
  pinMode(DOORLOCK, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(PRESS, INPUT);
  G_I2C_Scanner(); delay(1000);
  OLED_SETUP();

}

void loop() {
  digitalWrite(DOORLOCK, LOW);
  int value = analogRead(A0);        //압력 센서의 데이터를 저장
  if (value > 1000) {
    delay(500);
    long duration, distance;

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duration = pulseIn(ECHO, HIGH);
    distance = duration * 17 / 1000;
    Serial.println(distance);

    if (distance < 10) {
      mp3_play(1);    //0001 파일 재생
      OLED_Display(2,0,10,"Dangerous");
      bluetooth.write('1');
      delay(2000);
      mp3_play(1);
      delay(3000);
      OLED_Display_clear_display();
      digitalWrite(DOORLOCK, HIGH);
      delay(500);
      digitalWrite(DOORLOCK, LOW);
      while(value < 1000){
        value = digitalRead(A0);
      }
    } else {
      digitalWrite(DOORLOCK, HIGH);
      delay(500);
      digitalWrite(DOORLOCK, LOW);
      while(value < 1000){
        value = digitalRead(A0);
      }
      

    }
    
  }







}
