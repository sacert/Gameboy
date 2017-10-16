struct registers {
    unsigned char A;
    unsigned char F; // flags: Z N H C

    unsigned char B;
    unsigned char C;

    unsigned char D;
    unsigned char E;

    unsigned char H;
    unsigned char L;

    unsigned short SP;
    unsigned short PC;

    unsigned int cycles;
} registers;

struct registers registers;

void reset(void) {

    registers.pc = 0x100;
    registers.cycles = 0;
}

void cpuCycle(void) {
    unsigned char instruction = readByte(registers.pc++);

    switch (instruction) {
        case 0x43:
            registers.b = registers.e;
            break;
        default:
            printf("Undefined instruction.");
            break;
    }
}
