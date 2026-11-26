# 🛡️ Portable Multi-Protocol Security Research Platform

![C++](https://img.shields.io/badge/Language-C++-blue.svg)
![Platform](https://img.shields.io/badge/Platform-ESP32--S3-red.svg)
![Framework](https://img.shields.io/badge/Framework-Arduino-teal.svg)
![Status](https://img.shields.io/badge/Status-Software_Complete-success.svg)

## 📖 Overview
This project is a fully functional, modular software platform for a portable security research tool, inspired by devices like the Flipper Zero. Built around the ESP32-S3 microcontroller, it provides an affordable, open-source environment for analyzing and interacting with various wireless protocols.

Due to hardware procurement delays, this project was engineered as a **Software-First** application. The entire C++ codebase was developed, tested, and verified using a web-based ESP32 simulator (Velxio). The software utilizes Conditional Compilation, meaning it is 100% hardware-ready and will automatically compile for physical components (ST7789 screen, CC1101, PN532) when deployed to real hardware.

> ⚠️ **Legal Disclaimer:** This tool is strictly for **authorized security research and education only**. It is designed to be passive and does not include active attack vectors (no jamming, no deauthentication, no exploitation). Usage must comply with the Turkish Penal Code (TCK 243, 244, 245/A).

## ✨ Key Features

*   **🖥️ Custom UI State Machine:** A responsive, non-blocking menu system with a "Hacker" dark theme (Neon Green/Black).
*   **📡 Wi-Fi Scanner:** Passively scans for nearby networks, displays SSID/RSSI, and logs results to a `.csv` file.
*   **🔴 IR Module:** Captures, filters duplicates, and replays Infrared signals (NEC, Sony, etc.) using the `IRremote` library.
*   **📻 Sub-GHz Module (Simulated):** Mimics capturing and transmitting 433MHz signals, ready to be swapped with CC1101 drivers.
*   **🏷️ NFC Module (Simulated):** Mimics reading and emulating NFC tag UIDs, ready to be swapped with PN532 drivers.
*   **💾 Safe SD Card Logging:** Saves all captured data to TXT/CSV files without freezing the device if the SD card is missing.
*   **⚙️ System Info:** Displays real-time MAC address, Free RAM, and SD card status.

## 🏗️ Software Architecture
The codebase is built using **Object-Oriented Programming (OOP)** and follows a strict 4-Layer Architecture:
1.  **HAL (Hardware Abstraction Layer):** SPI, GPIO, PWM.
2.  **Drivers:** Screen rendering (`TFTDisplay`), SD Card mounting (`SDLogger`).
3.  **Services:** Logic for scanning (`WiFiScanner`), capturing (`IRModule`), etc.
4.  **Apps/UI:** The main State Machine (`Menu`), Boot Screen, and User Input handling.

## 🛠️ Simulation Instructions (Velxio)

You can run this project instantly in your browser without any hardware.

1. Go to [Velxio.dev](https://velxio.dev) and create a new ESP32-S3 project.
2. **Copy the Code:** Copy the combined `sketch.ino` file provided in this repository and paste it into the Velxio editor.
3. **Wiring:** Set up the virtual components as follows:
   *   **TFT Screen (ILI9341):** CS=10, DC=9, RST=8, SCK=12, MOSI=11.
   *   **Buttons:** UP=4, DOWN=5, SELECT=6, BACK=3, RIGHT=2, SIM_RF=42, SIM_NFC=41.
   *   **IR Receiver:** Connect to GPIO 7.
   *   **SD Card:** CS=13, MISO=14, MOSI=11, SCK=12.
4. Hit **Run** and interact with the simulation!

## 💻 Real Hardware Deployment
To move from the simulator to a physical device:
1. Open `platformio.ini` and **remove/comment** the line `build_flags = -D WOKWI_SIMULATION`.
2. Wire the physical components according to `include/Config.h`.
3. Click **Upload** in PlatformIO. The code will automatically switch to the physical ST7789 display driver and real CC1101/PN532 libraries.

## 📂 Project Structure
```text
├── include/            # Header files (.h)
│   ├── Config.h        # Pin definitions
│   ├── Menu.h          # UI State Machine blueprint
│   ├── IRModule.h      # IR driver blueprint
│   ├── NFCModule.h     # NFC driver blueprint
│   ├── SubGHzModule.h  # RF driver blueprint
│   ├── WiFiScanner.h   # Wi-Fi blueprint
│   └── SDLogger.h      # SD Card blueprint
├── src/                # Source files (.cpp)
│   ├── main.cpp        # Master coordinator & conditional compilation
│   ├── Menu.cpp
│   ├── IRModule.cpp
│   ├── NFCModule.cpp
│   ├── SubGHzModule.cpp
│   ├── WiFiScanner.cpp
│   ├── SDLogger.cpp
│   └── TFTDisplay.cpp  # Hardware screen implementation
├── platformio.ini      # Build configuration
└── README.md

## 👨‍💻 Author
**Malek Kamal** 
Computer Engineering Student
*Graduation Project - 2026-2027*

## 📚 References & Libraries
*   [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
*   [Adafruit GFX & ST7789](https://github.com/adafruit/Adafruit-ST7735-Library)
*   [Arduino-IRremote](https://github.com/Arduino-IRremote/Arduino-IRremote)
