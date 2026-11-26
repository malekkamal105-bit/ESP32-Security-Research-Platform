#pragma once
#include <WiFi.h>
#include <Arduino.h>

class WiFiScanner {
public:
    // Arrays to store scan results (max 20 networks).
    String ssidList[20];
    int rssiList[20];
    int networkCount = 0;

    void begin();        // Initializes Wi-Fi radio
    void scanNetworks(); // Performs the scan and stores results
};