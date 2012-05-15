/*
    A simple 1-d oscilliscope: scan all the channels, setting the PWM output
    value to 4x the analog pin 0 value (0 - 1024 * 4 = 4096).  The value will
    fade to zero as the channels keep scanning.

    See the BasicUse example for hardware setup.

    Alex Leone <acleone ~AT~ gmail.com>, 2009-02-03 */

#include "tlc_config.h"
#include "Tlc5940.h"

// which analog pin to use
#define ANALOG_PIN      0



// RGB-LED settings - color offsets
#define LED_R  2
#define LED_G  1
#define LED_B  0


// set rgb 
void tlc_setRGB(TLC_CHANNEL_TYPE channel, uint16_t valueR, uint16_t valueG, uint16_t valueB) {
  TLC_CHANNEL_TYPE tlc_realChannel = channel*3;
  Tlc.set(tlc_realChannel + LED_R, valueR);
  Tlc.set(tlc_realChannel + LED_G, valueG);
  Tlc.set(tlc_realChannel + LED_B, valueB);
}

int getColor(int nr, int run) {
  return random(406);
  if( run & nr ) {
    return 500;
  } else { 
    return 0;
  }
}

void setup()
{
  Tlc.init();
}

void loop()
{

  int run = 0;  
  int direction = 1;
  int r;
  int g;
  int b;
  for (int channel = 0; channel < NUM_TLCS*16/3; channel += direction) {

    /* Tlc.clear() sets all the grayscale values to zero, but does not send
       them to the TLCs.  To actually send the data, call Tlc.update() */
    //Tlc.clear();

    /* Tlc.set(channel (0-15), value (0-4095)) sets the grayscale value for
       one channel (15 is OUT15 on the first TLC, if multiple TLCs are daisy-
       chained, then channel = 16 would be OUT0 of the second TLC, etc.).

       value goes from off (0) to always on (4095).

       Like Tlc.clear(), this function only sets up the data, Tlc.update()
       will send the data. */
    
    if (channel == 0) {
      direction = 1;
      run++;
      if (run >= 8) {
        Tlc.clear();
        run=1;
      }
      r = getColor(1,run);
      g = getColor(2,run);
      b = getColor(4,run);
    } else {
      tlc_setRGB(channel - 1, r, g, b);
    }
    tlc_setRGB(channel, r,g,b);
    if (channel != NUM_TLCS*16/3 - 1) {
      tlc_setRGB(channel + 1, r,g,b);
    } else {
      direction = -1;
    }

    /* Tlc.update() sends the data to the TLCs.  This is when the LEDs will
       actually change. */
    Tlc.update();

    delay(50);
  }
  
}


