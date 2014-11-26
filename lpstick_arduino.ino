#include <SD.h>
#include <Adafruit_NeoPixel.h>
#define PIN 8
int pause = 0;
boolean  lp= true;
boolean done = false;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_GRB + NEO_KHZ800);
File myFile;

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(57600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(30);

  Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  while(lp){
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
    strip.show();
    //delay(pause);
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    Serial.println("error opening dat.dat");
    for(int i=0; i<144; i++){
      strip.setPixelColor(i,0,0,0);
    }
  }
  }

}

void loop()
{
	// nothing happens after setup
}


