#import "interrupt.h"
#import "cpu.h"

struct interrupt interrupt;

void interruptCycle(void) {
    // if everything is enabled and there is a flag set
    if (interrupt.master && interrupt.enable && interrupt.flags) {
        // get which interrupt is currently being executed
        unsigned char inter = interrupt.enable & interrupt.flags;

        if (inter & VBLANK) {
            interrupt.flags &= ~VBLANK; // turn off the flag
            cpu_interrupt(0x40);
        }

        if (inter & LCDSTAT) {
            interrupt.flags &= ~LCDSTAT;
            cpu_interrupt(0x48);
        }

        if (inter & TIMER) {
            interrupt.flags &= ~TIMER;
            cpu_interrupt(0x50);
        }

        if (inter & SERIAL) {
            interrupt.flags &= ~SERIAL;
            cpu_interrupt(0x58);
        }

        if (inter & JOYPAD) {
            interrupt.flags &= ~JOYPAD;
            cpu_interrupt(0x60);
        }
    }
}
