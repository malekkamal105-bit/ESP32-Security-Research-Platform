#include "TFTDisplay.h"
#include <Adafruit_GFX.h>

// --- COLORS ---
#define COLOR_BG          0x0000
#define COLOR_TEXT        0x07E0
#define COLOR_HIGHLIGHT   0x07E0 
#define COLOR_HL_TEXT     0x0000 
#define COLOR_WARNING     0xFFE0 
#define COLOR_ERROR       0xF800 

TFTDisplay::TFTDisplay(Adafruit_ST7789& tft, uint16_t screenWidth, uint16_t screenHeight, uint8_t rotation)
    : tft_(tft), screenWidth_(screenWidth), screenHeight_(screenHeight), rotation_(rotation) {}

void TFTDisplay::begin() {
    tft_.init(screenWidth_, screenHeight_);
    tft_.setRotation(rotation_);
    tft_.fillScreen(COLOR_BG); 
}

void TFTDisplay::drawBootScreen() {
    tft_.fillScreen(COLOR_BG);
    tft_.setCursor(30, 50);
    tft_.setTextColor(COLOR_TEXT);
    tft_.setTextSize(3);
    tft_.print("SECURE");
    tft_.setCursor(30, 85);
    tft_.print("SCAN");
    
    tft_.setTextSize(1);
    tft_.setCursor(30, 130);
    tft_.print("Multi-Protocol Research Tool");
    tft_.setCursor(30, 145);
    tft_.print("v1.0.0-mvp");

    tft_.drawRect(30, 180, 180, 10, COLOR_TEXT);
    for (int i = 0; i < 176; i += 4) {
        tft_.fillRect(32 + i, 182, 4, 6, COLOR_HIGHLIGHT);
        delay(40); 
    }
    
    tft_.setCursor(30, 200);
    tft_.setTextColor(COLOR_TEXT);
    tft_.print("System Ready.");
    delay(500);
}

void TFTDisplay::drawMenu(const MenuItem* items, size_t itemCount, size_t selectedIndex) {
    tft_.fillScreen(COLOR_BG); 
    tft_.setCursor(10, 10);
    tft_.setTextColor(COLOR_TEXT); 
    tft_.setTextSize(2);
    tft_.print("MAIN MENU");
    
    for(size_t i=0; i<itemCount; i++) {
        int y = 40 + (i*30);
        if(i == selectedIndex) {
            tft_.fillRect(0, y-2, 240, 28, COLOR_HIGHLIGHT);
            tft_.setTextColor(COLOR_HL_TEXT); 
        } else {
            tft_.setTextColor(COLOR_TEXT);
        }
        tft_.setCursor(10, y);
        tft_.print(items[i].label);
    }
}

void TFTDisplay::drawWiFiList(String ssids[], int rssis[], int count, int scrollOffset) {
    tft_.fillScreen(COLOR_BG);
    tft_.setCursor(10, 10);
    tft_.setTextColor(COLOR_TEXT);
    tft_.setTextSize(2);
    tft_.print("WIFI SCAN");
    tft_.setTextSize(1);
    tft_.setTextColor(COLOR_TEXT);
    tft_.setCursor(10, 35);
    tft_.print("UP/DN: Scroll | SEL: Save | BACK: Exit");

    int maxVisible = 7; 
    for(int i = 0; i < maxVisible; i++) {
        int index = i + scrollOffset;
        if (index >= count) break;
        int y = 55 + (i * 20);
        tft_.setCursor(10, y);
        String ssid = ssids[index];
        if(ssid.length() > 15) ssid = ssid.substring(0, 15) + "...";
        tft_.print(ssid);
        tft_.setCursor(200, y);
        tft_.print(rssis[index]);
    }
    tft_.setCursor(10, 200);
    tft_.print("Showing ");
    tft_.print(scrollOffset + 1);
    tft_.print("-");
    tft_.print(min(scrollOffset + maxVisible, count));
    tft_.print(" of ");
    tft_.print(count);
}

void TFTDisplay::drawIRList(String values[], decode_type_t protocols[], int count, int scrollOffset, int selectedIndex) {
    tft_.fillScreen(COLOR_BG);
    tft_.setCursor(10, 10);
    tft_.setTextColor(COLOR_TEXT); 
    tft_.setTextSize(2);
    tft_.print("IR HISTORY");
    tft_.setTextSize(1);
    tft_.setTextColor(COLOR_TEXT); 
    tft_.setCursor(10, 35);
    tft_.print("UP/DN: Scroll | SEL: Save | RIGHT: Send | BACK: Exit");

    if (count == 0) {
        tft_.setCursor(10, 70);
        tft_.setTextSize(2);
        tft_.setTextColor(COLOR_WARNING); 
        tft_.println("Waiting for");
        tft_.setCursor(10, 90);
        tft_.println("signal...");
        return;
    }

    int maxVisible = 7; 
    for(int i = 0; i < maxVisible; i++) {
        int index = i + scrollOffset;
        if (index >= count) break;
        int y = 55 + (i * 20);
        if (index == selectedIndex) {
            tft_.fillRect(0, y-2, 240, 18, COLOR_HIGHLIGHT);
            tft_.setTextColor(COLOR_HL_TEXT);
        } else {
            tft_.setTextColor(COLOR_TEXT); 
        }
        tft_.setCursor(10, y);
        tft_.print(getProtocolString(protocols[index]));
        tft_.print(": ");
        tft_.print(values[index]);
    }
    tft_.setCursor(10, 200);
    tft_.setTextColor(COLOR_TEXT);
    tft_.print("Showing ");
    tft_.print(scrollOffset + 1);
    tft_.print("-");
    tft_.print(min(scrollOffset + maxVisible, count));
    tft_.print(" of ");
    tft_.print(count);
}

