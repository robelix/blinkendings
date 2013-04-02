1) copy library to Sketchbook directory -> libraries
2) reopen Arduino IDE
3) import library

example programm

this will constantly draw the ITS logo to the wall

8<--------8<------8<------

#include "tlc_config.h"
#include "Tlc5940.h"
#include "BlinkenWall.h"
#include "Animations.h"

BlinkenWall blinkenWall(150);

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
    }
}

------>8-------->8------>8
