#include "vBrailleDuino.h"


t_dict noteNames[7] =         { {'c', B001110},
                                {'d', B001010},
                                {'e', B011100},
                                {'f', B011110},
                                {'g', B011010},
                                {'a', B010100},
                                {'b', B010110}
                              };

t_dict noteValues[4]    =    { {'0', B100001},
                               {'1', B100000},
                               {'2', B000001},
                               {'3', B000000}
                             };

t_dict octaveNumbers[7] =    { {'1', B000100},
                                {'2', B000110},
                                {'3', B000111},
                                {'4', B000010},
                                {'5', B000101},
                                {'6', B000011},
                                {'7', B000001}
                              };

t_dict rests[4] =             { {'0', B101100},
                                {'1', B101001},
                                {'2', B111001},
                                {'3', B101101}
                              };

t_dict accidentalsAndDot[4] = { {'n', B001001},
                                {'+', B001101},
                                {'-', B011001},
                                {'.', B100000}
                              };

t_dict measures[1] =          { {'|', B111111}};
                      
                      
int Button::nextID = 0;

int uType=0;
                  
Button::Button(uint8_t buttonPin, long debounceDelay) {
  id = ++nextID;
  _pin = buttonPin;
  _currState = 0;
  _preState = 0;
  _reading = 0;
  _lastDebounceTime = 0;
  _debounceDelay = debounceDelay;
  pinMode(_pin, INPUT);
}

boolean Button::Clicked(){
  _reading = digitalRead(_pin);

  if (_reading != _currState) {
    _lastDebounceTime = millis();
    _currState = _reading;
  }
   
  if (_reading != _preState) {
    if ( millis() - _lastDebounceTime > _debounceDelay ){
      if (_reading == HIGH){ 
        if (_preState == LOW){
          _preState = _reading;
          return true;  
        }
      }
      else  {
        _preState = _reading;
      }
    }
  }
  return false;
}



/***********************************/
/*THIS IS THE HANDLER CLASS FOR VBD*/
/***********************************/

VBD_Handler::VBD_Handler() {
_filePos = 0;
}

void VBD_Handler::Navigate(int buttonID) {

  _buttonID = buttonID;
  switch(_buttonID) {
    
    case 1:                       //Left button clicked
      _filePos = _filePos - 4;
      _filePos = _filePos < 0 ? 0 : _filePos;
      for (int i=0; i<4; i++) {
        _file.seek(_filePos + i);
        _dataRead[i] = _file.peek();
        Serial.print(_dataRead[i]);
      }
      Serial.println();
      Parse(_dataRead);
      break;
      
    case 2:                       //Right button clicked
      for (int i=0; i<4; i++) {
        _file.seek(_filePos + i);
        _dataRead[i] = _file.peek();
        Serial.print(_dataRead[i]);
      }
      Serial.println();
      _filePos = (_filePos == _file.size() - 4) ? _filePos : _filePos + 4;
      Parse(_dataRead);
      break;
  }
}

void VBD_Handler::Parse(char *data) {
  uint8_t ledState=0;
  switch(data[0]) {
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'a':
    case 'b':
      ledState =    getValueFromDict(data[0], noteNames, 7)
                  + getValueFromDict(data[1], noteValues, 4);
      uType = 0;
      break;

    case 'r':
      ledState = getValueFromDict(data[1], rests, 4);
      uType = 0;
      break;
      
    case 'o':
      ledState = getValueFromDict(data[1], octaveNumbers, 7);
      uType = 0;
      break;

    case 'n':
    case '+':
    case '-':
    case '.':
      ledState = getValueFromDict(data[0], accidentalsAndDot, 4);
      uType = 0;
      break;

    case '|':
      ledState = getValueFromDict(data[0], measures, 1);
      uType = 1;
      break;
    
  }
  updateLEDs(ledState, MOTOR_INTERVAL_DELAY_TIME, uType);
}


void VBD_Handler::SetFile(File file) {
  _file = file;
}



//Function to read from dictionary
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


//UPDATE LEDs
void updateLEDs(int cell, int delayTime, int updateType) {
  uint8_t idx[6]  = {2, 1, 0, 3, 4, 5};
  uint8_t pins[6] = {6, 5, 4, 7, 8, 9};
  uint8_t mask[6] = {32, 16, 8, 4, 2, 1};
  
  switch (updateType) {

  case 0:
      for (int i=0; i<6; i++) {
        if ((cell & mask[idx[i]]) == mask[idx[i]]) {
          digitalWrite(pins[idx[i]], HIGH);
          Serial.println(pins[idx[i]]-3);
          delay(delayTime);
          digitalWrite(pins[idx[i]], LOW);
        }
        else
          digitalWrite(pins[idx[i]], LOW);
      }
      break;
      
   case 1:
      for (int i=0; i<6; i++) {
        if ((cell & mask[i]) == mask[i]) {
          digitalWrite(pins[i], HIGH);
          Serial.println(pins[i]-3);
        }
        else
          digitalWrite(pins[i], LOW);
      }
      delay (300);
      for (int i=0; i<6; i++) {
        digitalWrite(pins[i], LOW);
      }
  }
}
