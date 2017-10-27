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
static int halt = 0; // varify if static is needed

void reset(void) {

    registers.PC = 0x100;
    registers.cycles = 0;
}

void cpuCycle(void) {

    if (halt) {
      registers.cycles += 1;
      return;
    }

    unsigned char instruction = readByte(registers.PC);

    switch (instruction) {
        case 0x00:    // NOP
            registers.PC += 1;
            registers.cycles += 1;
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
        case 0x03:    // INC BC
            SET_BC(GET_BC() + 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x04:    // INC B
            registers.B += 1;
            SET_Z(!registers.B);
            SET_N(0);
            SET_H((registers.B & 0xF) < ((registers.B-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x05:    // DEC B
            registers.B -= 1;
            SET_Z(registers.B);
            SET_N(1);
            SET_H((registers.B & 0xF) > ((registers.B-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x06:    // LD B,n
            registers.B = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x07:    // RLCA
            unsigned char s = registers.A;
            s = (s >> 7);
            registers.A = (registers.A << 1) | s;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x08:    // LD (nn),SP
            writeShort(readShort(registers.PC+1, registers.SP));
            registers.PC += 3;
            registers.cycles += 5;
            break;
        case 0x09:    // ADD HL,BC
            unsigned short s = GET_HL();
            SET_HL(s + GET_BC());
            SET_N(0);
            SET_H((GET_HL() & 0xFFF) < (s & 0xFFF));
            SET_C((GET_HL() & 0xFFFF) < (s & 0xFFFF));
            registers.PC += 1;
            registers.cycles += 2;
        case 0x0A:    // LD A,(BC)
            registers.A = readByte(GET_BC());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x0B:    // DEC BC
            SET_BC(GET_BC() - 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x0C:    // INC C
            registers.C += 1;
            SET_Z(!registers.C);
            SET_N(0);
            SET_H((registers.C & 0xF) < ((registers.C-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x0C:    // DEF C
            registers.C -= 1;
            SET_Z(registers.C);
            SET_N(1);
            SET_H((registers.C & 0xF) > ((registers.C-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x0E:    // LD C,n
            registers.C = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x0F:    // RRCA
            unsigned char s = registers.A;
            s = (s & 0x1);
            registers.A = (registers.A >> 1) | (s << 7);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.PC += 1;
            registers.cycles += 1;
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
        case 0x13:    // INC DE
            SET_DE(GET_DE() + 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x14:    // INC D
            registers.D += 1;
            SET_Z(!registers.D);
            SET_N(0);
            SET_H((registers.D & 0xF) < ((registers.D-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x15:    // DEC D
            registers.D -= 1;
            SET_Z(registers.D);
            SET_N(1);
            SET_H((registers.D & 0xF) > ((registers.D-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x16:    // LD D,n
            registers.D = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x17:    // RLA
            unsigned char t = registers.A;
            registers.A = (registers.A << 1) | FLAG_C;
            SET_C(t >> 7);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x18:    // JP n
            registers.PC += (signed char)readByte(registers.PC+1);
            registers.cycles += 2;
            break;
        case 0x19:    // ADD HL,DE
            unsigned short s = GET_HL();
            SET_HL(s + GET_DE());
            SET_N(0);
            SET_H((GET_HL() & 0xFFF) < (s & 0xFFF));
            SET_C((GET_HL() & 0xFFFF) < (s & 0xFFFF));
            registers.PC += 1;
            registers.cycles += 2;
        case 0x1A:    // LD A,(DE)
            registers.A = readByte(GET_DE());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x1B:    // DEC DE
            SET_DE(GET_DE() - 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x1C:    // INC E
            registers.E += 1;
            SET_Z(!registers.E);
            SET_N(0);
            SET_H((registers.E & 0xF) < ((registers.E-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x1D:    // DEC E
            registers.E -= 1;
            SET_Z(registers.E);
            SET_N(1);
            SET_H((registers.E & 0xF) > ((registers.E-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x1E:    // LD E,n
            registers.E = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x1F:    // RRA
            unsigned char t = registers.A;
            t = (t & 0x1);
            registers.A = (registers.A >> 1) | (FLAG_C << 7);
            SET_C(t);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x20:    // JP NZ
            if (FLAG_Z == 0) {
              registers.PC += (signed char)readByte(registers.PC+1);
              registers.cycles += 3;
            } else {
              registers.PC += 2;
            }
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
        case 0x23:    // INC HL
            SET_HL(GET_HL() + 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x24:    // INC H
            registers.H += 1;
            SET_Z(!registers.H);
            SET_N(0);
            SET_H((registers.H & 0xF) < ((registers.H-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x25:    // DEC H
            registers.H -= 1;
            SET_Z(registers.H);
            SET_N(1);
            SET_H((registers.H & 0xF) > ((registers.H-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x26:    // LD H,n
            registers.H = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x27:    // DAA
            unsigned int s = registers.A;

            if (FLAG_N) {
              if(FLAG_H)
                registers.A = (registers.A - 0x06)&0xFF;
              if(FLAG_C)
                registers.A -= 0x60;
            } else {
              if(flag_H || (registers.A & 0xF) > 9)
                registers.A += 0x06;
              if(flag_C || registers.A > 0x9F)
                registers.A += 0x60;
            }
            SET_H(0);
            SET_Z(!registers.A);
            SET_C((registers.A & 0xFF) < (s & 0xFF));
            registers.PC += 1;
            registers.cycles += 1;
        case 0x28:    // JP Z
            if (FLAG_Z == 1) {
              registers.PC += (signed char)readByte(registers.PC+1);
              registers.cycles += 3;
            } else {
              registers.PC += 2;
            }
            break;
        case 0x29:    // ADD HL,HL
            unsigned short s = GET_HL();
            SET_HL(s + GET_HL());
            SET_N(0);
            SET_H((GET_HL() & 0xFFF) < (s & 0xFFF));
            SET_C((GET_HL() & 0xFFFF) < (s & 0xFFFF));
            registers.PC += 1;
            registers.cycles += 2;
        case 0x2A:    // LDI A,(HL)
            registers.A = readByte(GET_HL());
            SET_HL(GET_HL()+1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x2B:    // DEC HL
            SET_HL(GET_HL() - 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x2C:    // INC L
            registers.L += 1;
            SET_Z(!registers.L);
            SET_N(0);
            SET_H((registers.L & 0xF) < ((registers.L-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x2F:    // CPL
            registers.A = ~registers.A;
            SET_N(1);
            SET_H(1);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x2D:    // DEC L
            registers.L -= 1;
            SET_Z(registers.L);
            SET_N(1);
            SET_H((registers.L & 0xF) > ((registers.L-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x2E:    // LD L,n
            registers.L = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x30:    // JP NC
            if (FLAG_C == 0) {
              registers.PC += (signed char)readByte(registers.PC+1);
              registers.cycles += 3;
            } else {
              registers.PC += 2;
            }
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
        case 0x33:    // INC SP
            registers.SP += 1;
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x34:    // INC (HL)
            unsigned char t = readByte(GET_HL()) + 1;
            writeByte(GET_HL(), t);
            SET_Z(!t);
            SET_N(0);
            SET_H((t & 0xF) < ((t-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x35:    // DEC (HL)
            unsigned char t = readByte(GET_HL()) - 1;
            writeByte(GET_HL(), t);
            SET_Z(t);
            SET_N(1);
            SET_H((t & 0xF) > ((t-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x36:    // LD (HL),n
            writeByte(GET_HL(), readByte(registers.PC+1));
            registers.PC += 2;
            registers.cycles += 3;
            break;
        case 0x37:    // SCF
            SET_N(0);
            SET_H(0);
            SET_C(1);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x38:    // JP C
            if (FLAG_C == 1) {
              registers.PC += (signed char)readByte(registers.PC+1);
              registers.cycles += 3;
            } else {
              registers.PC += 2;
            }
            break;
        case 0x39:    // ADD HL,SP
            unsigned short s = GET_HL();
            SET_HL(s + registers.SP);
            SET_N(0);
            SET_H((GET_HL() & 0xFFF) < (s & 0xFFF));
            SET_C((GET_HL() & 0xFFFF) < (s & 0xFFFF));
            registers.PC += 1;
            registers.cycles += 2;
        case 0x3A:    // LDD A, (HL)
            registers.A = readByte(GET_HL());
            SET_HL(GET_HL() - 1);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x3B:    // DEC SP
            registers.SP -= 1;
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x3C:    // INC A
            registers.A += 1;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((registers.A & 0xF) < ((registers.A-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x3D:    // DEC A
            registers.A -= 1;
            SET_Z(registers.A);
            SET_N(1);
            SET_H((registers.A & 0xF) > ((registers.A-1) & 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x3F:    // CCF
            SET_N(0);
            SET_H(0);
            SET_C(!FLAG_C);
            registers.PC += 1;
            registers.cycles += 1;
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
        case 0x76:    // HALT
            halt = 1;
            registers.PC += 1;
            registers.cycles += 1;
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
        case 0xC2:    // JP NZ,nn
            if (FLAG_Z == 0) {
                registers.PC = readShort(registers.PC+1);
                registers.cyles += 3;
            } else {
                registers.PC += 3;
                registers.cyles += 2;
            }
            break;
        case 0xC3:    // JP nn
            registers.PC = readShort(registers.PC+1);
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
        case 0xCA:    // JP Z,nn
            if (FLAG_Z == 1) {
                registers.PC = readShort(registers.PC+1);
                registers.cyles += 3;
            } else {
                registers.PC += 3;
                registers.cyles += 2;
            }
            break;
        case 0xCB:    // Prefix
            cbPrefix(register.PC+1);
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
        case 0xD2:    // JP NC,nn
            if (FLAG_C == 0) {
                registers.PC = readShort(registers.PC+1);
                registers.cyles += 3;
            } else {
                registers.PC += 3;
                registers.cyles += 2;
            }
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
        case 0xDA:    // JP C,nn
            if (FLAG_C == 1) {
                registers.PC = readShort(registers.PC+1);
                registers.cyles += 3;
            } else {
                registers.PC += 3;
                registers.cyles += 2;
            }
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
        case 0xE8:    // ADD SP,n
            unsigned short s = registers.SP;
            registers.SP += readByte(registers.PC);
            SET_N(0);
            SET_H((GET_HL() & 0xF) < (s & 0xF));
            SET_C((GET_HL() & 0xFF) < (s & 0xFF));
            registers.PC += 2;
            registers.cycles += 4;
        case 0xE9:    // JP (HL)
            registers.PC = GET_HL();
            registers.cycles += 1;
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

void cbPrefix(inst) {
    unsigned char instruction = inst;

    switch (instruction) {
        case 0x00:    // RLC B
            unsigned char s = registers.B;
            s = (s >> 7);
            registers.B = (registers.B << 1) | s;
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x01:    // RLC C
            unsigned char s = registers.C;
            s = (s >> 7);
            registers.C = (registers.C << 1) | s;
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x02:    // RLC D
            unsigned char s = registers.D;
            s = (s >> 7);
            registers.D = (registers.D << 1) | s;
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x03:    // RLC E
            unsigned char s = registers.E;
            s = (s >> 7);
            registers.E = (registers.E << 1) | s;
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x04:    // RLC H
            unsigned char s = registers.H;
            s = (s >> 7);
            registers.H = (registers.H << 1) | s;
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x05:    // RLC L
            unsigned char s = registers.L;
            s = (s >> 7);
            registers.L = (registers.L << 1) | s;
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x06:    // RLC HL
            unsigned char s = readByte(GET_HL());
            s = (s >> 7);
            SET_HL((GET_HL() << 1) | s);
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.cycles += 2;
            break;
        case 0x07:    // RLC A
            unsigned char s = registers.A;
            s = (s >> 7);
            registers.A = (registers.A << 1) | s;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x08:    // RRC B
            unsigned char s = registers.B;
            s = (s & 0x1);
            registers.B = (registers.B >> 1) | (s << 7);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x09:    // RRC C
            unsigned char s = registers.C;
            s = (s & 0x1);
            registers.C = (registers.C >> 1) | (s << 7);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0A:    // RRC D
            unsigned char s = registers.D;
            s = (s & 0x1);
            registers.D = (registers.D >> 1) | (s << 7);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0B:    // RRC E
            unsigned char s = registers.E;
            s = (s & 0x1);
            registers.E = (registers.E >> 1) | (s << 7);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0C:    // RRC H
            unsigned char s = registers.H;
            s = (s & 0x1);
            registers.H = (registers.H >> 1) | (s << 7);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0D:    // RRC L
            unsigned char s = registers.L;
            s = (s & 0x1);
            registers.L = (registers.L >> 1) | (s << 7);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0E:    // RRC (HL)
            unsigned char s = readByte(GET_HL());
            s = (s & 0x1);
            SET_HL((GET_HL() >> 1) | (s << 7));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.cycles += 2;
            break;
        case 0x0F:    // RRC A
            unsigned char s = registers.A;
            s = (s & 0x1);
            registers.A = (registers.A >> 1) | (s << 7);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x10:    // RL B
            unsigned char t = registers.B;
            registers.B = (registers.B << 1) | FLAG_C;
            SET_C(t >> 7);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            break;
        case 0x11:    // RL C
            unsigned char t = registers.C;
            registers.C = (registers.C << 1) | FLAG_C;
            SET_C(t >> 7);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            break;
        case 0x12:    // RL D
            unsigned char t = registers.D;
            registers.D = (registers.D << 1) | FLAG_C;
            SET_C(t >> 7);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            break;
        case 0x13:    // RL E
            unsigned char t = registers.E;
            registers.E = (registers.E << 1) | FLAG_C;
            SET_C(t >> 7);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            break;
        case 0x14:    // RL H
            unsigned char t = registers.H;
            registers.H = (registers.H << 1) | FLAG_C;
            SET_C(t >> 7);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            break;
        case 0x15:    // RL L
            unsigned char t = registers.L;
            registers.L = (registers.L << 1) | FLAG_C;
            SET_C(t >> 7);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            break;
        case 0x16:    // RL (HL)
            unsigned char t = readByte(GET_HL());
            SET_HL((GET_HL() << 1) | FLAG_C);
            SET_C(t >> 7);
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            registers.cycles += 2;
            break;
        case 0x17:    // RL A
            unsigned char t = registers.A;
            registers.A = (registers.A << 1) | FLAG_C;
            SET_C(t >> 7);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            break;
        case 0x18:    // RR B
            unsigned char t = registers.B;
            t = (t & 0x1);
            registers.B = (registers.B >> 1) | (FLAG_C << 7);
            SET_C(t);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            break;
        case 0x19:    // RR C
            unsigned char t = registers.C;
            t = (t & 0x1);
            registers.C = (registers.C >> 1) | (FLAG_C << 7);
            SET_C(t);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1A:    // RR D
            unsigned char t = registers.D;
            t = (t & 0x1);
            registers.D = (registers.D >> 1) | (FLAG_C << 7);
            SET_C(t);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1B:    // RR E
            unsigned char t = registers.E;
            t = (t & 0x1);
            registers.E = (registers.E >> 1) | (FLAG_C << 7);
            SET_C(t);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1C:    // RR H
            unsigned char t = registers.H;
            t = (t & 0x1);
            registers.H = (registers.H >> 1) | (FLAG_C << 7);
            SET_C(t);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1D:    // RR L
            unsigned char t = registers.L;
            t = (t & 0x1);
            registers.L = (registers.L >> 1) | (FLAG_C << 7);
            SET_C(t);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1E:    // RR (HL)
            unsigned char t = readByte(GET_HL());
            t = (t & 0x1);
            SET_HL((GET_HL() >> 1) | (FLAG_C << 7));
            SET_C(t);
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            registers.cycles += 2;
            break;
        case 0x1F:    // RR A
            unsigned char t = registers.A;
            t = (t & 0x1);
            registers.A = (registers.A >> 1) | (FLAG_C << 7);
            SET_C(t);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            break;
        case 0x20:    // SLA B
            unsigned char s = registers.B;
            s = (s >> 7);
            registers.B = (registers.B << 1);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x21:    // SLA C
            unsigned char s = registers.C;
            s = (s >> 7);
            registers.C = (registers.C << 1);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x22:    // SLA D
            unsigned char s = registers.D;
            s = (s >> 7);
            registers.D = (registers.D << 1);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x23:    // SLA E
            unsigned char s = registers.E;
            s = (s >> 7);
            registers.E = (registers.E << 1);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x24:    // SLA H
            unsigned char s = registers.H;
            s = (s >> 7);
            registers.H = (registers.H << 1);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x25:    // SLA L
            unsigned char s = registers.L;
            s = (s >> 7);
            registers.L = (registers.L << 1);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x26:    // SLA HL
            unsigned char s = readByte(GET_HL());
            s = (s >> 7);
            SET_HL((GET_HL() << 1));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.cycles += 2;
            break;
        case 0x27:    // SLA A
            unsigned char s = registers.A;
            s = (s >> 7);
            registers.A = (registers.A << 1);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x28:    // SRA B
            unsigned char t = registers.B;
            registers.B = (registers.B >> 1) | (registers.B & 0x80);
            SET_C(t >> 7);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            break;
        case 0x29:    // SRA C
            unsigned char t = registers.C;
            registers.C = (registers.C >> 1) | (registers.C & 0x80);
            SET_C(t >> 7);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2A:    // SRA D
            unsigned char t = registers.D;
            registers.D = (registers.D >> 1) | (registers.D & 0x80);
            SET_C(t >> 7);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2B:    // SRA E
            unsigned char t = registers.E;
            registers.E = (registers.E >> 1) | (registers.E & 0x80);
            SET_C(t >> 7);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2C:    // SRA H
            unsigned char t = registers.H;
            registers.H = (registers.H >> 1) | (registers.H & 0x80);
            SET_C(t >> 7);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2D:    // SRA L
            unsigned char t = registers.L;
            registers.L = (registers.L >> 1) | (registers.L & 0x80);
            SET_C(t >> 7);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2E:    // SRA (HL)
            unsigned char t = readByte(GET_HL());
            SET_HL((GET_HL() >> 1) | (GET_HL() & 0x80)));
            SET_C(t >> 7);
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            registers.cycles += 2;
            break;
        case 0x2F:    // SRA A
            unsigned char t = registers.A;
            registers.A = (registers.A >> 1) | (registers.A & 0x80);
            SET_C(t >> 7);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            break;
        case 0x30:    // SWAP B
            registers.B = (((registers.B & 0x0F) << 4) | ((registers.B & 0xF0) >> 4));
            SET_Z(!registers.B);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            break;
        case 0x31:    // SWAP C
            registers.C = (((registers.C & 0x0F) << 4) | ((registers.C & 0xF0) >> 4));
            SET_Z(!registers.C);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            break;
        case 0x32:    // SWAP D
            registers.D = (((registers.D & 0x0F) << 4) | ((registers.D & 0xF0) >> 4));
            SET_Z(!registers.D);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            break;
        case 0x33:    // SWAP E
            registers.E = (((registers.E & 0x0F) << 4) | ((registers.E & 0xF0) >> 4));
            SET_Z(!registers.E);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            break;
        case 0x34:    // SWAP H
            registers.H = (((registers.H & 0x0F) << 4) | ((registers.H & 0xF0) >> 4));
            SET_Z(!registers.H);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            break;
        case 0x35:    // SWAP L
            registers.L = (((registers.L & 0x0F) << 4) | ((registers.L & 0xF0) >> 4));
            SET_Z(!registers.L);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            break;
        case 0x36:    // SWAP HL
            writeByte(GET_HL(), (((GET_HL() & 0x0F) << 4) | ((GET_HL() & 0xF0) >> 4)));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H((0);
            SET_C(0);
            registers.cycles += 2;
            break;
        case 0x37:    // SWAP A
            registers.A = (((registers.A & 0x0F) << 4) | ((registers.A & 0xF0) >> 4));
            SET_Z(!registers.A);
            SET_N(0);
            SET_H((0);
            SET_C(0);
            break;
        case 0x38:    // SRL B
            unsigned char t = registers.B;
            registers.B = (registers.B >> 1);
            SET_C(t >> 7);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            break;
        case 0x39:    // SRL C
            unsigned char t = registers.C;
            registers.C = (registers.C >> 1);
            SET_C(t >> 7);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3A:    // SRL D
            unsigned char t = registers.D;
            registers.D = (registers.D >> 1);
            SET_C(t >> 7);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3B:    // SRL E
            unsigned char t = registers.E;
            registers.E = (registers.E >> 1);
            SET_C(t >> 7);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3C:    // SRL H
            unsigned char t = registers.H;
            registers.H = (registers.H >> 1);
            SET_C(t >> 7);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3D:    // SRL L
            unsigned char t = registers.L;
            registers.L = (registers.L >> 1);
            SET_C(t >> 7);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3E:    // SRL (HL)
            unsigned char t = readByte(GET_HL());
            SET_HL((GET_HL() >> 1));
            SET_C(t >> 7);
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            registers.cycles += 2;
            break;
        case 0x3F:    // SRL A
            unsigned char t = registers.A;
            registers.A = (registers.A >> 1);
            SET_C(t >> 7);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            break;
        case 0x40:    // BIT B 0
            SET_Z(!(registers.B & 0x01));
            SET_N(0);
            SET_H(1);
            break;
        case 0x41:    // BIT C 0
            SET_Z(!(registers.C & 0x01));
            SET_N(0);
            SET_H(1);
            break;
        case 0x42:    // BIT D 0
            SET_Z(!(registers.D & 0x01));
            SET_N(0);
            SET_H(1);
            break;
        case 0x43:    // BIT E 0
            SET_Z(!(registers.E & 0x01));
            SET_N(0);
            SET_H(1);
            break;
        case 0x44:    // BIT H 0
            SET_Z(!(registers.H & 0x01));
            SET_N(0);
            SET_H(1);
            break;
        case 0x45:    // BIT L 0
            SET_Z(!(registers.L & 0x01));
            SET_N(0);
            SET_H(1);
            break;
        case 0x46:    // BIT (HL) 0
            SET_Z(!(readByte(GET_HL()) & 0x01));
            SET_N(0);
            SET_H(1);
            registers.cycles += 2;
            break;
        case 0x47:    // BIT A 0
            SET_Z(!(registers.A & 0x01));
            SET_N(0);
            SET_H(1);
            break;
        case 0x48:    // BIT B 1
            SET_Z(!(registers.B & 0x02));
            SET_N(0);
            SET_H(1);
            break;
        case 0x49:    // BIT C 1
            SET_Z(!(registers.C & 0x02));
            SET_N(0);
            SET_H(1);
            break;
        case 0x4A:    // BIT D 1
            SET_Z(!(registers.D & 0x02));
            SET_N(0);
            SET_H(1);
            break;
        case 0x4B:    // BIT E 1
            SET_Z(!(registers.E & 0x02));
            SET_N(0);
            SET_H(1);
            break;
        case 0x4C:    // BIT H 1
            SET_Z(!(registers.H & 0x02));
            SET_N(0);
            SET_H(1);
            break;
        case 0x4D:    // BIT L 1
            SET_Z(!(registers.L & 0x02));
            SET_N(0);
            SET_H(1);
            break;
        case 0x4E:    // BIT (HL) 1
            SET_Z(!(readByte(GET_HL()) & 0x02));
            SET_N(0);
            SET_H(1);
            registers.cycles += 2;
            break;
        case 0x4F:    // BIT A 1
            SET_Z(!(registers.A & 0x02));
            SET_N(0);
            SET_H(1);
            break;
        case 0x50:    // BIT B 2
            SET_Z(!(registers.B & 0x04));
            SET_N(0);
            SET_H(1);
            break;
        case 0x51:    // BIT C 2
            SET_Z(!(registers.C & 0x04));
            SET_N(0);
            SET_H(1);
            break;
        case 0x52:    // BIT D 2
            SET_Z(!(registers.D & 0x04));
            SET_N(0);
            SET_H(1);
            break;
        case 0x53:    // BIT E 2
            SET_Z(!(registers.E & 0x04));
            SET_N(0);
            SET_H(1);
            break;
        case 0x54:    // BIT H 2
            SET_Z(!(registers.H & 0x04));
            SET_N(0);
            SET_H(1);
            break;
        case 0x55:    // BIT L 2
            SET_Z(!(registers.L & 0x04));
            SET_N(0);
            SET_H(1);
            break;
        case 0x56:    // BIT (HL) 2
            SET_Z(!(readByte(GET_HL()) & 0x04));
            SET_N(0);
            SET_H(1);
            registers.cycles += 2;
            break;
        case 0x57:    // BIT A 2
            SET_Z(!(registers.A & 0x04));
            SET_N(0);
            SET_H(1);
            break;
        case 0x58:    // BIT B 3
            SET_Z(!(registers.B & 0x08));
            SET_N(0);
            SET_H(1);
            break;
        case 0x59:    // BIT C 3
            SET_Z(!(registers.C & 0x08));
            SET_N(0);
            SET_H(1);
            break;
        case 0x5A:    // BIT D 3
            SET_Z(!(registers.D & 0x08));
            SET_N(0);
            SET_H(1);
            break;
        case 0x5B:    // BIT E 3
            SET_Z(!(registers.E & 0x08));
            SET_N(0);
            SET_H(1);
            break;
        case 0x5C:    // BIT H 3
            SET_Z(!(registers.H & 0x08));
            SET_N(0);
            SET_H(1);
            break;
        case 0x5D:    // BIT L 3
            SET_Z(!(registers.L & 0x08));
            SET_N(0);
            SET_H(1);
            break;
        case 0x5E:    // BIT (HL) 3
            SET_Z(!(readByte(GET_HL()) & 0x08));
            SET_N(0);
            SET_H(1);
            registers.cycles += 2;
            break;
        case 0x5F:    // BIT A 3
            SET_Z(!(registers.A & 0x08));
            SET_N(0);
            SET_H(1);
            break;
        case 0x60:    // BIT B 4
            SET_Z(!(registers.B & 0x10));
            SET_N(0);
            SET_H(1);
            break;
        case 0x61:    // BIT C 4
            SET_Z(!(registers.C & 0x10));
            SET_N(0);
            SET_H(1);
            break;
        case 0x62:    // BIT D 4
            SET_Z(!(registers.D & 0x10));
            SET_N(0);
            SET_H(1);
            break;
        case 0x63:    // BIT E 4
            SET_Z(!(registers.E & 0x10));
            SET_N(0);
            SET_H(1);
            break;
        case 0x64:    // BIT H 4
            SET_Z(!(registers.H & 0x10));
            SET_N(0);
            SET_H(1);
            break;
        case 0x65:    // BIT L 4
            SET_Z(!(registers.L & 0x10));
            SET_N(0);
            SET_H(1);
            break;
        case 0x66:    // BIT (HL) 4
            SET_Z(!(readByte(GET_HL()) & 0x10));
            SET_N(0);
            SET_H(1);
            registers.cycles += 2;
            break;
        case 0x67:    // BIT A 4
            SET_Z(!(registers.A & 0x10));
            SET_N(0);
            SET_H(1);
            break;
        case 0x68:    // BIT B 5
            SET_Z(!(registers.B & 0x20));
            SET_N(0);
            SET_H(1);
            break;
        case 0x69:    // BIT C 5
            SET_Z(!(registers.C & 0x20));
            SET_N(0);
            SET_H(1);
            break;
        case 0x6A:    // BIT D 5
            SET_Z(!(registers.D & 0x20));
            SET_N(0);
            SET_H(1);
            break;
        case 0x6B:    // BIT E 5
            SET_Z(!(registers.E & 0x20));
            SET_N(0);
            SET_H(1);
            break;
        case 0x6C:    // BIT H 5
            SET_Z(!(registers.H & 0x20));
            SET_N(0);
            SET_H(1);
            break;
        case 0x6D:    // BIT L 5
            SET_Z(!(registers.L & 0x20));
            SET_N(0);
            SET_H(1);
            break;
        case 0x6E:    // BIT (HL) 5
            SET_Z(!(readByte(GET_HL()) & 0x20));
            SET_N(0);
            SET_H(1);
            registers.cycles += 2;
            break;
        case 0x6F:    // BIT A 5
            SET_Z(!(registers.A & 0x20));
            SET_N(0);
            SET_H(1);
            break;
        case 0x70:    // BIT B 6
            SET_Z(!(registers.B & 0x40));
            SET_N(0);
            SET_H(1);
            break;
        case 0x71:    // BIT C 6
            SET_Z(!(registers.C & 0x40));
            SET_N(0);
            SET_H(1);
            break;
        case 0x72:    // BIT D 6
            SET_Z(!(registers.D & 0x40));
            SET_N(0);
            SET_H(1);
            break;
        case 0x73:    // BIT E 6
            SET_Z(!(registers.E & 0x40));
            SET_N(0);
            SET_H(1);
            break;
        case 0x74:    // BIT H 6
            SET_Z(!(registers.H & 0x40));
            SET_N(0);
            SET_H(1);
            break;
        case 0x75:    // BIT L 6
            SET_Z(!(registers.L & 0x40));
            SET_N(0);
            SET_H(1);
            break;
        case 0x76:    // BIT (HL) 6
            SET_Z(!(readByte(GET_HL()) & 0x40));
            SET_N(0);
            SET_H(1);
            registers.cycles += 2;
            break;
        case 0x77:    // BIT A 6
            SET_Z(!(registers.A & 0x40));
            SET_N(0);
            SET_H(1);
            break;
        case 0x78:    // BIT B 7
            SET_Z(!(registers.B & 0x80));
            SET_N(0);
            SET_H(1);
            break;
        case 0x79:    // BIT C 7
            SET_Z(!(registers.C & 0x80));
            SET_N(0);
            SET_H(1);
            break;
        case 0x7A:    // BIT D 7
            SET_Z(!(registers.D & 0x80));
            SET_N(0);
            SET_H(1);
            break;
        case 0x7B:    // BIT E 7
            SET_Z(!(registers.E & 0x80));
            SET_N(0);
            SET_H(1);
            break;
        case 0x7C:    // BIT H 7
            SET_Z(!(registers.H & 0x80));
            SET_N(0);
            SET_H(1);
            break;
        case 0x7D:    // BIT L 7
            SET_Z(!(registers.L & 0x80));
            SET_N(0);
            SET_H(1);
            break;
        case 0x7E:    // BIT (HL) 7
            SET_Z(!(readByte(GET_HL()) & 0x80));
            SET_N(0);
            SET_H(1);
            registers.cycles += 2;
            break;
        case 0x7F:    // BIT A 7
            SET_Z(!(registers.A & 0x80));
            SET_N(0);
            SET_H(1);
            break;
        case 0x80:    // RES B 0
            registers.B &= 0xFE;
            break;
        case 0x81:    // RES C 0
            registers.C &= 0xFE;
            break;
        case 0x82:    // RES D 0
            registers.D &= 0xFE;
            break;
        case 0x83:    // RES E 0
            registers.E &= 0xFE;
            break;
        case 0x84:    // RES H 0
            registers.H &= 0xFE;
            break;
        case 0x85:    // RES L 0
            registers.L &= 0xFE;
            break;
        case 0x86:    // RES (HL) 0
            SET_HL(GET_HL() & 0xFE);
            registers.cyles += 2;
            break;
        case 0x87:    // RES A 0
            registers.A &= 0xFE;
            break;
        case 0x88:    // RES B 1
            registers.B &= 0xFD;
            break;
        case 0x89:    // RES C 1
            registers.C &= 0xFD;
            break;
        case 0x8A:    // RES D 1
            registers.D &= 0xFD;
            break;
        case 0x8B:    // RES E 1
            registers.E &= 0xFD;
            break;
        case 0x8C:    // RES H 1
            registers.H &= 0xFD;
            break;
        case 0x8D:    // RES L 1
            registers.L &= 0xFD;
            break;
        case 0x8E:    // RES (HL) 1
            SET_HL(GET_HL() & 0xFD);
            registers.cyles += 2;
            break;
        case 0x8F:    // RES A 1
            registers.A &= 0xFD;
            break;
        case 0x90:    // RES B 2
            registers.B &= 0xFB;
            break;
        case 0x91:    // RES C 2
            registers.C &= 0xFB;
            break;
        case 0x92:    // RES D 2
            registers.D &= 0xFB;
            break;
        case 0x93:    // RES E 2
            registers.E &= 0xFB;
            break;
        case 0x94:    // RES H 2
            registers.H &= 0xFB;
            break;
        case 0x95:    // RES L 2
            registers.L &= 0xFB;
            break;
        case 0x96:    // RES (HL) 2
            SET_HL(GET_HL() & 0xFB);
            registers.cyles += 2;
            break;
        case 0x97:    // RES A 2
            registers.A &= 0xFB;
            break;
        case 0x98:    // RES B 3
            registers.B &= 0xF7;
            break;
        case 0x99:    // RES C 3
            registers.C &= 0xF7;
            break;
        case 0x9A:    // RES D 3
            registers.D &= 0xF7;
            break;
        case 0x9B:    // RES E 3
            registers.E &= 0xF7;
            break;
        case 0x9C:    // RES H 3
            registers.H &= 0xF7;
            break;
        case 0x9D:    // RES L 3
            registers.L &= 0xF7;
            break;
        case 0x9E:    // RES (HL) 3
            SET_HL(GET_HL() & 0xF7);
            registers.cyles += 2;
            break;
        case 0x9F:    // RES A 3
            registers.A &= 0xF7;
            break;
        case 0xA0:    // RES B 4
            registers.B &= 0xEF;
            break;
        case 0xA1:    // RES C 4
            registers.C &= 0xEF;
            break;
        case 0xA2:    // RES D 4
            registers.D &= 0xEF;
            break;
        case 0xA3:    // RES E 4
            registers.E &= 0xEF;
            break;
        case 0xA4:    // RES H 4
            registers.H &= 0xEF;
            break;
        case 0xA5:    // RES L 4
            registers.L &= 0xEF;
            break;
        case 0xA6:    // RES (HL) 4
            SET_HL(GET_HL() & 0xEF);
            registers.cyles += 2;
            break;
        case 0xA7:    // RES A 4
            registers.A &= 0xEF;
            break;
        case 0xA8:    // RES B 5
            registers.B &= 0xDF;
            break;
        case 0xA9:    // RES C 5
            registers.C &= 0xDF;
            break;
        case 0xAA:    // RES D 5
            registers.D &= 0xDF;
            break;
        case 0xAB:    // RES E 5
            registers.E &= 0xDF;
            break;
        case 0xAC:    // RES H 5
            registers.H &= 0xDF;
            break;
        case 0xAD:    // RES L 5
            registers.L &= 0xDF;
            break;
        case 0xAE:    // RES (HL) 5
            SET_HL(GET_HL() & 0xDF);
            registers.cyles += 2;
            break;
        case 0xAF:    // RES A 5
            registers.A &= 0xDF;
            break;
        case 0xB0:    // RES B 6
            registers.B &= 0xBF;
            break;
        case 0xB1:    // RES C 6
            registers.C &= 0xBF;
            break;
        case 0xB2:    // RES D 6
            registers.D &= 0xBF;
            break;
        case 0xB3:    // RES E 6
            registers.E &= 0xBF;
            break;
        case 0xB4:    // RES H 6
            registers.H &= 0xBF;
            break;
        case 0xB5:    // RES L 6
            registers.L &= 0xBF;
            break;
        case 0xB6:    // RES (HL) 6
            SET_HL(GET_HL() & 0xBF);
            registers.cyles += 2;
            break;
        case 0xB7:    // RES A 6
            registers.A &= 0x7F;
            break;
        case 0xB8:    // RES B 7
            registers.B &= 0x7F;
            break;
        case 0xB9:    // RES C 7
            registers.C &= 0x7F;
            break;
        case 0xBA:    // RES D 7
            registers.D &= 0x7F;
            break;
        case 0xBB:    // RES E 7
            registers.E &= 0x7F;
            break;
        case 0xBC:    // RES H 7
            registers.H &= 0x7F;
            break;
        case 0xBD:    // RES L 7
            registers.L &= 0x7F;
            break;
        case 0xBE:    // RES (HL) 7
            SET_HL(GET_HL() & 0x7F);
            registers.cyles += 2;
            break;
        case 0xBF:    // RES A 7
            registers.A &= 0x7F;
            break;
        case 0xC0:    // SET B 0
            registers.B |= 0x01;
            break;
        case 0xC1:    // SET C 0
            registers.C |= 0x01;
            break;
        case 0xC2:    // SET D 0
            registers.D |= 0x01;
            break;
        case 0xC3:    // SET E 0
            registers.E |= 0x01;
            break;
        case 0xC4:    // SET H 0
            registers.H |= 0x01;
            break;
        case 0xC5:    // SET L 0
            registers.L |= 0x01;
            break;
        case 0xC6:    // SET (HL) 0
            SET_HL(GET_HL() | 0x01);
            registers.cyles += 2;
            break;
        case 0xC7:    // SET A 0
            registers.A |= 0x01;
            break;
        case 0xC8:    // SET B 1
            registers.B |= 0x02;
            break;
        case 0xC9:    // SET C 1
            registers.C |= 0x02;
            break;
        case 0xCA:    // SET D 1
            registers.D |= 0x02;
            break;
        case 0xCB:    // SET E 1
            registers.E |= 0x02;
            break;
        case 0xCC:    // SET H 1
            registers.H |= 0x02;
            break;
        case 0xCD:    // SET L 1
            registers.L |= 0x02;
            break;
        case 0xCE:    // SET (HL) 1
            SET_HL(GET_HL() | 0x02);
            registers.cyles += 2;
            break;
        case 0xCF:    // SET A 1
            registers.A |= 0x02;
            break;
        case 0xD0:    // SET B 2
            registers.B |= 0x04;
            break;
        case 0xD1:    // SET C 2
            registers.C |= 0x04;
            break;
        case 0xD2:    // SET D 2
            registers.D |= 0x04;
            break;
        case 0xD3:    // SET E 2
            registers.E |= 0x04;
            break;
        case 0xD4:    // SET H 2
            registers.H |= 0x04;
            break;
        case 0xD5:    // SET L 2
            registers.L |= 0x04;
            break;
        case 0xD6:    // SET (HL) 2
            SET_HL(GET_HL() | 0x04);
            registers.cyles += 2;
            break;
        case 0xD7:    // SET A 2
            registers.A |= 0x04;
            break;
        case 0xD8:    // SET B 3
            registers.B |= 0x08;
            break;
        case 0xD9:    // SET C 3
            registers.C |= 0x08;
            break;
        case 0xDA:    // SET D 3
            registers.D |= 0x08;
            break;
        case 0xDB:    // SET E 3
            registers.E |= 0x08;
            break;
        case 0xDC:    // SET H 3
            registers.H |= 0x08;
            break;
        case 0xDD:    // SET L 3
            registers.L |= 0x08;
            break;
        case 0xDE:    // SET (HL) 3
            SET_HL(GET_HL() | 0x08);
            registers.cyles += 2;
            break;
        case 0xDF:    // SET A 3
            registers.A |= 0x08;
            break;
        case 0xE0:    // SET B 4
            registers.B |= 0x10;
            break;
        case 0xE1:    // SET C 4
            registers.C |= 0x10;
            break;
        case 0xE2:    // SET D 4
            registers.D |= 0x10;
            break;
        case 0xE3:    // SET E 4
            registers.E |= 0x10;
            break;
        case 0xE4:    // SET H 4
            registers.H |= 0x10;
            break;
        case 0xE5:    // SET L 4
            registers.L |= 0x10;
            break;
        case 0xE6:    // SET (HL) 4
            SET_HL(GET_HL() | 0x10);
            registers.cyles += 2;
            break;
        case 0xE7:    // SET A 4
            registers.A |= 0x10;
            break;
        case 0xE8:    // SET B 5
            registers.B |= 0x20;
            break;
        case 0xE9:    // SET C 5
            registers.C |= 0x20;
            break;
        case 0xEA:    // SET D 5
            registers.D |= 0x20;
            break;
        case 0xEB:    // SET E 5
            registers.E |= 0x20;
            break;
        case 0xEC:    // SET H 5
            registers.H |= 0x20;
            break;
        case 0xED:    // SET L 5
            registers.L |= 0x20;
            break;
        case 0xEE:    // SET (HL) 5
            SET_HL(GET_HL() | 0x20);
            registers.cyles += 2;
            break;
        case 0xEF:    // SET A 5
            registers.A |= 0x20;
            break;
        case 0xF0:    // SET B 6
            registers.B |= 0x40;
            break;
        case 0xF1:    // SET C 6
            registers.C |= 0x40;
            break;
        case 0xF2:    // SET D 6
            registers.D |= 0x40;
            break;
        case 0xF3:    // SET E 6
            registers.E |= 0x40;
            break;
        case 0xF4:    // SET H 6
            registers.H |= 0x40;
            break;
        case 0xF5:    // SET L 6
            registers.L |= 0x40;
            break;
        case 0xF6:    // SET (HL) 6
            SET_HL(GET_HL() | 0x40);
            registers.cyles += 2;
            break;
        case 0xF7:    // SET A 6
            registers.A |= 0x40;
            break;
        case 0xF8:    // SET B 7
            registers.B |= 0x80;
            break;
        case 0xF9:    // SET C 7
            registers.C |= 0x80;
            break;
        case 0xFA:    // SET D 7
            registers.D |= 0x80;
            break;
        case 0xFB:    // SET E 7
            registers.E |= 0x80;
            break;
        case 0xFC:    // SET H 7
            registers.H |= 0x80;
            break;
        case 0xFD:    // SET L 7
            registers.L |= 0x80;
            break;
        case 0xFE:    // SET (HL) 7
            SET_HL(GET_HL() | 0x80);
            registers.cyles += 2;
            break;
        case 0xFF:    // SET A 7
            registers.A |= 0x80;
            break;
    }
}
