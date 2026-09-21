#ifndef _MT6261_320X240_H
#define _MT6261_320X240_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

// Color definitions (RGB565 format)
#define MT6261_BLACK       0x0000
#define MT6261_NAVY        0x000F
#define MT6261_DARKGREEN   0x03E0
#define MT6261_DARKCYAN    0x03EF
#define MT6261_MAROON      0x7800
#define MT6261_PURPLE      0x780F
#define MT6261_OLIVE       0x7BE0
#define MT6261_LIGHTGREY   0xC618
#define MT6261_DARKGREY    0x7BEF
#define MT6261_BLUE        0x001F
#define MT6261_GREEN       0x07E0
#define MT6261_CYAN        0x07FF
#define MT6261_RED         0xF800
#define MT6261_MAGENTA     0xF81F
#define MT6261_YELLOW      0xFFE0
#define MT6261_WHITE       0xFFFF
#define MT6261_ORANGE      0xFD20
#define MT6261_GREENYELLOW 0xAFE5
#define MT6261_PINK        0xF81F

class MT6261_320x240 : public Adafruit_GFX {
public:
    MT6261_320x240(int8_t cs, int8_t clk, int8_t mosi, int8_t rst);
    
    void begin();
    void reset();
    
    // GFX required functions
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillScreen(uint16_t color);
    
    // Optimized functions
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    
    // Display control
    void setRotation(uint8_t r);
    void invertDisplay(bool i);
    void displayOn();
    void displayOff();
    
    // Custom functions
    void setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void pushColor(uint16_t color);
    void pushColors(uint16_t *colors, uint32_t len, bool first = true);
    
    // Color helper function
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }
    
private:
    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
    void writeCommandData(uint8_t cmd, uint8_t data);
    void sdiWrite(uint8_t dc, uint8_t val);
    
    int8_t _cs, _clk, _mosi, _rst;
    uint8_t rotation;
};

#endif