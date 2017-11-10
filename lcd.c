#include "lcd.h"

struct colour palette[4] = {
    {255, 255, 255},
    {192, 192, 192},
    {96, 96, 96},
    {0, 0, 0}
};

struct LCD LCD;
struct LCDC LCDC;
struct LCDS LCDS;

int bgPalette[4];
int spritePalette1[4];
int spritePalette2[4];

void setLCDC(unsigned char address) {
    LCDC.lcdDisplay = ((address & 0x80) >> 7);
    LCDC.windowTileMap = ((address & 0x40) >> 6);
    LCDC.windowDisplay = ((address & 0x20) >> 5);
    LCDC.tileDataSelect = ((address & 0x10) >> 4);
    LCDC.tileMapSelect = ((address & 0x08) >> 3);
    LCDC.spriteSize = ((address & 0x04) >> 2);
    LCDC.spriteDisplay = ((address & 0x02) >> 1);
    LCDC.bgWindowDisplay = ((address & 0x01));
}

unsigned char getLCDC(void) {
    return ((LCDC.lcdDisplay << 7) | (LCDC.windowTileMap << 6) | (LCDC.windowDisplay << 5) | (LCDC.tileDataSelect << 4) | (LCDC.tileMapSelect << 3) | (LCDC.spriteSize << 2) | (LCDC.spriteDisplay << 1) | (LCDC.bgWindowDisplay));
}

void setLCDS(unsigned char address) {
    LCDS.lyInterrupt = ((address & 0x40) >> 6);
    LCDS.oamInterrupt = ((address & 0x20) >> 5);
    LCDS.vblankInterrupt = ((address & 0x10) >> 4);
    LCDS.hblankInterrupt = ((address & 0x08) >> 3);
    LCDS.lyFlag = ((address & 0x04) >> 2);
    LCDS.modeFlag = ((address & 0x03));
}

unsigned char getLCDS(void) {
    return ((LCDS.lyInterrupt << 6) | (LCDS.oamInterrupt << 5) | (LCDS.vblankInterrupt << 4) | (LCDS.hblankInterrupt << 3) | (LCDS.lyFlag << 2) | (LCDS.modeFlag));
}

void setBGPalette(unsigned char address) {
    bgPalette[3] = ((address >> 6) & 0x03);
    bgPalette[2] = ((address >> 4) & 0x03);
    bgPalette[1] = ((address >> 2) & 0x03);
    bgPalette[0] = ((address) & 0x03);
}

void setSpritePalette1(unsigned char address) {
    spritePalette1[3] = ((address >> 6) & 0x03);
    spritePalette1[2] = ((address >> 4) & 0x03);
    spritePalette1[1] = ((address >> 2) & 0x03);
    spritePalette1[0] = ((address) & 0x03);
}

void setSpritePalette2(unsigned char address) {
    spritePalette2[3] = ((address >> 6) & 0x03);
    spritePalette2[2] = ((address >> 4) & 0x03);
    spritePalette2[1] = ((address >> 2) & 0x03);
    spritePalette2[0] = ((address) & 0x03);
}

void setScrollX(unsigned char address) {
    LCD.scrollX = address;
}

unsigned char getScrollX(void) {
    return LCD.scrollX;
}

void setScrollY(unsigned char address) {
    LCD.scrollY = address;
}

unsigned char getScrollY(void) {
    return LCD.scrollY;
}

void setWindowX(unsigned char address) {
    LCD.windowX = address;
}

void setWindowY(unsigned char address) {
    LCD.windowY = address;
}

int getLine(void) {
    return LCD.line;
}

void setLyCompare(unsigned char address) {
    LCD.lyCompare = address;
}