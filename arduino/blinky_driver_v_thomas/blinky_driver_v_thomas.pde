#include "Tlc5940.h"


void setup()
{
  /* Call Tlc.init() to setup the tlc.
     You can optionally pass an initial PWM value (0 - 4095) for all channels.*/
  Tlc.init();
  Serial.begin(9600);
  Serial.println("RESET");
  
  pinMode(A0, INPUT);
}

/* This loop will create a Knight Rider-like effect if you have LEDs plugged
   into all the TLC outputs.  NUM_TLCS is defined in "tlc_config.h" in the
   library folder.  After editing tlc_config.h for your setup, delete the
   Tlc5940.o file to save the changes. */

int Board_boards = 0;
    
class Board
{
    static const int maxLeds = 16;
    int tlcStartAddress;
    int lastLedSet;
    boolean rotated;
    
public:
   Board(boolean rotiert=false)
   {
      rotated = rotiert;
      tlcStartAddress = Board_boards * 48;
      Serial.println(tlcStartAddress);
      Board_boards++;
      lastLedSet = 0;
   };
   
   void setColor(int r, int g, int b)
   {
       for(int i=0;i<maxLeds*3;i+=3)
       {
          Tlc.set(tlcStartAddress + i, r); // led rot
          Tlc.set(tlcStartAddress + i + 1, g); // led GRÜN
          Tlc.set(tlcStartAddress + i + 2, b); // led BLAU
       };
   };
   
   inline void setLed(int led, int r, int g, int b)
   {
       led = led % maxLeds;
       lastLedSet = led;
       if (rotated) {
         led = 15-led;
       }
       Tlc.set(tlcStartAddress + led*3, r); // led rot
       Tlc.set(tlcStartAddress + led*3 + 1, g); // led GRÜN
       Tlc.set(tlcStartAddress + led*3 + 2, b); // led BLAU
   };
   
   //inline void setLedXY(int x, 
   
   inline int lastLed()
    {
      return lastLedSet;
    };
  
   void effectLeftRight(int r, int g, int b, int del=100)
   {
       for(int i=0;i<maxLeds/4;i++)
       {
          setLed(i,r,g,b);
          setLed(i+4,r,g,b);
          setLed(i+8,r,g,b);
          setLed(i+12,r,g,b);
          Tlc.update();
          delay(del);
       };
   };
   
   void effectTest(int r, int g, int b, int del=100)
   {
       for(int i=0;i<maxLeds/2;i++)
       {
          setLed(i,r,g,b);
          setLed(15-i,r,g,b);
          Tlc.update();
          delay(del);
       };
   };
};


#define NR_BOARDS 8
Board board[NR_BOARDS] = { Board(), Board(), Board(), Board(), Board(true), Board(true), Board(true), Board(true) };


int spiralMap[16] = { 1, 2, 3, 7, 5, 6, 10, 11, 4, 0, 9, 15, 8, 12 ,13 ,14 };

int next(int last)
{
  return last+3;
}

double ran=100;

void loop()
{
  int x = 1024;
  
  static int c[3]={2048,0,0};
  
  if(random()%25==0)
  {
     int r = random()%3;
     c[r] = (c[r]+3333)%4096; 
  }
  
   for(unsigned int i=0;i<NR_BOARDS;i++)
   {
        //reset prev board to 0 ... 
        //board[(i-1)%9].setColor(0,0,0);
        
        //board[i].setColor(random()%x,random()%x,random()%x);
        //board[i].effectTest(random()%x,random()%x,random()%x,300);
        board[i].setLed(board[i].lastLed(),c[0]/25,c[1]/25,c[2]/25);
        board[i].setLed(next(board[i].lastLed()),c[0],c[1],c[2]);
        //board[i].setLed(random()%16,0,0,0);
        Tlc.update();
   }
   double poti = analogRead(A0);
   if (random(1) ==0) {
     ran++;
   } else {
     ran--;
   }
   
   delayMicroseconds(poti*ran);
}


