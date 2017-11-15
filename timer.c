#include "timer.h"
#include "interrupt.h"
#include "cpu.h"

struct timer timer;
unsigned int time = 0;
unsigned int change = 0;

void setDiv(unsigned char address) {
    timer.div = 0; // setting div to anything makes it 0
}

unsigned int getDiv(void) {
    return timer.div;
}

void setTima(unsigned char address) {
    timer.tima = address;
}

unsigned int getTima(void) {
    return timer.tima;
}

void setTma(unsigned char address) {
    timer.tma = address;
}

unsigned int getTma(void) {
    return timer.tma;
}

void setTac(unsigned char address) { // revisit this
    int speeds[] = {64, 1, 4, 16};
    timer.tac = address;
    timer.started = address & 4;
    timer.speed = speeds[address & 3];
}

unsigned int getTac(void) {
    return timer.tac;
}

void tick(void) {

    timer.tick += 1;

    if (timer.tick == 16) {
        timer.div += 1;
        timer.tick = 0;
    }

    if (!timer.started)
        return;
    
    if (timer.tick == timer.speed) {
        timer.tima += 1;
        timer.tick = 0;
    }

    if (timer.tima == 0x100) {
        interrupt.flags |= TIMER;
        timer.tima = timer.tma;
    }
}

void timerCycle(void) {
    unsigned int delta = getCycles() - time;
    time = getCycles();

    change += time * 4;

    if (change >= 16) {
        tick();
        change -= 16;
    }
}


