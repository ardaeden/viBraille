#ifndef _vBrailleDuinoH
#define _vBrailleDuinoH

#include <Arduino.h>
#include <SD.h>

#define SD_SC_PIN   10

typedef struct dict {
  char id;
  int value;
} t_dict;

int getValueFromDict(char c, t_dict *d, int dictSize);
void updateLEDs(int veri, int delayTime, int updateType);

class Button {
  protected:
    static int nextID;
  public:
    int id;
    Button(uint8_t buttonPin, long debounceDelay);
    boolean Clicked();
  private:
    uint8_t _pin;
    int _currState;            
    int _preState;   
    int _reading;
    long _lastDebounceTime;  
    long _debounceDelay; 
};

class VBD_Handler {
  public:
    VBD_Handler(void);
    void Navigate(int buttonID);
    void Parse(char *data);
    void SetFile(File file);
    void SetDelayTime(int delayTime);
    void SetFilePos(int filePos);
    void SetFileSize(unsigned long fileSize);
    unsigned long GetFileSize();
    unsigned long filePos;
  private:
    File _file;
    int _buttonID;
    long _filePos;
    char _dataRead[4];
    int _delayTime;
    unsigned long _fileSize;
    int _cellNo;
};
#endif
