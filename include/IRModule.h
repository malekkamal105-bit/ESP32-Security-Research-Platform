#pragma once
#include <Arduino.h>
#include <IRremote.h>
#include "Config.h"

class IRModule {
public:
    // Arrays to store the history of captured IR signals.
    String valueList[15];
    decode_type_t protocolList[15];
    uint16_t addressList[15];
    uint16_t commandList[15];
    int irCount = 0;
    bool newSignal = false;
    
    // Variables to filter out duplicate codes and holding-down repeats.
    uint32_t lastRawValue = 0;
    unsigned long lastCaptureTime = 0;

    // Sets up the receiver and transmitter pins.
    void begin();
    
    // Listens for a signal and saves it if it's new.
    void capture();
    
    // Sends the saved signal at the given index.
    void transmit(int index);
    
    // Tells the main loop we have handled the new signal.
    void clearSignal();
    
    // Clears the history when entering the IR menu.
    void reset();
};