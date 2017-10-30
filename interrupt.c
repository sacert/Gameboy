#import "interrupt.h"

struct interrupt interrupt;

void interruptCycle(void) {
    // if everything is enabled and there is a flag set
    if (interrupt.master && interrupt.enable && interrupt.flags) {
        // get which interrupt is currently being executed
        unsigned char inter = interrupt.enable & interrupt.flags;

        if (fire & VBLANK) {
            interrupt.flags &= ~VBLANK; // turn off the flag
            cpu_interrupt(0x40);
        }

        if (fire & LCDSTAT) {
            interrupt.flags &= ~LCDSTAT;
            cpu_interrupt(0x48);
        }

        if (fire & TIMER) {
            interrupt.flags &= ~TIMER;
            cpu_interrupt(0x50);
        }

        if (fire & SERIAL) {
            interrupt.flags &= ~SERIAL;
            cpu_interrupt(0x58);
        }

        if (fire & JOYPAD) {
            interrupt.flags &= ~JOYPAD;
            cpu_interrupt(0x60);
        }
    }
}
