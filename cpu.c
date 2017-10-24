#define SET_AF(x) do {registers.A = ((x & 0xFF00) >> 8); registers.F = (x&0x00FF);} while(0) // multi-line macro
#define SET_BC(x) do {registers.B = ((x & 0xFF00) >> 8); registers.C = (x&0x00FF);} while(0)
#define SET_DE(x) do {registers.D = ((x & 0xFF00) >> 8); registers.E = (x&0x00FF);} while(0)
#define SET_HL(x) do {registers.H = ((x & 0xFF00) >> 8); registers.L = (x&0x00FF);} while(0)

#define GET_AF() ((registers.A << 8) | registers.F)
#define GET_BC() ((registers.B << 8) | registers.C)
#define GET_DE() ((registers.D << 8) | registers.E)
#define GET_HL() ((registers.H << 8) | registers.L)

#define SET_Z(x) ((registers.F & 0x7F) | (x << 7))
#define SET_N(x) ((registers.F & 0xBF) | (x << 6))
#define SET_H(x) ((registers.F & 0xDF) | (x << 5))
#define SET_C(x) ((registers.F & 0xEF) | (x << 4))

#define FLAG_Z ((registers.F >> 7) & 0x1)
#define FLAG_N ((registers.F >> 6) & 0x1)
#define FLAG_H ((registers.F >> 5) & 0x1)
#define FLAG_C ((registers.F >> 4) & 0x1)

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

    registers.PC = 0x100;
    registers.cycles = 0;
}

