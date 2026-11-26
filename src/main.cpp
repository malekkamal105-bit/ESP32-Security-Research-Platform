#include <Arduino.h>
#include <SPI.h>
#include "Config.h"
#include "Menu.h"
#include "WiFiScanner.h"
#include "IRModule.h"
#include "SubGHzModule.h"
#include "NFCModule.h"
#include "SDLogger.h"

// --- CONDITIONAL COMPILATION ---
#if defined(WOKWI_SIMULATION)
  #include <Adafruit_ILI9341.h>
  #include <Adafruit_GFX.h>
  #include <IRremote.h>

  // --- COLORS (HACKER THEME) ---
  #define COLOR_BG          0x0000
  #define COLOR_TEXT        0x07E0
  #define COLOR_HIGHLIGHT   0x07E0 
  #define COLOR_HL_TEXT     0x0000 
  #define COLOR_WARNING     0xFFE0 
  #define COLOR_ERROR       0xF800 

  class WokwiDisplay : public IMenuDisplay {
  public:
      Adafruit_ILI9341& tft;
      WokwiDisplay(Adafruit_ILI9341& t) : tft(t) {}
      
      void begin() { 
          tft.begin(); 
          tft.setRotation(1); 
          tft.fillScreen(COLOR_BG); 
      }
      
      void drawBootScreen() {
          tft.fillScreen(COLOR_BG);
          tft.setCursor(30, 50);
          tft.setTextColor(COLOR_TEXT);
          tft.setTextSize(3);
          tft.print("SECURE");
          tft.setCursor(30, 85);
          tft.print("SCAN");
          
          tft.setTextSize(1);
          tft.setCursor(30, 130);
          tft.print("Multi-Protocol Research Tool");
          tft.setCursor(30, 145);
          tft.print("v1.0.0-mvp");

          tft.drawRect(30, 180, 180, 10, COLOR_TEXT);
          for (int i = 0; i < 176; i += 4) {
              tft.fillRect(32 + i, 182, 4, 6, COLOR_HIGHLIGHT);
              delay(40); 
          }
          
          tft.setCursor(30, 200);
          tft.setTextColor(COLOR_TEXT);
          tft.print("System Ready.");
          delay(500);
      }
      
      void drawMenu(const MenuItem* items, size_t itemCount, size_t selectedIndex) override {
          tft.fillScreen(COLOR_BG); 
          tft.setCursor(10, 10);
          tft.setTextColor(COLOR_TEXT); 
          tft.setTextSize(2);
          tft.print("MAIN MENU");
          
          for(size_t i=0; i<itemCount; i++) {
              int y = 40 + (i*30);
              if(i == selectedIndex) {
                  tft.fillRect(0, y-2, 240, 28, COLOR_HIGHLIGHT);
                  tft.setTextColor(COLOR_HL_TEXT); 
              } else {
                  tft.setTextColor(COLOR_TEXT);
              }
              tft.setCursor(10, y);
              tft.print(items[i].label);
          }
      }

      void drawWiFiList(String ssids[], int rssis[], int count, int scrollOffset) {
          tft.fillScreen(COLOR_BG);
          tft.setCursor(10, 10);
          tft.setTextColor(COLOR_TEXT);
          tft.setTextSize(2);
          tft.print("WIFI SCAN");
          tft.setTextSize(1);
          tft.setTextColor(COLOR_TEXT);
          tft.setCursor(10, 35);
          tft.print("UP/DN: Scroll | SEL: Save | BACK: Exit");

          int maxVisible = 7; 
          for(int i = 0; i < maxVisible; i++) {
              int index = i + scrollOffset;
              if (index >= count) break;
              int y = 55 + (i * 20);
              tft.setCursor(10, y);
              String ssid = ssids[index];
              if(ssid.length() > 15) ssid = ssid.substring(0, 15) + "...";
              tft.print(ssid);
              tft.setCursor(200, y);
              tft.print(rssis[index]);
          }
          tft.setCursor(10, 200);
          tft.print("Showing ");
          tft.print(scrollOffset + 1);
          tft.print("-");
          tft.print(min(scrollOffset + maxVisible, count));
          tft.print(" of ");
          tft.print(count);
      }

      void drawIRList(String values[], decode_type_t protocols[], int count, int scrollOffset, int selectedIndex) {
          tft.fillScreen(COLOR_BG);
          tft.setCursor(10, 10);
          tft.setTextColor(COLOR_TEXT); 
          tft.setTextSize(2);
          tft.print("IR HISTORY");
          tft.setTextSize(1);
          tft.setTextColor(COLOR_TEXT); 
          tft.setCursor(10, 35);
          tft.print("UP/DN: Scroll | SEL: Save | RIGHT: Send | BACK: Exit");

          if (count == 0) {
              tft.setCursor(10, 70);
              tft.setTextSize(2);
              tft.setTextColor(COLOR_WARNING); 
              tft.println("Waiting for");
              tft.setCursor(10, 90);
              tft.println("signal...");
              return;
          }

          int maxVisible = 7; 
          for(int i = 0; i < maxVisible; i++) {
              int index = i + scrollOffset;
              if (index >= count) break;
              int y = 55 + (i * 20);
              if (index == selectedIndex) {
                  tft.fillRect(0, y-2, 240, 18, COLOR_HIGHLIGHT);
                  tft.setTextColor(COLOR_HL_TEXT);
              } else {
                  tft.setTextColor(COLOR_TEXT); 
              }
              tft.setCursor(10, y);
              tft.print(getProtocolString(protocols[index]));
              tft.print(": ");
              tft.print(values[index]);
          }
          tft.setCursor(10, 200);
          tft.setTextColor(COLOR_TEXT);
          tft.print("Showing ");
          tft.print(scrollOffset + 1);
          tft.print("-");
          tft.print(min(scrollOffset + maxVisible, count));
          tft.print(" of ");
          tft.print(count);
      }

      void drawSubGHzList(String values[], int count, int scrollOffset, int selectedIndex) {
          tft.fillScreen(COLOR_BG);
          tft.setCursor(10, 10);
          tft.setTextColor(COLOR_TEXT); 
          tft.setTextSize(2);
          tft.print("SUB-GHZ");
          tft.setTextSize(1);
          tft.setTextColor(COLOR_TEXT); 
          tft.setCursor(10, 35);
          tft.print("UP/DN: Scroll | SEL: Save | RIGHT: Send | BACK: Exit");

          if (count == 0) {
              tft.setCursor(10, 70);
              tft.setTextSize(2);
              tft.setTextColor(COLOR_WARNING); 
              tft.println("Press SIM_RF");
              tft.setCursor(10, 90);
              tft.println("to capture...");
              return;
          }

          int maxVisible = 7; 
          for(int i = 0; i < maxVisible; i++) {
              int index = i + scrollOffset;
              if (index >= count) break;
              int y = 55 + (i * 20);
              if (index == selectedIndex) {
                  tft.fillRect(0, y-2, 240, 18, COLOR_HIGHLIGHT);
                  tft.setTextColor(COLOR_HL_TEXT);
              } else {
                  tft.setTextColor(COLOR_TEXT); 
              }
              tft.setCursor(10, y);
              tft.print("433MHz: ");
              tft.print(values[index]);
          }
          tft.setCursor(10, 200);
          tft.setTextColor(COLOR_TEXT);
          tft.print("Showing ");
          tft.print(scrollOffset + 1);
          tft.print("-");
          tft.print(min(scrollOffset + maxVisible, count));
          tft.print(" of ");
          tft.print(count);
      }

      void drawNFCList(String values[], int count, int scrollOffset, int selectedIndex) {
          tft.fillScreen(COLOR_BG);
          tft.setCursor(10, 10);
          tft.setTextColor(COLOR_TEXT); 
          tft.setTextSize(2);
          tft.print("NFC TAGS");

          tft.setTextSize(1);
          tft.setTextColor(COLOR_TEXT); 
          tft.setCursor(10, 35);
          tft.print("UP/DN: Scroll | SEL: Save | RIGHT: Send | BACK: Exit");

          if (count == 0) {
              tft.setCursor(10, 70);
              tft.setTextSize(2);
              tft.setTextColor(COLOR_WARNING); 
              tft.println("Press SIM_NFC");
              tft.setCursor(10, 90);
              tft.println("to scan...");
              return;
          }

          int maxVisible = 7; 
          for(int i = 0; i < maxVisible; i++) {
              int index = i + scrollOffset;
              if (index >= count) break;
              int y = 55 + (i * 20);
              
              if (index == selectedIndex) {
                  tft.fillRect(0, y-2, 240, 18, COLOR_HIGHLIGHT);
                  tft.setTextColor(COLOR_HL_TEXT);
              } else {
                  tft.setTextColor(COLOR_TEXT); 
              }

              tft.setCursor(10, y);
              tft.print("UID: ");
              tft.print(values[index]);
          }
          tft.setCursor(10, 200);
          tft.setTextColor(COLOR_TEXT);
          tft.print("Showing ");
          tft.print(scrollOffset + 1);
          tft.print("-");
          tft.print(min(scrollOffset + maxVisible, count));
          tft.print(" of ");
          tft.print(count);
      }

      void drawSettingsScreen(bool sdMounted, String macAddress) {
          tft.fillScreen(COLOR_BG);
          tft.setCursor(10, 10);
          tft.setTextColor(COLOR_TEXT);
          tft.setTextSize(2);
          tft.print("SYSTEM INFO");

          tft.setTextSize(1);
          tft.setTextColor(COLOR_TEXT);
          tft.setCursor(10, 45);
          tft.print("Firmware : v1.0.0-mvp");
          tft.setCursor(10, 65);
          tft.print("Battery  : 85% (Simulated)");
          
          tft.setCursor(10, 85);
          tft.print("SD Card  : ");
          if (sdMounted) {
              tft.setTextColor(COLOR_TEXT);
              tft.print("MOUNTED");
          } else {
              tft.setTextColor(COLOR_ERROR);
              tft.print("FAILED");
          }

          tft.setTextColor(COLOR_TEXT);
          tft.setCursor(10, 105);
          tft.print("MAC Addr : ");
          tft.print(macAddress);
          
          tft.setCursor(10, 125);
          tft.print("Free RAM : ");
          tft.print(ESP.getFreeHeap());
          tft.print(" bytes");

          tft.setCursor(10, 180);
          tft.setTextColor(COLOR_WARNING);
          tft.print("Press BACK or SEL to exit.");
      }
  };

  Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
  WokwiDisplay display(tft);
