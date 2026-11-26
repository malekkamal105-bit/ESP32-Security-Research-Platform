#pragma once
#include <Arduino.h>
#include "Config.h"

class NFCModule {
public:
    String valueList[15]; 
    int nfcCount = 0;
    bool newSignal = false;
    bool lastButtonState = false;

    void begin();
    void capture();
    void transmit(int index);
    void clearSignal();
    void reset();
};