#include "viBrailleDuino.h"
#include <SPI.h>
#include <SD.h>

File myFile;
String s;
t_dict notes[7] = { {'c', B100110},
                    {'d', B100010},
                    {'e', B110100},
                    {'f', B110110},
                    {'g', B110010},
                    {'a', B010100},
                    {'b', B010110}
                  };

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
//  if (myFile) {
//    Serial.print("Writing to test.txt...");
//    myFile.println("testing 1, 2, 3.");
//    // close the file:
//    myFile.close();
//    Serial.println("done.");
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }

  // re-open the file for reading:
  myFile = SD.open("output.txt");
//  if (myFile) {
//    Serial.println("output.txt:");
//
//    // read from the file until there's nothing else in it:
//    while (myFile.available()) {
//      Serial.write(myFile.read());
//    }
//    // close the file:
//    myFile.close();
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }
}

void loop() {
  while(myFile.available()) {
    s = myFile.readStringUntil('\n');
    switch(s[0]) {
      case '.':
        Serial.println("DOT");
        updateLEDs(B001000);
        break;
      default:
        Serial.println(s[0]);
    }
  }
if (digitalRead(2)==LOW) Serial.println("pressed");
else Serial.println("Not Pressed");
}


int getValueFromDict(char c, t_dict *d, int dictSize) {
  int i, val;
  for (i=0; i<dictSize; i++) {
    if (d[i].id==c) {
      val=d[i].value;
      break;
    }
    
  }
  return (i<dictSize) ? val : -1;
}


void updateLEDs(int veri) {
  int pins[6] = {2, 3, 5, 6, 7, 8};
  int mask[6] = {32, 16, 8, 4, 2, 1};
  for (int i=0; i<6; i++) {
    if ((veri & mask[i]) == mask[i])
      digitalWrite(pins[i], HIGH);
    else
      digitalWrite(pins[i], LOW);
  }
}
