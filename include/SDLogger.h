#pragma once
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "Config.h"

class SDLogger {
public:
    bool isMounted = false;

    bool begin();
    void saveWiFiList(String ssids[], int rssis[], int count);
    void saveIRList(String values[], int count);
    void saveRFList(String values[], int count);
    void saveNFCList(String values[], int count);
};