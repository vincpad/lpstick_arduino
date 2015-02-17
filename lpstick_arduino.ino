#include <SPI.h>
#include <SD.h>
#include <avr/io.h>
#include <util/delay.h>
#include "WS2811.h"

#define BIT(B)           (0x01 << (uint8_t)(B))
#define SET_BIT_HI(V, B) (V) |= (uint8_t)BIT(B)
#define SET_BIT_LO(V, B) (V) &= (uint8_t)~BIT(B)

#define pause  1     // msec
 
// Define the output function, using pin 0 on port b.
DEFINE_WS2811_FN(WS2811RGB0, PORTB, 0)
DEFINE_WS2811_FN(WS2811RGB1, PORTD, 7)
DEFINE_WS2811_FN(WS2811RGB2, PORTB, 1)
DEFINE_WS2811_FN(WS2811RGB3, PORTC, 4)

RGB_t rgb0[36] = {0};
RGB_t rgb1[36] = {0};
RGB_t rgb2[36] = {0};
RGB_t rgb3[36] = {0};

boolean  lp= false;
boolean done = false;

File myFile; 

void setup()
{
 // Open serial communications and wait for port to open:
  //Serial.begin(57600);
  SET_BIT_HI(DDRB, 0);
  SET_BIT_HI(DDRD, 7);
  SET_BIT_HI(DDRB, 1);
  SET_BIT_HI(DDRC, 4);

  SET_BIT_LO(PORTB, 0);
  SET_BIT_LO(PORTD, 7);
  SET_BIT_LO(PORTB, 1);
  SET_BIT_LO(PORTC, 4);
  
  //Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
   
  if (!SD.begin(10)) {
    //Serial.println("initialization failed!");
    return;
  }
  //Serial.println("initialization done.");
  
  attachInterrupt(0, playOnce, RISING);
  attachInterrupt(1, startEndLoop, RISING);
}

void loop()
{
  if(lp){
    play();
    if(lp == false){
      powerOff();
    }
  }
}

void playOnce(){
  play();
  powerOff();
}

void startEndLoop(){
  if(lp) lp = false;
  else lp = true;
}

void play(){
      myFile = SD.open("data.dat");
      if (myFile) {
        int i=0;
        /*while (myFile.available()) {
          setPixelColor(i, myFile.read(),myFile.read(),myFile.read());
          i++;
          if(i>=144){
            refreshAll();
            i=0;
            //delay(pause);
          }
        } */
        for(int i=0; i<1000; i++){
            for(int j=0; j<144; j++){
                setPixelColor(j,255,0,0);
            }
            refreshAll();
            for(int j=0; j<144; j++){
                setPixelColor(j,0,255,0);
            }
            refreshAll();
            for(int j=0; j<144; j++){
                setPixelColor(j,0,0,255);
            }
            refreshAll();

        }

        // close the file:
        myFile.close();
        powerOff();
        } 
        else {
  	    // if the file didn't open, print an error:
          Serial.println("error opening dat.dat");
          powerOff();
        }
}

void powerOff(){
  delay(10);
  for(int i=0; i<144; i++){
    setPixelColor(i,0,0,0);
  }
  refreshAll();
}

void setPixelColor(int id, int r, int g, int b){
  if(id < 36){
    rgb0[id].r = r;
    rgb0[id].g = g;
    rgb0[id].b = b;
  }
  else if(id < 72){
    rgb1[id-36].r = r;
    rgb1[id-36].g = g;
    rgb1[id-36].b = b;
  }
  else if(id < 108){
    rgb2[id-72].r = r;
    rgb2[id-72].g = g;
    rgb2[id-72].b = b;
  }
  else if(id < 144){
    rgb3[id-108].r = r;
    rgb3[id-108].g = g;
    rgb3[id-108].b = b;
  }
}

void refreshAll(){
    WS2811RGB0(rgb0, ARRAYLEN(rgb0));
    WS2811RGB1(rgb1, ARRAYLEN(rgb1));
    WS2811RGB2(rgb2, ARRAYLEN(rgb2));
    WS2811RGB3(rgb3, ARRAYLEN(rgb3));
}