#include "U8glib.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NO_ACK);

const int y = A0; //eixo X do joystick
int yValue;
const int x = A1; // eixo Y do joystick
int xValue;

#define be 4 //left button
boolean beState;
#define bm 5 //middle button
boolean bmState;
#define bd 7 //right button
boolean bdState;

int data = 10;