#else
  #include <Adafruit_ST7789.h>
  #include "TFTDisplay.h"
  
  Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_RST);
  TFTDisplay display(tft, 240, 320, 1);
#endif

Menu mainMenu(display);
WiFiScanner wifiScanner;
IRModule irModule; 
SubGHzModule subGHzModule;
NFCModule nfcModule;
SDLogger sdLogger;

UiState currentState = UiState::Boot; 

bool lastSelectState = false;
bool lastUpState = false;
bool lastDownState = false;
bool lastBackState = false; 
bool lastRightState = false; 

int wifiScrollOffset = 0;
int irScrollOffset = 0; 
int irSelectedIndex = 0; 
int rfScrollOffset = 0;
int rfSelectedIndex = 0;
int nfcScrollOffset = 0;
int nfcSelectedIndex = 0;

void setup() {
    Serial.begin(115200);
    delay(1000);
    
    pinMode(JOY_UP, INPUT_PULLUP);
    pinMode(JOY_DOWN, INPUT_PULLUP);
    pinMode(JOY_SEL, INPUT_PULLUP);
    pinMode(JOY_BACK, INPUT_PULLUP); 
    pinMode(JOY_RIGHT, INPUT_PULLUP); 

    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS); 
    
    display.begin();
    
    // Initialize hardware quickly behind the boot screen
    wifiScanner.begin();
    irModule.begin(); 
    subGHzModule.begin();
    nfcModule.begin();
    sdLogger.begin();
    
    // Show the boot screen animation
    display.drawBootScreen();
    
    // Transition to main menu
    currentState = UiState::MainMenu;
    mainMenu.begin();
    
    Serial.println("System Ready.");
}

