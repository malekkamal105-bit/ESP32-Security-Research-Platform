#include "IRModule.h"

void IRModule::begin() {
    // Use the 3-argument version to enable feedback on the built-in LED (usually pin 13 or the default)
    IrReceiver.begin(IR_RX_PIN, LED_BUILTIN, ENABLE_LED_FEEDBACK);
    IrSender.begin(IR_TX_PIN); 
}

void IRModule::capture() {
    if (IrReceiver.decode()) {
        uint32_t val = IrReceiver.decodedIRData.decodedRawData;
        
        // 1. Check if the remote is sending a "repeat" code (0x00000000).
        bool isRepeat = (val == 0x00000000);
        
        // 2. Check if this is the exact same button pressed within 300ms.
        bool isDuplicate = (val == lastRawValue && (millis() - lastCaptureTime < 300));

        // Only save if it's a real signal, not a duplicate, and we have space.
        if (!isRepeat && !isDuplicate && irCount < 15) {
            protocolList[irCount] = IrReceiver.decodedIRData.protocol;
            addressList[irCount] = IrReceiver.decodedIRData.address;
            commandList[irCount] = IrReceiver.decodedIRData.command;
            
            // Format the raw hex value into a string for the screen.
            char buf[15];
            sprintf(buf, "0x%08lX", (unsigned long)val);
            valueList[irCount] = String(buf);
            
            irCount++;
            newSignal = true;
            
            lastRawValue = val;
            lastCaptureTime = millis();
        }
        IrReceiver.resume(); // Get ready for the next signal.
    }
}

void IRModule::transmit(int index) {
    Serial.print(">>> TRANSMITTING: ");
    Serial.println(valueList[index]);
    
    // The modern IRremote library requires protocol-specific sending functions.
    switch (protocolList[index]) {
        case NEC:    IrSender.sendNEC(addressList[index], commandList[index], 0); break;
        case SONY:   IrSender.sendSony(addressList[index], commandList[index], 0); break;
        case RC5:    IrSender.sendRC5(addressList[index], commandList[index], 0); break;
        case RC6:    IrSender.sendRC6(addressList[index], commandList[index], 0); break;
        case SAMSUNG:IrSender.sendSamsung(addressList[index], commandList[index], 0); break;
        default:  
           Serial.println("Unsupported protocol.");
            break;
    }
}

void IRModule::clearSignal() { 
    newSignal = false; 
}

void IRModule::reset() { 
    irCount = 0; 
    newSignal = false; 
    lastRawValue = 0;
    lastCaptureTime = 0;
}