void TFTDisplay::drawSubGHzList(String values[], int count, int scrollOffset, int selectedIndex) {
    tft_.fillScreen(COLOR_BG);
    tft_.setCursor(10, 10);
    tft_.setTextColor(COLOR_TEXT); 
    tft_.setTextSize(2);
    tft_.print("SUB-GHZ");
    tft_.setTextSize(1);
    tft_.setTextColor(COLOR_TEXT); 
    tft_.setCursor(10, 35);
    tft_.print("UP/DN: Scroll | SEL: Save | RIGHT: Send | BACK: Exit");

    if (count == 0) {
        tft_.setCursor(10, 70);
        tft_.setTextSize(2);
        tft_.setTextColor(COLOR_WARNING); 
        tft_.println("Press SIM_RF");
        tft_.setCursor(10, 90);
        tft_.println("to capture...");
        return;
    }

    int maxVisible = 7; 
    for(int i = 0; i < maxVisible; i++) {
        int index = i + scrollOffset;
        if (index >= count) break;
        int y = 55 + (i * 20);
        if (index == selectedIndex) {
            tft_.fillRect(0, y-2, 240, 18, COLOR_HIGHLIGHT);
            tft_.setTextColor(COLOR_HL_TEXT);
        } else {
            tft_.setTextColor(COLOR_TEXT); 
        }
        tft_.setCursor(10, y);
        tft_.print("433MHz: ");
        tft_.print(values[index]);
    }
    tft_.setCursor(10, 200);
    tft_.setTextColor(COLOR_TEXT);
    tft_.print("Showing ");
    tft_.print(scrollOffset + 1);
    tft_.print("-");
    tft_.print(min(scrollOffset + maxVisible, count));
    tft_.print(" of ");
    tft_.print(count);
}

void TFTDisplay::drawNFCList(String values[], int count, int scrollOffset, int selectedIndex) {
    tft_.fillScreen(COLOR_BG);
    tft_.setCursor(10, 10);
    tft_.setTextColor(COLOR_TEXT); 
    tft_.setTextSize(2);
    tft_.print("NFC TAGS");

    tft_.setTextSize(1);
    tft_.setTextColor(COLOR_TEXT); 
    tft_.setCursor(10, 35);
    tft_.print("UP/DN: Scroll | SEL: Save | RIGHT: Send | BACK: Exit");

    if (count == 0) {
        tft_.setCursor(10, 70);
        tft_.setTextSize(2);
        tft_.setTextColor(COLOR_WARNING); 
        tft_.println("Press SIM_NFC");
        tft_.setCursor(10, 90);
        tft_.println("to scan...");
        return;
    }

    int maxVisible = 7; 
    for(int i = 0; i < maxVisible; i++) {
        int index = i + scrollOffset;
        if (index >= count) break;
        int y = 55 + (i * 20);
        if (index == selectedIndex) {
            tft_.fillRect(0, y-2, 240, 18, COLOR_HIGHLIGHT);
            tft_.setTextColor(COLOR_HL_TEXT);
        } else {
            tft_.setTextColor(COLOR_TEXT); 
        }
        tft_.setCursor(10, y);
        tft_.print("UID: ");
        tft_.print(values[index]);
    }
    tft_.setCursor(10, 200);
    tft_.setTextColor(COLOR_TEXT);
    tft_.print("Showing ");
    tft_.print(scrollOffset + 1);
    tft_.print("-");
    tft_.print(min(scrollOffset + maxVisible, count));
    tft_.print(" of ");
    tft_.print(count);
}

void TFTDisplay::drawSettingsScreen(bool sdMounted, String macAddress) {
    tft_.fillScreen(COLOR_BG);
    tft_.setCursor(10, 10);
    tft_.setTextColor(COLOR_TEXT);
    tft_.setTextSize(2);
    tft_.print("SYSTEM INFO");

    tft_.setTextSize(1);
    tft_.setTextColor(COLOR_TEXT);
    tft_.setCursor(10, 45);
    tft_.print("Firmware : v1.0.0-mvp");
    tft_.setCursor(10, 65);
    tft_.print("Battery  : 85% (Simulated)");
    
    tft_.setCursor(10, 85);
    tft_.print("SD Card  : ");
    if (sdMounted) {
        tft_.setTextColor(COLOR_TEXT);
        tft_.print("MOUNTED");
    } else {
        tft_.setTextColor(COLOR_ERROR);
        tft_.print("FAILED");
    }

    tft_.setTextColor(COLOR_TEXT);
    tft_.setCursor(10, 105);
    tft_.print("MAC Addr : ");
    tft_.print(macAddress);
    
    tft_.setCursor(10, 125);
    tft_.print("Free RAM : ");
    tft_.print(ESP.getFreeHeap());
    tft_.print(" bytes");

    tft_.setCursor(10, 180);
    tft_.setTextColor(COLOR_WARNING);
    tft_.print("Press BACK or SEL to exit.");
}