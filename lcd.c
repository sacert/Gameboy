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

void drawBgWindow(unsigned int* buf, int line) {

    int x;
    for (x = 0; x < 160; x++) {

        unsigned int xShift, yShift, map, mapOffset, tileNum, tileAddr;
        unsigned char buf1, buf2, colour;

        if (LCDC.windowDisplay && line >= LCD.windowY && line-LCD.windowY< 144) {
            xShift = x;
            yShift = line - LCD.windowY;
            map = LCDC.windowTileMap;
        } else {
            if (!LCDC.bgWindowDisplay) {
                buf[line*640 + x] = 0;
            }
            xShift = (x + LCD.scrollX) % 256;
            yShift = (line + LCD.scrollX) % 256;
            map = LCDC.tileMapSelect;
        }
        mapOffset = (yShift/8)*32 + xShift/8;
        tileNum = readByte(0x9800 + map * 0x400 + mapOffset);

        if (LCDC.tileMapSelect)
            tileAddr = 0x8000 + tileNum*16;
        else
            tileAddr = 0x9000 + ((signed char)tileNum)*16;
        
        buf1 = readByte(tileAddr + (yShift%8)*2);
        buf2 = readByte(tileAddr + (yShift%8)*2 + 1);
        colour = ((!!(buf2 & (128 >> xShift%8))<<1) | !!(buf1 & (128 >> xShift%8)));
        buf[line * 640 + x] = colours[bgPalette[colour]];

    }
}

void draw_sprites(unsigned int *b, int line, int nsprites, struct sprite *s)
{
	int i;

	for(i = 0; i < nsprites; i++)
	{
		unsigned int b1, b2, tile_addr, sprite_line, x;

		/* Sprite is offscreen */
		if(s[i].x < -7)
			continue;

		/* Which line of the sprite (0-7) are we rendering */
		sprite_line = s[i].flags & 0x40 ? (LCDC.spriteSize ? 15 : 7)-(line - s[i].y) : line - s[i].y;

		/* Address of the tile data for this sprite line */
		tile_addr = 0x8000 + (s[i].patternNum*16) + sprite_line*2;

		/* The two bytes of data holding the palette entries */
		b1 = readByte(tile_addr);
		b2 = readByte(tile_addr+1);

		/* For each pixel in the line, draw it */
		for(x = 0; x < 8; x++)
		{
			unsigned char mask, colour;
			int *pal;

			if((s[i].x + x) >= 160)
				continue;

			mask = s[i].flags & 0x20 ? 128>>(7-x) : 128>>x;
			colour = ((!!(b2&mask))<<1) | !!(b1&mask);
			if(colour == 0)
				continue;


			pal = (s[i].flags & 0x10) ? spritePalette2 : spritePalette1;
			/* Sprite is behind BG, only render over palette entry 0 */
			if(s[i].flags & 0x80)
			{
				unsigned int temp = b[line*640+(x + s[i].x)];
				if(temp != colours[bgPalette[0]])
					continue;
			}
			b[line*640+(x + s[i].x)] = colours[pal[colour]];
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
        sprite[c].y = readByte(0xFE00 + (i*4)) - 16; // read OAM
        
        if (line < sprite[c].y || line >= sprite[c].y + 8 + (8*LCDC.spriteSize)) // out of bounds check
            continue;
        
        sprite[c].x = readByte(0xFE00 + (i*4) + 1) - 8;
        sprite[c].patternNum = readByte(0xFE00 + (i*4) + 2);
        sprite[c].flags = readByte(0xFE00 + (i*4) + 3);
        c++;

        // max 10 sprites per line
        if (c == 10)
            break;
    }

    if (c > 0)
        sortSprites(sprite, c);

    drawBgWindow(buf, line);
    draw_sprites(buf, line, c, sprite);
}

void draw_stuff(void)
{
	unsigned int *b = sdlFrameBuffer();
	int y, tx, ty;

	for(ty = 0; ty < 24; ty++)
	{
	for(tx = 0; tx < 16; tx++)
	{
	for(y = 0; y<8; y++)
	{
		unsigned char b1, b2;
		int tileaddr = 0x8000 +  ty*0x100 + tx*16 + y*2;

		b1 = readByte(tileaddr);
		b2 = readByte(tileaddr+1);
		b[(ty*640*8)+(tx*8) + (y*640) + 0 + 0x1F400] = colours[(!!(b1&0x80))<<1 | !!(b2&0x80)];
		b[(ty*640*8)+(tx*8) + (y*640) + 1 + 0x1F400] = colours[(!!(b1&0x40))<<1 | !!(b2&0x40)];
		b[(ty*640*8)+(tx*8) + (y*640) + 2 + 0x1F400] = colours[(!!(b1&0x20))<<1 | !!(b2&0x20)];
		b[(ty*640*8)+(tx*8) + (y*640) + 3 + 0x1F400] = colours[(!!(b1&0x10))<<1 | !!(b2&0x10)];
		b[(ty*640*8)+(tx*8) + (y*640) + 4 + 0x1F400] = colours[(!!(b1&0x8))<<1 | !!(b2&0x8)];
		b[(ty*640*8)+(tx*8) + (y*640) + 5 + 0x1F400] = colours[(!!(b1&0x4))<<1 | !!(b2&0x4)];
		b[(ty*640*8)+(tx*8) + (y*640) + 6 + 0x1F400] = colours[(!!(b1&0x2))<<1 | !!(b2&0x2)];
		b[(ty*640*8)+(tx*8) + (y*640) + 7 + 0x1F400] = colours[(!!(b1&0x1))<<1 | !!(b2&0x1)];
	}
	}
	}
}

int lcdCycle(void) {
    
    int cycles = getCycles();
    int prevLine;
    
    if (sdlUpdate())
        return 0;
    LCD.frame = cycles % (70224/4); // 70224 clks per screen
    LCD.line = LCD.frame / (456/4); // 465 clks per line

    if (LCD.frame < 204/4)
        LCDS.modeFlag = 2;  // OAM
    else if (LCD.frame < 284/4)
        LCDS.modeFlag = 3;  // VRA
    else if (LCD.frame < 456/4)
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
        draw_stuff();
        interrupt.flags |= VBLANK;
        sdlSetFrame();
    }

    //printf("%i\n", LCD.line);
    
    prevLine = LCD.line;
    
    return 1;
}