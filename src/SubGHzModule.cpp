#include "SubGHzModule.h"

void SubGHzModule::begin() {
    // Uses a physical/virtual button to simulate receiving RF
    pinMode(SIM_RF_RX_PIN, INPUT_PULLUP);
}

void SubGHzModule::capture() {
    bool buttonState = digitalRead(SIM_RF_RX_PIN) == LOW;
    
    // Edge detection for the simulated RF button
    if (buttonState && !lastButtonState) {
        if (rfCount < 15) {
            // Generates a fake 433MHz code (like a garage remote)
            long randomCode = random(0x100000, 0xFFFFFF);
            char buf[15];
            sprintf(buf, "0x%06lX", randomCode);
            valueList[rfCount] = String(buf);
            rfCount++;
            newSignal = true;
        }
    }
    lastButtonState = buttonState;
}

void SubGHzModule::transmit(int index) {
    // On real hardware, this would trigger the CC1101 SPI commands
    Serial.print(">>> TRANSMITTING RF: ");
    Serial.println(valueList[index]);
}

void SubGHzModule::clearSignal() { newSignal = false; }

void SubGHzModule::reset() { 
    rfCount = 0; 
    newSignal = false; 
}