#include <SPI.h>
#include <SD.h>
#include "vBrailleDuino.h"

#define NUM_BTNS 2

int intervalDelayTime = 1000;
uint8_t mode = 1; //mode is 1 for reading mode, 0 for file mode
String command;

Button btn1(2, 10);
Button btn2(3, 10);
Button *btns[] {&btn1, &btn2};

VBD_Handler handler;

File file;

void setup() {
  for (int i=4; i<10; i++) {
    pinMode(i, OUTPUT);
  }
  Serial.begin(115200);
  Serial.println("Initializing SD card ...");
  
  if (!SD.begin(SD_SC_PIN)) {
    Serial.println("Initialization failed !!!");
    while(1);
  }
  
  else {
    Serial.println("SD card successfully initialized ...");
  }


  if (file) file.close();
  if (file = SD.open("d1")) {
    Serial.print("Opened file: ");
    Serial.println("d1");
    handler.SetFile(file);
    handler.SetFileSize(file.size());
    unsigned long fs = handler.GetFileSize();
    Serial.print("File length: ");
    Serial.println(fs);
    Serial.print("Number of cells: ");
    Serial.println(fs/4);
    handler.SetFilePos(0);
  }
  
  else {
    Serial.println("File could not open ...");
  }

  handler.SetFile(file); //Bu if clause içinde olmalı sanki
}

void loop() {
  readSerialAndParseCommand();
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
      handler.Navigate(btns[i]->id);  
    }
  }
}

void parseCommand(String cmd) {
  String part1, part2;
  part1 = cmd.substring(0, cmd.indexOf(" "));
  part2 = cmd.substring(cmd.indexOf(" ")+1);
        
  if(part1.equalsIgnoreCase("delay")) {
    int data = part2.toInt();
    handler.SetDelayTime(data);
    Serial.print("Delay time set to: ");
    Serial.println(data);
  }
  else if(part1.equalsIgnoreCase("goto")) {
    int data = part2.toInt();
    if (data<1 || data>(handler.GetFileSize()/4)) {
      Serial.println("Out of file size...");
    }
    else {
      handler.SetFilePos((data-1)*4); 
      Serial.print("Cell position set to: ");
      Serial.println(data);
    } 
  }
  else if (part1.equalsIgnoreCase("open")) {
    if (file) file.close();
    if (file = SD.open(part2)) {
      Serial.print("Opened file: ");
      Serial.println(part2);
      handler.SetFile(file);
      handler.SetFileSize(file.size());
      unsigned long fs = handler.GetFileSize();
      Serial.print("File length: ");
      Serial.println(fs);
      Serial.print("Number of cells: ");
      Serial.println(fs/4);
      handler.SetFilePos(0);
    }
  
    else {
      Serial.println("File could not open ...");
    }
    
  }
  else {
    Serial.println("Command not identified...");
  }
}

void readSerialAndParseCommand() {
if(Serial.available()) {
    char c = Serial.read();

    if(c=='\n') {
      parseCommand(command);
      command = "";
    }
    else {
      command += c;
    }
  }  
}
