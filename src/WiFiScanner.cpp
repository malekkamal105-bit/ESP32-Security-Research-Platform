#include "WiFiScanner.h"

void WiFiScanner::begin() {
    // Set the ESP32 to Station mode (client) and disconnect from any network.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
}

void WiFiScanner::scanNetworks() {
    Serial.println("Scanning...");
    networkCount = WiFi.scanNetworks(); // This function is built into the ESP32.
    
    // Cap the results at 20 so we don't overflow our arrays.
    if (networkCount > 20) networkCount = 20;

    for (int i = 0; i < networkCount; ++i) {
        ssidList[i] = WiFi.SSID(i);
        rssiList[i] = WiFi.RSSI(i);
        delay(10);
        yield(); // Feed the watchdog timer so the ESP32 doesn't crash.
    }
    WiFi.scanDelete(); // Free up memory after the scan.
}