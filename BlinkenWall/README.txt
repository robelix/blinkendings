1) copy library to Sketchbook directory -> libraries
2) reopen Arduino IDE
3) import library


This library depends on the TI Tlc5940 library.
For installation instructions please visit

http://playground.arduino.cc/learning/TLC5940

newer version might require other and/or additional
header-files to be included.


For more information on how to create a animation
please read the comments in BlinkenWall.h

example programm

this will constantly draw the ITS logo to the wall

8<--------8<------8<------

#include "tlc_config.h"
#include "Tlc5940.h"
#include "BlinkenWall.h"
#include "Animations.h"

/* delay 150 ms, black default background */
BlinkenWall blinkenWall(150, 0);

void
setup()
{
    blinkenWall.setup()
}

void
loop()
{
    int frame = 0;
    while(1) {
        blinkenWall.drawAnimation(&itssyndikat, 0, 0, 0, frame++);
        if (frame > 2)
            frame = 0;
        blinkenWall.update();
        blinkenWall.clear();
    }
}

------>8-------->8------>8
