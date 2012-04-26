/*
  Serial Call and Response
 Language: Wiring/Arduino

  Serial -> 3x TLC5940 mit 4x4 RGB-LEDS
  
  Daten:
  
  1. Byte - LED Nr
  2. Byte - Red
  3. Byte - Green 
  4. Byte - Blue
  
  FF FF FF FF -> update / reset

 */
 
#include "tlc_config.h"
#include "Tlc5940.h"

 
 // RGB-LED settings - color offsets
#define LED_R  0
#define LED_G  1
#define LED_B  2

#define COLORS 3

#define BYTE_LEDID  0
#define BYTE_RED    1
#define BYTE_GREEN  2
#define BYTE_BLUE   3

#define BYTENR_MAX 3


int inByte = 0;        // incoming serial byte
int byteNr = 0;        // current Byte-Nr.
int ffCounter = 0;      // ff (reset) Byte Counter

int ledNr = 0;         // current Led Nr.
int red   = 0;         // current color values
int green = 0;
int blue  = 0;


// set rgb 
void tlc_setRGB(TLC_CHANNEL_TYPE channel, uint16_t valueR, uint16_t valueG, uint16_t valueB) {
  TLC_CHANNEL_TYPE tlc_realChannel = channel*COLORS;
  Tlc.set(tlc_realChannel + LED_R, valueR);
  Tlc.set(tlc_realChannel + LED_G, valueG);
  Tlc.set(tlc_realChannel + LED_B, valueB);
}


void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(115200);
  Tlc.init();
  Tlc.clear();
}

void loop()
{
  // if we get a valid byte, read analog ins:
  while (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    
    // Byte 0 - LED Nr    
    if (byteNr == BYTE_LEDID) {
        ledNr = inByte;
        //Serial.print('L', BYTE);
    }
    
    // Byte 1 - RED
    if (byteNr == BYTE_RED) {
      red = inByte;
      //Serial.print('R', BYTE);
    }
    
    // Byte 2 - GREEN
    if (byteNr == BYTE_GREEN) {
      green = inByte;
      //Serial.print('G', BYTE);
    }

    // Byte 3 - BLUE
    if (byteNr == BYTE_BLUE) {
      blue = inByte;
      //Serial.print('B', BYTE);
    }
    
    byteNr++;

    // write value after blue received    
    if (byteNr > BYTENR_MAX) {
        byteNr = 0;
        if (ledNr < 96) {
          tlc_setRGB(ledNr, 16*red, 16*green, 16*blue);
          //Tlc.update();
        }
    }

    // update/reset count
    if (inByte == 255) {
      ffCounter++;
      //Serial.print('F', BYTE);
    } else {
      //Serial.print('0', BYTE);
      ffCounter = 0;
    }
    
    // 4x FF = reset/update
    if (ffCounter >= 4) {
       Tlc.update();
       byteNr = 0;
       //Serial.print('X', BYTE);
    }

  }
}