void cpuCycle(void) {
    unsigned char instruction = readByte(registers.PC);

    switch (instruction) {
        case 0x01:    // LD BC,nn
            SET_BC(readShort(registers.PC+1));
            registers.PC += 3;
            registers.cycles += 3;
            break;
        case 0x02:    // LD (BC),A
            writeByte(GET_BC(), registers.A);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x04:    // INC B
            registers.B += 1;
            SET_Z(!registers.B);
            SET_N(0);
            SET_H((registers.B & 0xF) < ((registers.B-1) & 0xF));
            break;
        case 0x05:    // DEC B
            registers.B -= 1;
            SET_Z(registers.B);
            SET_N(1);
            SET_H((registers.B & 0xF) > ((registers.B-1) & 0xF));
            break;
        case 0x06:    // LD B,n
            registers.B = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x08:    // LD (nn),SP
            writeShort(readShort(registers.PC+1, registers.SP));
            registers.PC += 3;
            registers.cycles += 5;
            break;
        case 0x0A:    // LD A,(BC)
            registers.A = readByte(GET_BC());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x0C:    // INC C
            registers.C += 1;
            SET_Z(!registers.C);
            SET_N(0);
            SET_H((registers.C & 0xF) < ((registers.C-1) & 0xF));
            break;
        case 0x0C:    // DEF C
            registers.C -= 1;
            SET_Z(registers.C);
            SET_N(1);
            SET_H((registers.C & 0xF) > ((registers.C-1) & 0xF));
            break;
        case 0x0E:    // LD C,n
            registers.C = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x11:    // LD DE,nn
            SET_DE(readShort(registers.PC+1));
            registers.PC += 3;
            registers.cycles += 3;
            break;
        case 0x12:    // LD (DE),A
            writeByte(GET_DE(), registers.A);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x14:    // INC D
            registers.D += 1;
            SET_Z(!registers.D);
            SET_N(0);
            SET_H((registers.D & 0xF) < ((registers.D-1) & 0xF));
            break;
        case 0x15:    // DEC D
            registers.D -= 1;
            SET_Z(registers.D);
            SET_N(1);
            SET_H((registers.D & 0xF) > ((registers.D-1) & 0xF));
            break;
        case 0x16:    // LD D,n
            registers.D = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x1A:    // LD A,(DE)
            registers.A = readByte(GET_DE());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x1C:    // INC E
            registers.E += 1;
            SET_Z(!registers.E);
            SET_N(0);
            SET_H((registers.E & 0xF) < ((registers.E-1) & 0xF));
            break;
        case 0x1D:    // DEC E
            registers.E -= 1;
            SET_Z(registers.E);
            SET_N(1);
            SET_H((registers.E & 0xF) > ((registers.E-1) & 0xF));
            break;
        case 0x1E:    // LD E,n
            registers.E = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x21:    // LD HL,nn
            SET_HL(readShort(registers.PC+1));
            registers.PC += 3;
            registers.cycles += 3;
            break;
        case 0x22:    // LDI (HL), A
            writeByte(GET_HL(),registers.A);
            SET_HL(GET_HL()+1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x24:    // INC H
            registers.H += 1;
            SET_Z(!registers.H);
            SET_N(0);
            SET_H((registers.H & 0xF) < ((registers.H-1) & 0xF));
            break;
        case 0x25:    // DEC H
            registers.H -= 1;
            SET_Z(registers.H);
            SET_N(1);
            SET_H((registers.H & 0xF) > ((registers.H-1) & 0xF));
            break;
        case 0x26:    // LD H,n
            registers.H = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x2A:    // LDI A,(HL)
            registers.A = readByte(GET_HL());
            SET_HL(GET_HL()+1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x2C:    // INC L
            registers.L += 1;
            SET_Z(!registers.L);
            SET_N(0);
            SET_H((registers.L & 0xF) < ((registers.L-1) & 0xF));
            break;
        case 0x2D:    // DEC L
            registers.L -= 1;
            SET_Z(registers.L);
            SET_N(1);
            SET_H((registers.L & 0xF) > ((registers.L-1) & 0xF));
            break;
        case 0x2E:    // LD L,n
            registers.L = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x31:    // LD SP,nn
            registers.SP = readShort(registers.PC+1);
            registers.PC += 3;
            registers.cycles += 3;
            break;
        case 0x32:    // LDD (HL), A
            writeByte(GET_HL(),registers.A);
            SET_HL(GET_HL() - 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x34:    // INC (HL)
            unsigned char t = readByte(GET_HL()) + 1;
            writeByte(GET_HL(), t);
            SET_Z(!t);
            SET_N(0);
            SET_H((t & 0xF) < ((t-1) & 0xF));
            break;
        case 0x35:    // DEC (HL)
            unsigned char t = readByte(GET_HL()) - 1;
            writeByte(GET_HL(), t);
            SET_Z(t);
            SET_N(1);
            SET_H((t & 0xF) > ((t-1) & 0xF));
            break;
        case 0x3A:    // LDD A, (HL)
            registers.A = readByte(GET_HL());
            SET_HL(GET_HL() - 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x3C:    // INC A
            registers.A += 1;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((registers.A & 0xF) < ((registers.A-1) & 0xF));
            break;
        case 0x3D:    // DEC A
            registers.A -= 1;
            SET_Z(registers.A);
            SET_N(1);
            SET_H((registers.A & 0xF) > ((registers.A-1) & 0xF));
            break;
        case 0x40:    // LD B,B
            registers.B = registers.B;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x41:    // LD B,C
            registers.B = registers.C;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x42:    // LD B,D
            registers.B = registers.D;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x43:    // LD B,E
            registers.B = registers.E;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x44:    // LD B,H
            registers.B = registers.H;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x45:    // LD B,L
            registers.B = registers.L;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x46:    // LD B,(HL)
            registers.B = readByte(GET_HL());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x47:    // LD B,A
            registers.B = registers.A;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x48:    // LD C,B
            registers.C = registers.B;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x49:    // LD C,C
            registers.C = registers.C;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x4A:    // LD C,D
            registers.C = registers.D;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x4B:    // LD C,E
            registers.C = registers.E;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x4C:    // LD C,H
            registers.C = registers.H;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x4D:    // LD C,L
            registers.C = registers.L;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x4E:    // LD C,(HL)
            registers.C = readByte(GET_HL());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x4F:    // LD C, A
            registers.C = registers.A;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x50:    // LD D,B
            registers.D = registers.B;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x51:    // LD D,C
            registers.D = registers.C;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x52:    // LD D,D
            registers.D = registers.D;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x53:    // LD D,E
            registers.D = registers.E;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x54:    // LD D,H
            registers.D = registers.H;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x55:    // LD D,L
            registers.D = registers.L;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x56:    // LD D,(HL)
            registers.D = readByte(GET_HL());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x57:    // LD D,A
            registers.D = registers.A;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x58:    // LD E,B
            registers.E = registers.B;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x59:    // LD E,C
            registers.E = registers.C;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x5A:    // LD E,D
            registers.E = registers.D;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x5B:    // LD E,E
            registers.E = registers.E;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x5C:    // LD E,H
            registers.E = registers.H;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x5D:    // LD E,L
            registers.E = registers.L;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x5E:    // LD E,(HL)
            registers.E = readByte(GET_HL());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x5F:    // LD E,A
            registers.E = registers.A;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x60:    // LD H,B
            registers.H = registers.B;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x61:    // LD H,C
            registers.H = registers.C;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x62:    // LD H,D
            registers.H = registers.D;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x63:    // LD H,E
            registers.H = registers.E;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x64:    // LD H,H
            registers.H = registers.H;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x65:    // LD H,L
            registers.H = registers.L;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x66:    // LD H,(HL)
            registers.H = readByte(GET_HL());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x67:    // LD H,A
            registers.H = registers.A;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x68:    // LD L,B
            registers.L = registers.B;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x69:    // LD L,C
            registers.L = registers.C;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x6A:    // LD L,D
            registers.L = registers.D;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x6B:    // LD L,E
            registers.L = registers.E;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x6C:    // LD L,H
            registers.L = registers.H;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x6D:    // LD L,L
            registers.L = registers.L;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x6E:    // LD L,(HL)
            registers.L = readByte(GET_HL());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x6F:    // LD L,A
            registers.L = registers.A;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x70:    // LD (HL),B
            writeByte(GET_HL(), registers.B);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x71:    // LD (HL),C
            writeByte(GET_HL(), registers.C);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x72:    // LD (HL),D
            writeByte(GET_HL(), registers.D);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x73:    // LD (HL),E
            writeByte(GET_HL(), registers.E);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x74:    // LD (HL),H
            writeByte(GET_HL(), registers.H);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x75:    // LD (HL),L
            writeByte(GET_HL(), registers.L);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x76:    // LD (HL),n
            writeByte(GET_HL(), readByte(registers.PC+1));
            registers.PC += 2;
            registers.cycles += 4;
            break;
        case 0x77:    // LD (HL),A
            writeByte(GET_HL(), registers.A);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x78:    // LD A,B
            registers.A = registers.B;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x79:    // LD A,C
            registers.A = registers.C;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x7A:    // LD A,D
            registers.A = registers.D;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x7B:    // LD A,E
            registers.A = registers.E;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x7C:    // LD A,H
            registers.A = registers.H;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x7D:    // LD A,L
            registers.A = registers.L;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x7E:    // LD A,(HL)
            registers.A = readByte(GET_HL());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xFA:    // LD A,(nn)
            registers.A = readShort(registers.PC+1);
            registers.PC += 3;
            registers.cycles += 4;
            break;
        case 0x7F:    // LD A,A
            registers.A = registers.A;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x80:    // ADD A,B
            int i = registers.A + registers.B;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x81:    // ADD A,C
            int i = registers.A + registers.C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x82:    // ADD A,D
            int i = registers.A + registers.D;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x83:    // ADD A,E
            int i = registers.A + registers.E;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x84:    // ADD A,H
            int i = registers.A + registers.H;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x85:    // ADD A,L
            int i = registers.A + registers.L;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x86:    // ADD A,(HL)
            int i = registers.A + readByte(GET_HL);
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x87:    // ADD A,A
            int i = registers.A + registers.A;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x88:    // ADC A,B
            int i = registers.A + registers.B + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x89:    // ADC A,C
            int i = registers.A + registers.C + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8A:    // ADC A,D
            int i = registers.A + registers.D + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8B:    // ADC A,E
            int i = registers.A + registers.E + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8C:    // ADC A,H
            int i = registers.A + registers.H + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8D:    // ADC A,L
            int i = registers.A + registers.L + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8E:    // ADC A,(HL)
            int i = registers.A + GET_HL() + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x8F:    // ADC A,A
            int i = registers.A + registers.A + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x90:    // SUB A,B
            int i = registers.A - registers.B;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x91:    // SUB A,C
            int i = registers.A - registers.C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x92:    // SUB A,D
            int i = registers.A - registers.D;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x93:    // SUB A,E
            int i = registers.A - registers.E;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x94:    // SUB A,H
            int i = registers.A - registers.B;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x95:    // SUB A,L
            int i = registers.A - registers.L;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x96:    // SUB A,(HL)
            int i = registers.A - GET_HL;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x97:    // SUB A,A
            int i = registers.A - registers.A;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x98:    // SBC A,B
            int i = registers.A - registers.B + FLAG_C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x99:    // SBC A,C
            int i = registers.A - registers.C + FLAG_C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9A:    // SBC A,D
            int i = registers.A - registers.D + FLAG_C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9B:    // SBC A,E
            int i = registers.A - registers.E + FLAG_C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9C:    // SBC A,H
            int i = registers.A - registers.H + FLAG_C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9D:    // SBC A,L
            int i = registers.A - registers.L + FLAG_C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9E:    // SBC A,(HL)
            int i = registers.A - readByte(GET_HL()) + FLAG_C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9F:    // SBC A,A
            int i = registers.A - registers.A + FLAG_C;
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA0:    // AND A,B
            registers.A &= registers.B;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA1:    // AND A,C
            registers.A &= registers.C;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA2:    // AND A,D
            registers.A &= registers.D;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA3:    // AND A,E
            registers.A &= registers.E;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA4:    // AND A,H
            registers.A &= registers.H;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA5:    // AND A,L
            registers.A &= registers.L;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA6:    // AND A,(HL)
            registers.A &= readByte(GET_HL());
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xA7:    // AND A,A
            registers.A &= registers.A;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA8:    // XOR A,B
            registers.A ^= registers.B;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA9:    // XOR A,C
            registers.A ^= registers.C;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAA:    // XOR A,D
            registers.A ^= registers.D;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAB:    // XOR A,E
            registers.A ^= registers.E;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAC:    // XOR A,H
            registers.A ^= registers.H;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAD:    // XOR A,L
            registers.A ^= registers.L;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAE:    // XOR A,(HL)
            registers.A ^= readByte(GET_HL());
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAF:    // XOR A,A
            registers.A ^= registers.A;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xB0:    // OR A,B
            registers.A |= registers.B;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB1:    // OR A,C
            registers.A |= registers.C;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB2:    // OR A,D
            registers.A |= registers.D;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB3:    // OR A,E
            registers.A |= registers.E;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB4:    // OR A,H
            registers.A |= registers.H;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB5:    // OR A,L
            registers.A |= registers.L;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB6:    // OR A,(HL)
            registers.A |= readByte(GET_HL());
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xB7:    // OR A,A
            registers.A |= registers.A;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB8:    // CP B
            SET_Z(registers.A == registers.B);
            SET_N(1);
            SET_H(((registers.A-registers.B) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < registers.B);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB9:    // CP C
            SET_Z(registers.A == registers.C);
            SET_N(1);
            SET_H(((registers.A-registers.C) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < registers.C);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBA:    // CP D
            SET_Z(registers.A == registers.D);
            SET_N(1);
            SET_H(((registers.A-registers.D) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < registers.D);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBB:    // CP E
            SET_Z(registers.A == registers.E);
            SET_N(1);
            SET_H(((registers.A-registers.E) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < registers.E);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBC:    // CP H
            SET_Z(registers.A == registers.H);
            SET_N(1);
            SET_H(((registers.A-registers.H) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < registers.H);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBD:    // CP L
            SET_Z(registers.A == registers.L);
            SET_N(1);
            SET_H(((registers.A-registers.L) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < registers.L);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBE:    // CP (HL)
            SET_Z(registers.A == readByte(GET_HL()));
            SET_N(1);
            SET_H(((registers.A-readByte(GET_HL())) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < readByte(GET_HL()));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xBF:    // CP
            SET_Z(registers.A == registers.A);
            SET_N(1);
            SET_H(((registers.A-registers.A) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xC1:    // POP BC
            SET_BC(readShort(registers.SP) & 0xFFF0);
            registers.SP += 2;
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0xC5:    // PUSH BC
            registers.SP -= 2;
            writeShort(registers.SP, GET_BC());
            registers.PC += 1;
            registers.cycles += 4;
            break;
        case 0xC6:    // ADD A,n
            int i = registers.A + readByte(registers.PC);
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xCE:    // ADC A,n
            int i = registers.A + readByte(registers.PC) + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H((i & 0xF) < (registers.A & 0xF));
            SET_C((i & 0xFF) < (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xD1:    // POP DE
            SET_DE(readShort(registers.SP) & 0xFFF0);
            registers.SP += 2;
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0xD5:    // PUSH DE
            registers.SP -= 2;
            writeShort(registers.SP, GET_DE());
            registers.PC += 1;
            registers.cycles += 4;
            break;
        case 0xD6:    // SUB A,n
            int i = registers.A - readByte(registers.PC);
            SET_Z(i);
            SET_N(1);
            SET_H((i & 0xF) > (registers.A & 0xF));
            SET_C((i & 0xFF) > (registers.A & 0xFF));
            registers.A = i;
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xE0:    // LD ($FF00+n), A
            writeByte(0xFF00 + readByte(registers.PC), registers.A);
            registers.PC += 2;
            registers.cycles += 3;
            break;
        case 0xE1:    // POP HL
            SET_HL(readShort(registers.SP) & 0xFFF0);
            registers.SP += 2;
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0xE2:    // LD A,($FF00+C)
            writeByte(registers.C + 0xFF00, registers.A);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xE5:    // PUSH HL
            registers.SP -= 2;
            writeShort(registers.SP, GET_HL());
            registers.PC += 1;
            registers.cycles += 4;
            break;
        case 0xE6:    // AND A,n
            registers.A &= readByte(registers.PC);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((1);
            SET_C(0);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xEA:    // LD A,n
            writeByte(readShort(registers.PC+1), registers.A);
            registers.PC += 3;
            registers.cycles += 4;
            break;
        case 0xEE:    // XOR A,n
            registers.A ^= readByte(registers.PC);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xF0:    // LD A, ($FF00+n)
            registers.A = readByte(registers.PC+ 0xFF00);
            registers.PC += 2;
            registers.cycles += 3;
            break;
        case 0xF1:    // POP AF
            SET_AF(readShort(registers.SP) & 0xFFF0);
            registers.SP += 2;
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0xF2:    // LD A,($FF00+C)
            registers.A = readByte(registers.C + 0xFF00);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xF5:    // PUSH AF
            registers.SP -= 2;
            writeShort(registers.SP, GET_AF());
            registers.PC += 1;
            registers.cycles += 4;
            break;
        case 0xF6:    // OR A,n
            registers.A |= readByte(registers.PC);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xF8:    // LD HL, SP + n
            SET_HL(registers.SP + readByte(registers.PC+1));
            set_N(0);
      			set_Z(0);
      			set_C(((registers.SP+i)&0xFF) < (registers.SP&0xFF)); // a carry will cause a wrap around = making new value smaller
      			set_H(((registers.SP+i)&0x0F) < (registers.SP&0x0F)); // add the two, see if it becomes larger
            registers.PC += 2;
            registers.cycles += 3;
            break;
        case 0xF9:    // LD SP,HL
            registers.SP = GET_HL();
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xFE:    // CP n
            SET_Z(registers.A == readByte(registers.PC));
            SET_N(1);
            SET_H(((registers.A-readByte(registers.PC)) & 0xF) > (registers.A & 0xF));
            SET_C(registers.A < readByte(registers.PC));
            registers.PC += 2;
            registers.cycles += 2;
            break;
        default:
            printf("Undefined instruction.");
            break;
    }
}
