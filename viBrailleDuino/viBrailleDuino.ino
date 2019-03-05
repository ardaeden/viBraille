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
  for (int i=4; i<10; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

   myFile = SD.open("output.txt");
}
void loop() {
//  while(myFile.available()) {
//    s = myFile.readStringUntil('\n');
//    switch(s[0]) {
//      case '.':
//        Serial.println("DOT");
//        updateLEDs(B100010);
//        break;
//      default:
//        Serial.println(s[0]);
//    }
//  }
  if (digitalRead(2)) digitalWrite(4, HIGH);
  else digitalWrite(4, LOW);

    if (digitalRead(3)) digitalWrite(7, HIGH);
  else digitalWrite(7,
  , LOW);
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
  int pins[6] = {6, 5, 4, 7, 8, 9};
  int mask[6] = {32, 16, 8, 4, 2, 1};
  for (int i=0; i<6; i++) {
    if ((veri & mask[i]) == mask[i])
      digitalWrite(pins[i], HIGH);
    else
      digitalWrite(pins[i], LOW);
  }
}
