#include "OneButton.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <IRremote.h>
#include "pitches.h"
#include "U8glib.h"
#include "telas.h"
//#include "fencingremote.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);

const int y = A0; //eixo X do joystick
int yValue;
const int x = A1; // eixo Y do joystick
int xValue;

#define r 0 // red, green e blue da LED RGB
int rValue; // valores pra intensidade de cada cor
#define g 1
int gValue;
#define b 2
int bValue;

#define irrx A2 // decoder IR
IRsend irsend; // LED IR (no pino 3 por default)

#define buzzer 6 // nome autoexplicativo

int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

RF24 radio(9, 10); // CE e CSN (pinos de comunicação com o NRF)
const byte addresses[][6] = {"00001", "00002"};// endereços do radio

#define be 4 //left button
boolean beState;
#define bm 5 //middle button
boolean bmState;
#define bd 7 //right button
boolean bdState;
//OneButton button(4, true); //botões com a biblioteca OneButton
//OneButton button(5, true);
//OneButton button(7, true);

int data = 10;

int frame = 0;
int ok = 0;

void setup() {
  delay(100);  // This delay is needed to let the display to initialize

  pinMode(be, INPUT_PULLUP);
  pinMode(bm, INPUT_PULLUP);
  pinMode(bd, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);

  
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MAX);
  
  Serial.begin(9600);
}

void loop() {
  // definindo a leitura dos botões
bdState = digitalRead(bd);
beState = digitalRead(be);
bmState = digitalRead(bm);


 if(ok == 0){
   if(bdState == LOW){
  frame ++;
  tone(buzzer, 622, 250);
  delay(200);
 }
 if(beState == LOW){
  frame --;
  tone(buzzer, 622, 250);
  delay(200);
 }

 
 if(bmState == LOW){
  ok++;
  tone(buzzer, 622, 250);
  delay(200);
 }
 u8g.firstPage();
  do{
   draw();
 } while(u8g.nextPage());
}

 
 if(ok ==1){
  u8g.firstPage();
  do{
   apps();
 } while(u8g.nextPage());
 }
 if(ok == 1 && beState == LOW){
  ok--;
  tone(buzzer, 622, 250);
  delay(200);
 }
}

void draw()
{
  if(frame==0)
   u8g.drawBitmapP( 0, 0, 16, 64, frame1);
  else if (frame == 1)
   u8g.drawBitmapP( 0, 0, 16, 64, frame2);
  else if(frame == 2)
   u8g.drawBitmapP( 0, 0, 16, 64, frame3);
   else if(frame == 3)
   u8g.drawBitmapP( 0, 0, 16, 64, frame4);
   else if(frame == 4)
   u8g.drawBitmapP( 0, 0, 16, 64, frame5);
}

void apps(){
  if(frame == 0){
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 20); 
    u8g.print("IR REMOTE!!!");
  }
  
  else if(frame == 1){
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 20); 
    u8g.print("RF REMOTE!!!");
    

    rf1();
  }
  
  else if(frame == 2){
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 20); 
    u8g.print("GAMES!!!");
  }

   else if(frame == 3){
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 20); 
    u8g.print("ACCELEROMETER!!!");
  }

   else if(frame == 4){
    u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 20); 
    u8g.print("SETTINGS!!!");
  }
}

void rf1(){
 xValue = analogRead(x);
 yValue = analogRead(y);

 beState = digitalRead(be);
 bmState = digitalRead(bm);
 bdState = digitalRead(bd);
 
 u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 35); 
    u8g.print("X:");

 u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(15, 35); 
    u8g.print(xValue);

 u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(0, 55); 
    u8g.print("Y:");

 u8g.setFont(u8g_font_unifont);
    u8g.setPrintPos(15, 55); 
    u8g.print(yValue);

if(bdState == LOW){ //b direita
  data = 8;
  radio.write(&data, sizeof(data));
  }

  if(beState == LOW){ //b esquerda
  data = 9;
  radio.write(&data, sizeof(data));
  }

if(xValue > 700){ // direita
  data = 6;
  radio.write(&data, sizeof(data));
}
if(xValue < 200){ //esquerda
  data = 7;
  radio.write(&data, sizeof(data));
}

if(yValue > 700){ //p/ cima
  data = 4;
  radio.write(&data, sizeof(data));
}
if(yValue < 200){ //p/ baixo
  data = 5;
  radio.write(&data, sizeof(data));
}
}
