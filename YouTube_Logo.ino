/*
 * MT6261 320x240 Display Demo
 * Created by Ilyas Rao from CHAND MOBILES
 * 
 * Contact Information:
 * Facebook: https://web.facebook.com/raoilyas.chand
 * YouTube: https://www.youtube.com/@ilyasrao
 * 
 * This demo displays a rainbow animation, YouTube logo, and channel info.
 * Display Rotation: 1 (Landscape 90°)
 */

#include <Adafruit_GFX.h>
#include "MT6261_320x240.h"
#include <Fonts/FreeSerifBoldItalic18pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>  // For smaller text

#define TFT_CS   22
#define TFT_CLK  18
#define TFT_MOSI 23
#define TFT_RST  4

MT6261_320x240 tft = MT6261_320x240(TFT_CS, TFT_CLK, TFT_MOSI, TFT_RST);

// Rainbow colors
const uint16_t rainbowColors[] = {
  MT6261_RED, MT6261_ORANGE, MT6261_YELLOW, MT6261_GREEN,
  MT6261_CYAN, MT6261_BLUE, MT6261_PURPLE, MT6261_MAGENTA
};

// YouTube colors
#define YOUTUBE_RED   0xF800  // RGB565 Red
#define YOUTUBE_WHITE 0xFFFF  // RGB565 White
#define YOUTUBE_BLACK 0x0000  // RGB565 Black

void setup() {
  Serial.begin(115200);
  Serial.println("\nStarting CHAND MOBILES YouTube Channel Display");
  
  tft.begin();
  tft.setRotation(1);  // Landscape mode (90°)
  
  // Step 1: Rainbow Animation
  rainbowAnimation();
  delay(500);
  
  // Step 2: Clear and show YouTube logo
  tft.fillScreen(MT6261_BLACK);
  drawYouTubeLogo();
  delay(2000);
  tft.fillScreen(MT6261_BLACK);

  // Step 3: Show channel text
  showChannelInfo();
  
  Serial.println("Display setup complete!");
}

void loop() {
  // Optional: Add a subtle animation or update
  static unsigned long lastUpdate = 0;
  static uint8_t pulse = 0;
  static bool increasing = true;
  
  if (millis() - lastUpdate > 50) {
    lastUpdate = millis();
    
    // Pulse effect on "Search My Channel" text
    if (increasing) {
      pulse++;
      if (pulse > 20) increasing = false;
    } else {
      pulse--;
      if (pulse < 5) increasing = true;
    }
    
    // Redraw "Search My Channel" with pulse effect
    tft.setTextColor(tft.color565(255, pulse*10, pulse*5), MT6261_BLACK);
    tft.setFont(&FreeSerifBoldItalic18pt7b);
    tft.setCursor(20, 50);
    tft.print("Search My Channel");
  }
}

void rainbowAnimation() {
  Serial.println("Playing rainbow animation...");
  
  
  // Rainbow bars from center
  tft.fillScreen(MT6261_BLACK);
  int centerX = 160;
  int centerY = 120;
  
  for (int r = 0; r < 120; r += 5) {
    for (int i = 0; i < 8; i++) {
      float angle = (i * 45) * 3.14159 / 180.0;
      int x = centerX + r * cos(angle);
      int y = centerY + r * sin(angle);
      tft.fillCircle(x, y, 10, rainbowColors[i]);
    }
    delay(30);
  }
  
  // Expanding circles
  for (int r = 10; r < 200; r += 20) {
    for (int i = 0; i < 8; i++) {
      uint16_t color = rainbowColors[i % 8];
      tft.drawCircle(centerX, centerY, r + i*2, color);
    }
    delay(50);
  }
  
  delay(300);
}

void drawYouTubeLogo() {
  Serial.println("Drawing YouTube logo...");
  
  // Draw YouTube play button background (red)
  int centerX = 160;
  int centerY = 100;
  int logoSize = 80;
  
  // Red rounded rectangle background
  tft.fillRoundRect(centerX - logoSize/2, centerY - logoSize/2, 
                    logoSize, logoSize, 15, YOUTUBE_RED);
  
  // White play triangle
  int triangleSize = 40;
  int x1 = centerX - triangleSize/3;
  int y1 = centerY - triangleSize/2;
  int x2 = centerX - triangleSize/3;
  int y2 = centerY + triangleSize/2;
  int x3 = centerX + triangleSize/2;
  int y3 = centerY;
  
  // Draw filled triangle
  tft.fillTriangle(x1, y1, x2, y2, x3, y3, YOUTUBE_WHITE);
  
  // Add "YouTube" text below
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextColor(YOUTUBE_WHITE);
  tft.setCursor(110, 180);
  tft.print("YouTube");
  
  // Add glow effect
  for (int i = 1; i <= 3; i++) {
    tft.drawRoundRect(centerX - logoSize/2 - i, centerY - logoSize/2 - i, 
                      logoSize + 2*i, logoSize + 2*i, 15 + i, 
                      tft.color565(100, 0, 0));
  }
}

