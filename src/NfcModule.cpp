#include "NFCModule.h"

void NFCModule::begin() { 
    pinMode(SIM_NFC_RX_PIN, INPUT_PULLUP); 
}

void NFCModule::capture() {
    bool buttonState = digitalRead(SIM_NFC_RX_PIN) == LOW;
    if (buttonState && !lastButtonState) {
        if (nfcCount < 15) {
            long uid = random(0x100000, 0xFFFFFF);
            char buf[15];
            sprintf(buf, "0x%06lX", uid);
            valueList[nfcCount] = String(buf);
            nfcCount++;
            newSignal = true;
        }
    }
    lastButtonState = buttonState;
}

void NFCModule::transmit(int index) {
    Serial.print(">>> EMULATING NFC TAG: ");
    Serial.println(valueList[index]);
}

void NFCModule::clearSignal() { newSignal = false; }

void NFCModule::reset() { 
    nfcCount = 0; 
    newSignal = false; 
}