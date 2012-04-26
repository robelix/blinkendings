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
  Tlc.init();
  Tlc.clear();
  randomSeed(analogRead(0));
}

void loop()
{
  // if we get a valid byte, read analog ins:
    // get incoming byte:
    
    ledNr = random(96);
    //ledNr ++;
    red = random(4096);
    green = random(4096);
    blue = random(4096);
    
    //red = 20000;
    
    tlc_setRGB(ledNr, red, green, blue);
    Tlc.update();
    delay(10);
}


