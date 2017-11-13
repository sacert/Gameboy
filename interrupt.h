#define VBLANK    (1 << 0)
#define LCDSTAT   (1 << 1)
#define TIMER     (1 << 2)
#define SERIAL    (1 << 3)
#define JOYPAD    (1 << 4)

struct interrupt {
    unsigned char master;
    unsigned char enable;
    unsigned char flags;
    unsigned char pending;
} extern interrupt;

void interruptCycle(void);
