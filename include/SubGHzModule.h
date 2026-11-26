#pragma once
#include <Arduino.h>
#include "Config.h"

class SubGHzModule {
public:
    String valueList[15];
    int rfCount = 0;
    bool newSignal = false;
    bool lastButtonState = false;

    void begin();
    void capture();
    void transmit(int index);
    void clearSignal();
    void reset();
};