#include "SDLogger.h"

bool SDLogger::begin() {
    if (!SD.begin(SD_CS)) {
        isMounted = false;
        return false;
    }
    isMounted = true;
    return true;
}

void SDLogger::saveWiFiList(String ssids[], int rssis[], int count) {
    File file = SD.open("/wifi_scan.csv", FILE_WRITE);
    if (!file) return;
    file.println("SSID,RSSI");
    for (int i = 0; i < count; i++) { file.print(ssids[i]); file.print(","); file.println(rssis[i]); }
    file.close();
}

void SDLogger::saveIRList(String values[], int count) {
    File file = SD.open("/ir_signals.txt", FILE_WRITE);
    if (!file) return;
    for (int i = 0; i < count; i++) file.println(values[i]);
    file.close();
}

void SDLogger::saveRFList(String values[], int count) {
    File file = SD.open("/rf_signals.txt", FILE_WRITE);
    if (!file) return;
    for (int i = 0; i < count; i++) file.println(values[i]);
    file.close();
}

void SDLogger::saveNFCList(String values[], int count) {
    File file = SD.open("/nfc_tags.txt", FILE_WRITE);
    if (!file) return;
    for (int i = 0; i < count; i++) file.println(values[i]);
    file.close();
}