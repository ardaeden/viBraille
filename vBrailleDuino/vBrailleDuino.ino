#include <SPI.h>
#include <SD.h>
#include "vBrailleDuino.h"

#define NUM_BTNS 2

uint8_t mode = 1; //mode is 1 for reading mode, 0 for file mode

Button btn1(2, 10);
Button btn2(3, 10);
Button *btns[] {&btn1, &btn2};

VBD_Handler handler;

File file;

void setup() {

  Serial.begin(9600);
  Serial.println("Initializing SD card ...");
  
  if (!SD.begin(SD_SC_PIN)) {
    Serial.println("Initialization failed !!!");
    while(1);
  }
  
  else {
    Serial.println("SD card successfully initialized ..."); 
  }

  if (file = SD.open("output.txt")) {
    Serial.println("File successfully opened ...");
  }
  
  else {
    Serial.println("File could not open ...");
  }

  handler.SetFile(file);
}

void loop() {

  switch(mode) { 
    case 0:
      break;
    case 1:
      checkButtons();
      break;
  }
}

void checkButtons() {
  for (int i=0; i<NUM_BTNS; i++) {
    if (btns[i]->Clicked()) {
      handler.Update(btns[i]->id);  
    }
  }
}