void showChannelInfo() {
  Serial.println("Displaying channel information...");
  
  // Clear area for text
  tft.fillRect(0, 200, 320, 40, MT6261_BLACK);
  
  // Set font for main text
  tft.setFont(&FreeSerifBoldItalic18pt7b);
  
  // "Search My Channel" - with gradient effect
  for (int i = 0; i < 5; i++) {
    uint16_t color = tft.color565(255, 150 - i*30, 50 + i*20);
    tft.setTextColor(color, MT6261_BLACK);
    tft.setCursor(20 - i, 50 - i);
    tft.print("Search My Channel");
  }
  
  // Main white version
  tft.setTextColor(MT6261_WHITE, MT6261_BLACK);
  tft.setCursor(20, 50);
  tft.print("Search My Channel");
  
  delay(1000);
  
  // "CHAND MOBILES" - larger and bold
  tft.setFont(&FreeSerifBoldItalic18pt7b);
  tft.setTextColor(MT6261_WHITE, MT6261_BLACK);
  
  // Shadow effect
  //tft.setTextColor(MT6261_WHITE, MT6261_BLACK);
  //tft.setCursor(21, 101);
  //tft.print("CHAND MOBILES");
  
  // Main text
  tft.setTextColor(MT6261_WHITE, MT6261_BLACK);
  tft.setCursor(20, 100);
  tft.print("CHAND MOBILES");
  
  delay(1000);
  
  // "@ilyasrao" - with YouTube style
  tft.setFont(&FreeSansBold12pt7b);
  
  // Draw YouTube-style background
  //tft.fillRoundRect(110, 150, 100, 30, 5, YOUTUBE_RED);
  tft.fillRoundRect(112, 152, 96, 26, 3, MT6261_BLACK);
  
  // Draw @ symbol with YouTube red
  //tft.setTextColor(YOUTUBE_WHITE);
  //tft.setCursor(110, 172);
  //tft.print("@");
  
  // Draw username in white
  tft.setTextColor(YOUTUBE_WHITE);
  tft.setCursor(100, 152);
  tft.print("@ilyasrao");
  
  // Add subscribe button style
  //tft.drawRoundRect(108, 148, 104, 34, 7, YOUTUBE_WHITE);
  
  // Add social media icons (simplified)
  drawSocialIcons();
  
  // Draw decorative elements
  drawDecorations();
}

void drawSocialIcons() {
  // Facebook icon (simplified 'f')
  tft.fillRoundRect(20, 200, 30, 30, 5, 0x001F); // Facebook blue
  tft.setTextColor(MT6261_WHITE);
  tft.setFont();
  tft.setTextSize(2);
  tft.setCursor(28, 206);
  tft.print("f");
  
  // YouTube icon (simplified 'YT')
  tft.fillRoundRect(60, 200, 30, 30, 5, YOUTUBE_RED);
  tft.setTextColor(MT6261_WHITE);
  tft.setCursor(66, 206);
  tft.print("YT");
  
  // Website icon (simplified 'W')
  tft.fillRoundRect(100, 200, 30, 30, 5, MT6261_GREEN);
  tft.setTextColor(MT6261_WHITE);
  tft.setCursor(106, 206);
  tft.print("W");
  
  // Reset font for other text
  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextSize(1);
}

void drawDecorations() {
  // Draw borders
  tft.drawRect(0, 0, 319, 239, MT6261_BLUE);
  tft.drawRect(1, 1, 317, 237, MT6261_CYAN);
  
  // Draw corner accents
  //for (int i = 0; i < 4; i++) {
    //int x = (i % 2) * 300;
    //int y = (i / 2) * 220;
    //tft.fillTriangle(x, y, x+20, y, x, y+20, rainbowColors[i*2]);
  }
  
  // Add pulse dots at bottom
  //for (int i = 0; i < 8; i++) {
    //int x = 40 + i * 30;
    //drawPulsingDot(x, 230, rainbowColors[i]);
  //}
//}

void drawPulsingDot(int x, int y, uint16_t baseColor) {
  static uint8_t pulsePhase[8] = {0};
  static bool initialized = false;
  
  if (!initialized) {
    for (int i = 0; i < 8; i++) {
      pulsePhase[i] = i * 32;
    }
    initialized = true;
  }
  
  for (int i = 0; i < 8; i++) {
    uint8_t phase = (pulsePhase[i] + millis()/50) % 256;
    float brightness = (sin(phase * 3.14159 / 128.0) + 1.0) / 2.0;
    
    uint8_t r = ((baseColor >> 11) & 0x1F) * brightness;
    uint8_t g = ((baseColor >> 5) & 0x3F) * brightness;
    uint8_t b = (baseColor & 0x1F) * brightness;
    
    uint16_t pulseColor = ((r & 0x1F) << 11) | ((g & 0x3F) << 5) | (b & 0x1F);
    tft.fillCircle(40 + i * 30, 230, 3 + (brightness * 2), pulseColor);
  }
}

// Utility function to extract RGB components
void extractRGB(uint16_t color, uint8_t &r, uint8_t &g, uint8_t &b) {
  r = (color >> 11) & 0x1F;
  g = (color >> 5) & 0x3F;
  b = color & 0x1F;
  // Scale from 5/6/5 bit to 8 bit
  r = (r * 255) / 31;
  g = (g * 255) / 63;
  b = (b * 255) / 31;
}