void loop() {
    bool buttonUp = digitalRead(JOY_UP) == LOW;
    bool buttonDown = digitalRead(JOY_DOWN) == LOW;
    bool buttonSelect = digitalRead(JOY_SEL) == LOW;
    bool buttonBack = digitalRead(JOY_BACK) == LOW; 
    bool buttonRight = digitalRead(JOY_RIGHT) == LOW; 

    bool upEdge = (buttonUp && !lastUpState);
    bool downEdge = (buttonDown && !lastDownState);
    bool selectEdge = (buttonSelect && !lastSelectState);
    bool backEdge = (buttonBack && !lastBackState); 
    bool rightEdge = (buttonRight && !lastRightState); 

    lastUpState = buttonUp;
    lastDownState = buttonDown;
    lastSelectState = buttonSelect; 
    lastBackState = buttonBack;
    lastRightState = buttonRight; 

    if (backEdge && currentState != UiState::MainMenu && currentState != UiState::Boot) {
        currentState = UiState::MainMenu;
        mainMenu.redraw();
        delay(300); 
        return; 
    }

    if (currentState == UiState::MainMenu) {
        mainMenu.update(buttonUp, buttonDown, buttonSelect, currentState);
        
        if (currentState == UiState::WiFiScan) {
            wifiScanner.scanNetworks();
            wifiScrollOffset = 0; 
            display.drawWiFiList(wifiScanner.ssidList, wifiScanner.rssiList, wifiScanner.networkCount, wifiScrollOffset);
            delay(300); 
        }
        else if (currentState == UiState::Infrared) {
            irModule.reset(); 
            irScrollOffset = 0;
            irSelectedIndex = 0;
            display.drawIRList(irModule.valueList, irModule.protocolList, irModule.irCount, irScrollOffset, irSelectedIndex); 
            delay(300); 
        }
        else if (currentState == UiState::SubGHz) {
            subGHzModule.reset();
            rfScrollOffset = 0;
            rfSelectedIndex = 0;
            display.drawSubGHzList(subGHzModule.valueList, subGHzModule.rfCount, rfScrollOffset, rfSelectedIndex);
            delay(300);
        }
        else if (currentState == UiState::NFC) {
            nfcModule.reset();
            nfcScrollOffset = 0;
            nfcSelectedIndex = 0;
            display.drawNFCList(nfcModule.valueList, nfcModule.nfcCount, nfcScrollOffset, nfcSelectedIndex);
            delay(300);
        }
        else if (currentState == UiState::Settings) {
            String mac = WiFi.macAddress();
            display.drawSettingsScreen(sdLogger.isMounted, mac);
            delay(300);
        }
    } 
    else if (currentState == UiState::WiFiScan) {
        if (downEdge && wifiScrollOffset + 7 < wifiScanner.networkCount) {
            wifiScrollOffset++; 
            display.drawWiFiList(wifiScanner.ssidList, wifiScanner.rssiList, wifiScanner.networkCount, wifiScrollOffset);
        }
        if (upEdge && wifiScrollOffset > 0) {
            wifiScrollOffset--; 
            display.drawWiFiList(wifiScanner.ssidList, wifiScanner.rssiList, wifiScanner.networkCount, wifiScrollOffset);
        }
        if (selectEdge) {
            sdLogger.saveWiFiList(wifiScanner.ssidList, wifiScanner.rssiList, wifiScanner.networkCount);
            delay(300); 
        }
    }
    else if (currentState == UiState::Infrared) {
        irModule.capture();
        
        if (irModule.newSignal) {
            if (irModule.irCount > 7) irScrollOffset = irModule.irCount - 7;
            irSelectedIndex = irModule.irCount - 1; 
            display.drawIRList(irModule.valueList, irModule.protocolList, irModule.irCount, irScrollOffset, irSelectedIndex);
            irModule.clearSignal(); 
        }
        if (upEdge && irSelectedIndex > 0) {
            irSelectedIndex--;
            if (irSelectedIndex < irScrollOffset) irScrollOffset--;
            display.drawIRList(irModule.valueList, irModule.protocolList, irModule.irCount, irScrollOffset, irSelectedIndex);
        }
        if (downEdge && irSelectedIndex < irModule.irCount - 1) {
            irSelectedIndex++;
            if (irSelectedIndex >= irScrollOffset + 7) irScrollOffset++;
            display.drawIRList(irModule.valueList, irModule.protocolList, irModule.irCount, irScrollOffset, irSelectedIndex);
        }
        if (selectEdge && irModule.irCount > 0) {
            sdLogger.saveIRList(irModule.valueList, irModule.irCount);
            delay(300);
        }
        if (rightEdge && irModule.irCount > 0) {
            irModule.transmit(irSelectedIndex);
            delay(300);
        }
    }
    else if (currentState == UiState::SubGHz) {
        subGHzModule.capture();
        
        if (subGHzModule.newSignal) {
            if (subGHzModule.rfCount > 7) rfScrollOffset = subGHzModule.rfCount - 7;
            rfSelectedIndex = subGHzModule.rfCount - 1; 
            display.drawSubGHzList(subGHzModule.valueList, subGHzModule.rfCount, rfScrollOffset, rfSelectedIndex);
            subGHzModule.clearSignal(); 
        }
        if (upEdge && rfSelectedIndex > 0) {
            rfSelectedIndex--;
            if (rfSelectedIndex < rfScrollOffset) rfScrollOffset--;
            display.drawSubGHzList(subGHzModule.valueList, subGHzModule.rfCount, rfScrollOffset, rfSelectedIndex);
        }
        if (downEdge && rfSelectedIndex < subGHzModule.rfCount - 1) {
            rfSelectedIndex++;
            if (rfSelectedIndex >= rfScrollOffset + 7) rfScrollOffset++;
            display.drawSubGHzList(subGHzModule.valueList, subGHzModule.rfCount, rfScrollOffset, rfSelectedIndex);
        }
        if (selectEdge && subGHzModule.rfCount > 0) {
            sdLogger.saveRFList(subGHzModule.valueList, subGHzModule.rfCount);
            delay(300);
        }
        if (rightEdge && subGHzModule.rfCount > 0) {
            subGHzModule.transmit(rfSelectedIndex);
            delay(300);
        }
    }
    else if (currentState == UiState::NFC) {
        nfcModule.capture();
        
        if (nfcModule.newSignal) {
            if (nfcModule.nfcCount > 7) nfcScrollOffset = nfcModule.nfcCount - 7;
            nfcSelectedIndex = nfcModule.nfcCount - 1; 
            display.drawNFCList(nfcModule.valueList, nfcModule.nfcCount, nfcScrollOffset, nfcSelectedIndex);
            nfcModule.clearSignal(); 
        }
        if (upEdge && nfcSelectedIndex > 0) {
            nfcSelectedIndex--;
            if (nfcSelectedIndex < nfcScrollOffset) nfcScrollOffset--;
            display.drawNFCList(nfcModule.valueList, nfcModule.nfcCount, nfcScrollOffset, nfcSelectedIndex);
        }
        if (downEdge && nfcSelectedIndex < nfcModule.nfcCount - 1) {
            nfcSelectedIndex++;
            if (nfcSelectedIndex >= nfcScrollOffset + 7) nfcScrollOffset++;
            display.drawNFCList(nfcModule.valueList, nfcModule.nfcCount, nfcScrollOffset, nfcSelectedIndex);
        }
        if (selectEdge && nfcModule.nfcCount > 0) {
            sdLogger.saveNFCList(nfcModule.valueList, nfcModule.nfcCount);
            delay(300);
        }
        if (rightEdge && nfcModule.nfcCount > 0) {
            nfcModule.transmit(nfcSelectedIndex);
            delay(300);
        }
    }
    else if (currentState == UiState::Settings) {
        if (selectEdge) {
            currentState = UiState::MainMenu;
            mainMenu.redraw();
            delay(300);
        }
    }

    delay(50); 
}