#include "lcd.h"
#include "cpu.h"
#include "sdl.h"
#include "interrupt.h"
#include "MMU.h"

struct LCD LCD;
struct LCDC LCDC;
struct LCDS LCDS;

int bgPalette[4];
int spritePalette1[4];
int spritePalette2[4];
unsigned long colours[4] = {0xFFFFFF, 0xC0C0C0, 0x808080, 0x000000};

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
    LCD.lyCompare = (LCD.line == address);
}

void sortSprites(struct sprite* sprite, int c) {
    
    // blessed insertion sort 
    int i, j;
    for (i = 0; i < c; i++) {
        for (j = 0; j < c-1; j++) {
            if (sprite[j].x < sprite[j+1].x) {
                struct sprite s;

                s = sprite[j+1];
                sprite[j] = sprite[j+1];
                sprite[j+1] = s;
            }
        }
    }
}

void drawBgWindow(unsigned int *buf, int line) {
	int x;
	for(x = 0; x < 160; x++) // for the x size of the window (160x144)
	{
		unsigned int mapSelect, tileMapOffset, tileNum, tileAddr, currX, currY;
        unsigned char buf1, buf2, mask, colour;

		if(line >= LCD.windowY && LCDC.windowDisplay) { // wind
			currX = x;
			currY = line - LCD.windowY;
			mapSelect = LCDC.windowTileMap;
		} else if (LCDC.bgWindowDisplay) { // background
			currX = (x + LCD.scrollX) % 256; // mod 256 since if it goes off the screen, it wraps around
			currY = (line + LCD.scrollY) % 256;
			mapSelect = LCDC.tileMapSelect;
		} else { // neither
            buf[line*160 + x] = 0; // if not window or background, make it white
            return;
        }

        // map window to 32 rows of 32 bytes
		tileMapOffset = (currY/8)*32 + currX/8;

		tileNum = readByte(0x9800 + mapSelect*0x400 + tileMapOffset);
		if(LCDC.tileDataSelect)
			tileAddr = 0x8000 + (tileNum*16) + ((currY%8)*2);
		else
			tileAddr = 0x9000 + (((signed int)tileNum)*16) + ((currY%8)*2); // pattern 0 lies at 0x9000

		buf1 = readByte(tileAddr); // 2 bytes represent the line
		buf2 = readByte(tileAddr+1);
		mask = 128>>(currX%8);
		colour = (!!(buf2&mask)<<1) | !!(buf1&mask);
		buf[line*160 + x] = colours[bgPalette[colour]];
	}
}

void drawSprites(unsigned int *buf, int line, int blocks, struct sprite *sprite) {
	
    int i;
	for(i = 0; i < blocks; i++)
	{
		unsigned int buf1, buf2, tileAddr, spriteRow, x;

		// off screen
		if(sprite[i].x < -7)
			continue;

        if (sprite[i].flags & 0x40) {
            int size;
            if (LCDC.spriteSize == 15)
                size = 15;
            else 
                size = 7;
            spriteRow = size - (line - sprite[i].y); 
        } else {
            spriteRow = line - sprite[i].y;
        }

        // similar to background
		tileAddr = 0x8000 + (sprite[i].patternNum*16) + spriteRow*2;

		buf1 = readByte(tileAddr);
		buf2 = readByte(tileAddr+1);

		// draw each pixel
		for(x = 0; x < 8; x++)
		{
            // out of bounds check
            if((sprite[i].x + x) >= 160 && sprite[i].x < 0)
				continue;

			unsigned char mask, colour;
			int *pal;

			mask = sprite[i].flags & 0x20 ? 128>>(7-x) : 128>>x;
			colour = ((!!(buf2&mask))<<1) | !!(buf1&mask);

			if(colour == 0) // no need to draw it
				continue;

			pal = (sprite[i].flags & 0x10) ? spritePalette2 : spritePalette1;

			// only render over colour 0
			if(sprite[i].flags & 0x80)
			{
				unsigned int temp = buf[line*160+(x + sprite[i].x)];
				if(temp != colours[bgPalette[0]])
					continue;
			}
			//buf[line*160+(x + sprite[i].x)] = colours[pal[colour]]; // for testing
		}
	}
}

void renderLine(int line) {

    int i = 0; // num of OAM blocks
    int c = 0; // block counter
    struct sprite sprite[10]; // max 10 sprites per line
    unsigned int *buf = sdlFrameBuffer();

    // OAM is divided into 40 4-byte blocks each - corresponding to a sprite
    for (i = 0; i < 40; i++) {

        int y;
        y = readByte(0xFE00 + (i*4) - 16);
        sprite[c].y = readByte(0xFE00 + (i*4)) - 16; // read OAM
        
        if (line < y || line >= y + 8 + (8*LCDC.spriteSize)) // out of bounds check
            continue;
        
        sprite[c].y = y;
        sprite[c].x = readByte(0xFE00 + (i*4) + 1) - 8;
        sprite[c].patternNum = readByte(0xFE00 + (i*4) + 2);
        sprite[c].flags = readByte(0xFE00 + (i*4) + 3);
        c++;

        // max 10 sprites per line
        if (c == 10)
            break;
    }

    if (c)
        sortSprites(sprite, c);

    drawBgWindow(buf, line);
    drawSprites(buf, line, c, sprite);
}

int lcdCycle(void) {
    
    int cycles = getCycles();
    int prevLine;
    
    sdlUpdate();
    LCD.frame = cycles % (70224/64); // 70224 clks per screen
    LCD.line = LCD.frame / (456/64); // 465 clks per line

    if (LCD.frame < 204/64)
        LCDS.modeFlag = 2;  // OAM
    else if (LCD.frame < 284/64)
        LCDS.modeFlag = 3;  // VRA
    else if (LCD.frame < 456/64)
        LCDS.modeFlag = 0;  // HBlank
    
    // Done all lines
    if (LCD.line >= 144)
        LCDS.modeFlag = 1;  // VBlank

    if (LCD.line != prevLine && LCD.line < 144) {
        renderLine(LCD.line);
    }

    if (LCDS.lyInterrupt && LCD.lyCompare) {
        interrupt.flags |= LCDSTAT;
    }

    if (LCD.line == 144) {
        // draw the entire frame
        interrupt.flags |= VBLANK;
        sdlSetFrame();
    }
    
    prevLine = LCD.line;
    
    return 1;
}