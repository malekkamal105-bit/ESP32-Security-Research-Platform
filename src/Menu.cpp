#include "Menu.h"

const MenuItem Menu::items_[5] = {
    {"IR", UiState::Infrared},
    {"Sub-GHz", UiState::SubGHz},
    {"NFC", UiState::NFC},
    {"Wi-Fi Scan", UiState::WiFiScan},
    {"Settings", UiState::Settings}
};
const size_t Menu::itemCount_ = sizeof(Menu::items_) / sizeof(Menu::items_[0]);

Menu::Menu(IMenuDisplay& display)
    : display_(display), selectedIndex_(0), wasButtonUp_(false), wasButtonDown_(false), wasButtonSelect_(false) {}

void Menu::begin() {
    selectedIndex_ = 0;
    wasButtonUp_ = wasButtonDown_ = wasButtonSelect_ = false;
    draw();
}

void Menu::redraw() { draw(); }

void Menu::update(bool buttonUp, bool buttonDown, bool buttonSelect, UiState& state) {
    const bool upPressed = buttonUp && !wasButtonUp_;
    const bool downPressed = buttonDown && !wasButtonDown_;
    const bool selectPressed = buttonSelect && !wasButtonSelect_;

    wasButtonUp_ = buttonUp;
    wasButtonDown_ = buttonDown;
    wasButtonSelect_ = buttonSelect;

    if (state != UiState::MainMenu) return;

    bool selectionChanged = false;

    if (upPressed) {
        selectedIndex_ = (selectedIndex_ == 0) ? itemCount_ - 1 : selectedIndex_ - 1;
        selectionChanged = true;
    } else if (downPressed) {
        selectedIndex_ = (selectedIndex_ + 1) % itemCount_;
        selectionChanged = true;
    }

    if (selectionChanged) draw();
    if (selectPressed) state = items_[selectedIndex_].destination;
}

void Menu::draw() { display_.drawMenu(items_, itemCount_, selectedIndex_); }