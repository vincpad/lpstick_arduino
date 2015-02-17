#include <SPI.h>

#include <SD.h>
#include <Adafruit_NeoPixel.h>
#define PIN 8
int pause = 0;
boolean  lp= false;
boolean done = false;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_GRB + NEO_KHZ800);
File myFile;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(57600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(30); //30

  Serial.print("Initializing SD card...");
   pinMode(10, OUTPUT);
   
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
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
        while (myFile.available()) {
          strip.setPixelColor(i, myFile.read(),myFile.read(),myFile.read());
          i++;
          if(i>=144){
            strip.show();
            i=0;
            delay(pause);
          }
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
    strip.setPixelColor(i,0,0,0);
  }
  strip.show();
}
