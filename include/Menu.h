#pragma once
#include <stddef.h>

// Added 'Boot' state for the splash screen
enum class UiState { Boot, MainMenu, Infrared, SubGHz, NFC, WiFiScan, Settings };

struct MenuItem {
    const char* label;
    UiState destination;
};

class IMenuDisplay {
public:
    virtual ~IMenuDisplay() = default;
    virtual void drawMenu(const MenuItem* items, size_t itemCount, size_t selectedIndex) = 0;
};

class Menu {
public:
    explicit Menu(IMenuDisplay& display);
    void begin();
    void redraw();
    void update(bool buttonUp, bool buttonDown, bool buttonSelect, UiState& state);

private:
    static const MenuItem items_[5];
    static const size_t itemCount_;
    IMenuDisplay& display_;
    
    size_t selectedIndex_;
    bool wasButtonUp_, wasButtonDown_, wasButtonSelect_;
    void draw();
};