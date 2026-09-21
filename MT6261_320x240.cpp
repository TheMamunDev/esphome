#include "MT6261_320x240.h"
#include <SPI.h>

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// Constructor
MT6261_320x240::MT6261_320x240(int8_t cs, int8_t clk, int8_t mosi, int8_t rst)
    : Adafruit_GFX(TFT_WIDTH, TFT_HEIGHT) {
    _cs = cs;
    _clk = clk;
    _mosi = mosi;
    _rst = rst;
    rotation = 0;
}

// Initialize display
void MT6261_320x240::begin() {
    pinMode(_cs, OUTPUT);
    pinMode(_clk, OUTPUT);
    pinMode(_mosi, OUTPUT);
    pinMode(_rst, OUTPUT);
    
    digitalWrite(_cs, HIGH);
    digitalWrite(_clk, LOW);
    
    reset();
    
    // Initialization sequence for MT6261
    writeCommand(0x01);  // Software reset
    delay(150);
    writeCommand(0x11);  // Sleep out
    delay(150);
    
    writeCommandData(0x3A, 0x55);  // RGB565 color mode
    
    // Set default rotation
    setRotation(0);
    
    // Display on
    writeCommand(0x29);
    delay(100);
}

// Hardware reset
void MT6261_320x240::reset() {
    digitalWrite(_rst, LOW);
    delay(50);
    digitalWrite(_rst, HIGH);
    delay(150);
}

// Low-level SPI write
void MT6261_320x240::sdiWrite(uint8_t dc, uint8_t val) {
    digitalWrite(_cs, LOW);
    
    // 9th bit = DC (0: command, 1: data)
    digitalWrite(_mosi, dc);
    digitalWrite(_clk, HIGH);
    digitalWrite(_clk, LOW);
    
    // Send 8 data bits
    for (int i = 7; i >= 0; i--) {
        digitalWrite(_mosi, (val >> i) & 1);
        digitalWrite(_clk, HIGH);
        digitalWrite(_clk, LOW);
    }
    
    digitalWrite(_cs, HIGH);
}

void MT6261_320x240::writeCommand(uint8_t cmd) {
    sdiWrite(0, cmd);
}

void MT6261_320x240::writeData(uint8_t data) {
    sdiWrite(1, data);
}

void MT6261_320x240::writeCommandData(uint8_t cmd, uint8_t data) {
    writeCommand(cmd);
    writeData(data);
}

// Required GFX function - draw a single pixel
void MT6261_320x240::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if ((x < 0) || (x >= _width) || (y < 0) || (y >= _height))
        return;
    
    setWindow(x, y, x, y);
    pushColor(color);
}

// Fill screen with color
void MT6261_320x240::fillScreen(uint16_t color) {
    fillRect(0, 0, _width, _height, color);
}

// Optimized rectangle fill
void MT6261_320x240::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width) w = _width - x;
    if ((y + h - 1) >= _height) h = _height - y;
    
    setWindow(x, y, x + w - 1, y + h - 1);
    
    for (uint32_t i = 0; i < (uint32_t)w * h; i++) {
        pushColor(color);
    }
}

// Optimized vertical line
void MT6261_320x240::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if ((x >= _width) || (y >= _height)) return;
    if ((y + h - 1) >= _height) h = _height - y;
    
    fillRect(x, y, 1, h, color);
}

// Optimized horizontal line
void MT6261_320x240::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width) w = _width - x;
    
    fillRect(x, y, w, 1, color);
}

// Set display window for drawing - UPDATED for rotation
void MT6261_320x240::setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    writeCommand(0x2A);  // Column address set
    writeData(x0 >> 8);
    writeData(x0 & 0xFF);
    writeData(x1 >> 8);
    writeData(x1 & 0xFF);
    
    writeCommand(0x2B);  // Row address set
    writeData(y0 >> 8);
    writeData(y0 & 0xFF);
    writeData(y1 >> 8);
    writeData(y1 & 0xFF);
    
    writeCommand(0x2C);  // Memory write
}

// Push single color to display
void MT6261_320x240::pushColor(uint16_t color) {
    writeData(color >> 8);
    writeData(color & 0xFF);
}

// Push multiple colors (for images)
void MT6261_320x240::pushColors(uint16_t *colors, uint32_t len, bool first) {
    if (first) {
        digitalWrite(_cs, LOW);
        digitalWrite(_mosi, 1);  // DC = data
        digitalWrite(_clk, HIGH);
        digitalWrite(_clk, LOW);
    }
    
    for (uint32_t i = 0; i < len; i++) {
        uint16_t color = colors[i];
        for (int8_t b = 15; b >= 0; b--) {
            digitalWrite(_mosi, (color >> b) & 1);
            digitalWrite(_clk, HIGH);
            digitalWrite(_clk, LOW);
        }
    }
    
    if (first) {
        digitalWrite(_cs, HIGH);
    }
}

// Set display rotation - FIXED VERSION
void MT6261_320x240::setRotation(uint8_t r) {
    rotation = r & 3;
    
    writeCommand(0x36);  // MADCTL command
    
    switch (rotation) {
        case 0:  // Portrait (0°)
            _width = TFT_WIDTH;
            _height = TFT_HEIGHT;
            writeData(0x00);  // Try different values
            break;
        case 1:  // Landscape (90°)
            _width = TFT_HEIGHT;
            _height = TFT_WIDTH;
            writeData(0x60);  // Try 0x60, 0xE0, 0xA0, 0x20
            break;
        case 2:  // Portrait (180°)
            _width = TFT_WIDTH;
            _height = TFT_HEIGHT;
            writeData(0xC0);  // Try 0xC0, 0x80, 0x40
            break;
        case 3:  // Landscape (270°)
            _width = TFT_HEIGHT;
            _height = TFT_WIDTH;
            writeData(0xA0);  // Try 0xA0, 0x00, 0x20
            break;
    }
    
    // For some displays, need to send additional commands
    writeCommand(0x2A);  // Set column address
    writeData(0x00); writeData(0x00);
    writeData((_width-1) >> 8); writeData((_width-1) & 0xFF);
    
    writeCommand(0x2B);  // Set row address
    writeData(0x00); writeData(0x00);
    writeData((_height-1) >> 8); writeData((_height-1) & 0xFF);
}

// Display control
void MT6261_320x240::invertDisplay(bool i) {
    writeCommand(i ? 0x21 : 0x20);
}

void MT6261_320x240::displayOn() {
    writeCommand(0x29);
}

void MT6261_320x240::displayOff() {
    writeCommand(0x28);
}