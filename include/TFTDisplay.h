#pragma once
#include <Adafruit_ST7789.h>
#include "Menu.h"
#include <IRremote.h> 

class TFTDisplay : public IMenuDisplay {
public:
    TFTDisplay(Adafruit_ST7789& tft, uint16_t screenWidth, uint16_t screenHeight, uint8_t rotation = 0);
    
    void begin();
    void drawBootScreen();
    void drawMenu(const MenuItem* items, size_t itemCount, size_t selectedIndex) override;
    void drawWiFiList(String ssids[], int rssis[], int count, int scrollOffset);
    void drawIRList(String values[], decode_type_t protocols[], int count, int scrollOffset, int selectedIndex);
    void drawSubGHzList(String values[], int count, int scrollOffset, int selectedIndex);
    void drawNFCList(String values[], int count, int scrollOffset, int selectedIndex);
    void drawSettingsScreen(bool sdMounted, String macAddress);

private:
    Adafruit_ST7789& tft_;
    uint16_t screenWidth_;
    uint16_t screenHeight_;
    uint8_t rotation_;
};