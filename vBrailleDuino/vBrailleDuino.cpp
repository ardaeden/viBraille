#include "vBrailleDuino.h"


t_dict notenames[7] = { {'c', B100110},
                        {'d', B100010},
                        {'e', B110100},
                        {'f', B110110},
                        {'g', B110010},
                        {'a', B010100},
                        {'b', B010110}
                      };

                      
int Button::nextID = 0;
                  
Button::Button(uint8_t buttonPin, long debounceDelay) {
  id = ++nextID;
  _pin = buttonPin;
  _currState = 0;
  _preState = 0;
  _reading = 0;
  _prepreState = 0;
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
//_filePos = -2;
}

void VBD_Handler::Update(int buttonID) {

  _buttonID = buttonID;
  switch(_buttonID) {
    
    case 1:                       //Left button clicked 
      _filePos=_filePos-_readLen-1;
      Serial.println(_filePos);
      _file.seek(_filePos);
      _tmp = _file.readStringUntil('\n');
      Serial.print(_tmp);
      Serial.print("  ");
      Serial.print(_tmp.length());
      Serial.print("  ");
      //_filePos = _file.position();
      Serial.println(_filePos);

      //updateLEDs(getValueFromDict(tmp, notenames, 7));
      break;
      
    case 2:                       //Right button clicked

      _tmp = _file.readStringUntil('\n');
      Serial.print(_tmp);
      Serial.print("  ");
      Serial.print(_tmp.length());
      Serial.print("  ");
      _filePos = _file.position();
      _readLen = _tmp.length();
      Serial.println(_filePos);
      
      //updateLEDs(getValueFromDict(tmp, notenames, 7));
      break;
  }
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
void updateLEDs(int veri) {
  int pins[6] = {4, 5, 6, 7, 8, 9};
  int mask[6] = {32, 16, 8, 4, 2, 1};
  for (int i=0; i<6; i++) {
    if ((veri & mask[i]) == mask[i]) {
      digitalWrite(pins[i], HIGH);
      delay(500);
      digitalWrite(pins[i], LOW);
    }
    else
      digitalWrite(pins[i], LOW);
  }
}
