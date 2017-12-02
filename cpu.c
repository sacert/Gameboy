#include <string.h>
#include "cpu.h"
#include "MMU.h"
#include "interrupt.h"
#include "rom.h"
#include "cbprefix.h"

struct registers registers;
static int halted = 0;

void cpuInterrupt(unsigned short address) {
    interrupt.master = 0;
    registers.SP -= 2;
    writeShort(registers.SP, registers.PC);
    registers.PC = address;
}

void cpuInit(void) {

    memset(sram, 0, sizeof(sram));
  	memset(io, 0, sizeof(io));
  	memset(vram, 0, sizeof(vram));
  	memset(oam, 0, sizeof(oam));
  	memset(wram, 0, sizeof(wram));
  	memset(hram, 0, sizeof(hram));

    interrupt.master = 1; 
    interrupt.enable = 0;
    interrupt.flags = 0;

    SET_AF(0x01B0);
  	SET_BC(0x0013);
  	SET_DE(0x00D8);
  	SET_HL(0x014D);
    registers.SP = 0xFFFE;
    registers.PC = 0x0100;
    registers.cycles = 0;

    memInit(); 
}

unsigned int getCycles(void) {
    return registers.cycles;
}

void cpuCycle(void) {

    if (halted) {
        registers.cycles += 1;
        return;
    }

    int i;
    unsigned char s;
    unsigned short t;
    unsigned int u;
    unsigned char instruction = readByte(registers.PC);

    switch (instruction) {
        case 0x00:    // NOP
            registers.PC += 1;
            registers.cycles += 1;
            break;
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
            SET_BC((GET_BC() + 1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x04:    // INC B
            registers.B += 1;
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(((registers.B & 0xF) < ((registers.B-1) & 0xF)));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x05:    // DEC B
            registers.B -= 1;
            SET_Z(!registers.B);
            SET_N(1);
            SET_H(((registers.B & 0xF) == 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x06:    // LD B,n
            registers.B = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x07:    // RLCA
            s = registers.A;
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
            writeShort(readShort(registers.PC+1), registers.SP);
            registers.PC += 3;
            registers.cycles += 5;
            break;
        case 0x09:    // ADD HL,BC
            t = GET_HL();
            SET_HL((t + GET_BC()));
            SET_N(0);
            SET_H(((GET_HL() & 0xFFF) < (t & 0xFFF)));
            SET_C(((GET_HL() & 0xFFFF) < (t & 0xFFFF)));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x0A:    // LD A,(BC)
            registers.A = readByte(GET_BC());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x0B:    // DEC BC
            SET_BC((GET_BC() - 1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x0C:    // INC C
            registers.C += 1;
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(((registers.C & 0xF) < ((registers.C-1) & 0xF)));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x0D:    // DEC C
            registers.C -= 1;
            SET_Z(!registers.C);
            SET_N(1);
            SET_H(((registers.C & 0xF) == 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x0E:    // LD C,n
            registers.C = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x0F:    // RRCA
            s = (registers.A & 0x1);
            registers.A = ((registers.A >> 1) | (s << 7));
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x10:    // STOP
            halted = 1;
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
            SET_DE((GET_DE() + 1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x14:    // INC D
            registers.D += 1;
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(((registers.D & 0xF) < ((registers.D-1) & 0xF)));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x15:    // DEC D
            registers.D -= 1;
            SET_Z(!registers.D);
            SET_N(1);
            SET_H(((registers.D & 0xF) == 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x16:    // LD D,n
            registers.D = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x17:    // RLA
            s = registers.A;
            registers.A = ((registers.A << 1) | FLAG_C);
            SET_C(s >> 7);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x18:    // JP n
            registers.PC += (signed char)readByte(registers.PC+1) + 2;
            registers.cycles += 3;
            break;
        case 0x19:    // ADD HL,DE
            t = GET_HL();
            SET_HL((t + GET_DE()));
            SET_N(0);
            SET_H(((GET_HL() & 0xFFF) < (t & 0xFFF)));
            SET_C(((GET_HL() & 0xFFFF) < (t & 0xFFFF)));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x1A:    // LD A,(DE)
            registers.A = readByte(GET_DE());
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x1B:    // DEC DE
            SET_DE((GET_DE() - 1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x1C:    // INC E
            registers.E += 1;
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(((registers.E & 0xF) < ((registers.E-1) & 0xF)));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x1D:    // DEC E
            registers.E -= 1;
            SET_Z(!registers.E);
            SET_N(1);
            SET_H(((registers.E & 0xF) == 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x1E:    // LD E,n
            registers.E = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x1F:    // RRA
            s = (registers.A & 0x1);
            registers.A = (registers.A >> 1) | (FLAG_C << 7);
            SET_C(s);
            SET_Z(0);
            SET_N(0);
            SET_H(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x20:    // JP NZ
            if (FLAG_Z == 0) {
              registers.PC += (signed char)readByte(registers.PC+1) + 2;
              registers.cycles += 3;
            } else {
              registers.PC += 2;
              registers.cycles += 2;
            }
            break;
        case 0x21:    // LD HL,nn
            SET_HL(readShort(registers.PC+1));
            registers.PC += 3;
            registers.cycles += 3;
            break;
        case 0x22:    // LDI (HL), A
            writeByte(GET_HL(),registers.A);
            SET_HL((GET_HL()+1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x23:    // INC HL
            SET_HL((GET_HL()+1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x24:    // INC H
            registers.H += 1;
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(((registers.H & 0xF) < ((registers.H-1) & 0xF)));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x25:    // DEC H
            registers.H -= 1;
            SET_Z(!registers.H);
            SET_N(1);
            SET_H(((registers.H & 0xF) == 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x26:    // LD H,n
            registers.H = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x27:    // DAA
            u = registers.A;
            if (FLAG_N) {
              if(FLAG_H)
                u = (u - 0x06)&0xFF;
              if(FLAG_C)
                u -= 0x60;
            } else {
              if(FLAG_H || (u & 0xF) > 9)
                u += 0x06;
              if(FLAG_C || u > 0x9F)
                u += 0x60;
            }
            registers.A = u;
            SET_H(0);
            SET_Z(!registers.A);
            SET_C((u >= 0x100));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x28:    // JP Z
            if (FLAG_Z == 1) {
              registers.PC += (signed char)readByte(registers.PC+1) + 2;
              registers.cycles += 3;
            } else {
              registers.PC += 2;
              registers.cycles += 2;
            }
            break;
        case 0x29:    // ADD HL,HL
            t = GET_HL() * 2;
            SET_N(0);
            SET_H(((GET_HL() & 0x7FF) > (t & 0x7FF)));
            SET_C(((GET_HL() & 0xFFFF) > (t & 0xFFFF)));
            SET_HL(t);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x2A:    // LDI A,(HL)
            registers.A = readByte(GET_HL());
            SET_HL((GET_HL()+1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x2B:    // DEC HL
            SET_HL((GET_HL() - 1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x2C:    // INC L
            registers.L += 1;
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(((registers.L & 0xF) < ((registers.L-1) & 0xF)));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x2D:    // DEC L
            registers.L -= 1;
            SET_Z(!registers.L);
            SET_N(1);
            SET_H(((registers.L & 0xF) == 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x2E:    // LD L,n
            registers.L = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0x2F:  // CPL
            registers.A = ~registers.A;
            SET_N(1);
            SET_H(1);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x30:    // JP NC
            if (FLAG_C == 0) {
              registers.PC += (signed char)readByte(registers.PC+1) + 2;
              registers.cycles += 3;
            } else {
              registers.PC += 2;
              registers.cycles += 2;
            }
            break;
        case 0x31:    // LD SP,nn
            registers.SP = readShort(registers.PC+1);
            registers.PC += 3;
            registers.cycles += 3;
            break;
        case 0x32:    // LDD (HL), A
            t = GET_HL();
            writeByte(t,registers.A);
            SET_HL((t - 1));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x33:    // INC SP
            registers.SP += 1;
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x34:    // INC (HL)
            s = readByte(GET_HL()) + 1;
            writeByte(GET_HL(), s);
            SET_Z(!s);
            SET_N(0);
            SET_H(((s & 0xF) < ((s-1) & 0xF)));
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0x35:    // DEC (HL)
            s = readByte(GET_HL()) - 1;
            writeByte(GET_HL(), s);
            SET_Z(!s);
            SET_N(1);
            SET_H(((s & 0xF) == 0xF));
            registers.PC += 1;
            registers.cycles += 3;
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
              registers.PC += (signed char)readByte(registers.PC+1) + 2;
              registers.cycles += 3;
            } else {
              registers.PC += 2;
              registers.cycles += 2;
            }
            break;
        case 0x39:    // ADD HL,SP
            t = GET_HL();
            SET_HL(t + registers.SP);
            SET_N(0);
            SET_H(((GET_HL() & 0xFFF) < (t & 0xFFF)));
            SET_C(((GET_HL() & 0xFFFF) < (t & 0xFFFF)));
            registers.PC += 1;
            registers.cycles += 2;
            break;
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
            SET_H(((registers.A & 0xF) < ((registers.A-1) & 0xF)));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x3D:    // DEC A
            registers.A -= 1;
            SET_Z(!registers.A);
            SET_N(1);
            SET_H(((registers.A & 0xF) == 0xF));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x3E:    // LD A,n
            registers.A = readByte(registers.PC+1);
            registers.PC += 2;
            registers.cycles += 2;
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
            halted = 1;
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
        case 0x7F:    // LD A,A
            registers.A = registers.A;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x80:    // ADD A,B
            i = registers.A + registers.B;
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x81:    // ADD A,C
            i = registers.A + registers.C;
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x82:    // ADD A,D
            i = registers.A + registers.D;
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x83:    // ADD A,E
            i = registers.A + registers.E;
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x84:    // ADD A,H
            i = registers.A + registers.H;
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x85:    // ADD A,L
            i = registers.A + registers.L;
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x86:    // ADD A,(HL)
            i = registers.A + readByte(GET_HL());
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x87:    // ADD A,A
            i = registers.A + registers.A;
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x88:    // ADC A,B
            i = registers.A + registers.B + FLAG_C;
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x89:    // ADC A,C
            i = ((registers.A + registers.C + FLAG_C) >= 0x100);
			SET_N(0);
			SET_H((((registers.A&0xF) + (registers.C&0xF) + FLAG_C) >= 0x10));
			registers.A = (registers.A + registers.C + FLAG_C);
			SET_C(i);
			SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8A:    // ADC A,D
            i = ((registers.A + registers.D + FLAG_C) >= 0x100);
			SET_N(0);
			SET_H((((registers.A&0xF) + (registers.D&0xF) + FLAG_C) >= 0x10));
			registers.A = (registers.A + registers.D + FLAG_C);
			SET_C(i);
			SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8B:    // ADC A,E
            i = ((registers.A + registers.E + FLAG_C) >= 0x100);
			SET_N(0);
			SET_H((((registers.A&0xF) + (registers.E&0xF) + FLAG_C) >= 0x10));
			registers.A = (registers.A + registers.E + FLAG_C);
			SET_C(i);
			SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8C:    // ADC A,H
            i = ((registers.A + registers.H + FLAG_C) >= 0x100);
			SET_N(0);
			SET_H((((registers.A&0xF) + (registers.H&0xF) + FLAG_C) >= 0x10));
			registers.A = (registers.A + registers.H + FLAG_C);
			SET_C(i);
			SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8D:    // ADC A,L
            i = ((registers.A + registers.L + FLAG_C) >= 0x100);
			SET_N(0);
			SET_H((((registers.A&0xF) + (registers.L&0xF) + FLAG_C) >= 0x10));
			registers.A = (registers.A + registers.L + FLAG_C);
			SET_C(i);
			SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x8E:    // ADC A,(HL)
            s = readByte(GET_HL());
            i = ((registers.A + s + FLAG_C) >= 0x100);
			SET_N(0);
			SET_H((((registers.A&0xF) + (s&0xF) + FLAG_C) >= 0x10));
			registers.A = (registers.A + s + FLAG_C);
			SET_C(i);
			SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x8F:    // ADC A,A
            i = ((registers.A + registers.A + FLAG_C) >= 0x100);
			SET_N(0);
			SET_H((((registers.A&0xF) + (registers.A&0xF) + FLAG_C) >= 0x10));
			registers.A = (registers.A + registers.A + FLAG_C);
			SET_C(i);
			SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x90:    // SUB A,B
            i = registers.A - registers.B;
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x91:    // SUB A,C
            i = registers.A - registers.C;
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x92:    // SUB A,D
            i = registers.A - registers.D;
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x93:    // SUB A,E
            i = registers.A - registers.E;
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x94:    // SUB A,H
            i = registers.A - registers.H;
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x95:    // SUB A,L
            i = registers.A - registers.L;
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x96:    // SUB A,(HL)
            i = registers.A - readByte(GET_HL());
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x97:    // SUB A,A
            i = registers.A - registers.A;
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x98:    // SBC A,B
            i = FLAG_C + registers.B;
            SET_H((((registers.A&0xF) - (registers.B&0xF) - FLAG_C) < 0));
			SET_C(((registers.A - registers.B - FLAG_C) < 0));
			SET_N(1);
            registers.A -= i;
            SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x99:    // SBC A,C
            i = FLAG_C + registers.C;
            SET_H((((registers.A&0xF) - (registers.C&0xF) - FLAG_C) < 0));
			SET_C(((registers.A - registers.C - FLAG_C) < 0));
			SET_N(1);
            registers.A -= i;
            SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9A:    // SBC A,D
            i = FLAG_C + registers.D;
            SET_H((((registers.A&0xF) - (registers.D&0xF) - FLAG_C) < 0));
			SET_C(((registers.A - registers.D - FLAG_C) < 0));
			SET_N(1);
            registers.A -= i;
            SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9B:    // SBC A,E
            i = FLAG_C + registers.E;
            SET_H((((registers.A&0xF) - (registers.E&0xF) - FLAG_C) < 0));
			SET_C(((registers.A - registers.E - FLAG_C) < 0));
			SET_N(1);
            registers.A -= i;
            SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9C:    // SBC A,H
            i = FLAG_C + registers.H;
            SET_H((((registers.A&0xF) - (registers.H&0xF) - FLAG_C) < 0));
			SET_C(((registers.A - registers.H - FLAG_C) < 0));
			SET_N(1);
            registers.A -= i;
            SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9D:    // SBC A,L
            i = FLAG_C + registers.L;
            SET_H((((registers.A&0xF) - (registers.L&0xF) - FLAG_C) < 0));
			SET_C(((registers.A - registers.L - FLAG_C) < 0));
			SET_N(1);
            registers.A -= i;
            SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0x9E:    // SBC A,(HL)
            s = readByte(GET_HL());
            i = FLAG_C + s;
            SET_H((((registers.A&0xF) - (s&0xF) - FLAG_C) < 0));
			SET_C(((registers.A - s - FLAG_C) < 0));
			SET_N(1);
            registers.A -= i;
            SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0x9F:    // SBC A,A
            i = FLAG_C + registers.A;
            SET_H((((registers.A&0xF) - (registers.A&0xF) - FLAG_C) < 0));
			SET_C(((registers.A - registers.A - FLAG_C) < 0));
			SET_N(1);
            registers.A -= i;
            SET_Z(!registers.A);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA0:    // AND A,B
            registers.A &= registers.B;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA1:    // AND A,C
            registers.A &= registers.C;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA2:    // AND A,D
            registers.A &= registers.D;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA3:    // AND A,E
            registers.A &= registers.E;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA4:    // AND A,H
            registers.A &= registers.H;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA5:    // AND A,L
            registers.A &= registers.L;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA6:    // AND A,(HL)
            registers.A &= readByte(GET_HL());
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xA7:    // AND A,A
            registers.A &= registers.A;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA8:    // XOR A,B
            registers.A ^= registers.B;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xA9:    // XOR A,C
            registers.A ^= registers.C;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAA:    // XOR A,D
            registers.A ^= registers.D;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAB:    // XOR A,E
            registers.A ^= registers.E;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAC:    // XOR A,H
            registers.A ^= registers.H;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAD:    // XOR A,L
            registers.A ^= registers.L;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xAE:    // XOR A,(HL)
            registers.A ^= readByte(GET_HL());
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xAF:    // XOR A,A
            registers.A ^= registers.A;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB0:    // OR A,B
            registers.A |= registers.B;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB1:    // OR A,C
            registers.A |= registers.C;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB2:    // OR A,D
            registers.A |= registers.D;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB3:    // OR A,E
            registers.A |= registers.E;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB4:    // OR A,H
            registers.A |= registers.H;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB5:    // OR A,L
            registers.A |= registers.L;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB6:    // OR A,(HL)
            registers.A |= readByte(GET_HL());
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xB7:    // OR A,A
            registers.A |= registers.A;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB8:    // CP B
            SET_Z((registers.A == registers.B));
            SET_N(1);
            SET_H((((registers.A-registers.B) & 0xF) > (registers.A & 0xF)));
            SET_C((registers.A < registers.B));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xB9:    // CP C
            SET_Z((registers.A == registers.C));
            SET_N(1);
            SET_H((((registers.A-registers.C) & 0xF) > (registers.A & 0xF)));
            SET_C((registers.A < registers.C));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBA:    // CP D
            SET_Z((registers.A == registers.D));
            SET_N(1);
            SET_H((((registers.A-registers.D) & 0xF) > (registers.A & 0xF)));
            SET_C((registers.A < registers.D));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBB:    // CP E
            SET_Z((registers.A == registers.E));
            SET_N(1);
            SET_H((((registers.A-registers.E) & 0xF) > (registers.A & 0xF)));
            SET_C((registers.A < registers.E));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBC:    // CP H
            SET_Z((registers.A == registers.H));
            SET_N(1);
            SET_H((((registers.A-registers.H) & 0xF) > (registers.A & 0xF)));
            SET_C((registers.A < registers.H));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBD:    // CP L
            SET_Z((registers.A == registers.L));
            SET_N(1);
            SET_H((((registers.A-registers.L) & 0xF) > (registers.A & 0xF)));
            SET_C((registers.A < registers.L));
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xBE:    // CP (HL)
            SET_Z((registers.A == readByte(GET_HL())));
            SET_N(1);
            SET_H((((registers.A-readByte(GET_HL())) & 0xF) > (registers.A & 0xF)));
            SET_C((registers.A < readByte(GET_HL())));
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xBF:    // CP
            SET_Z(1);
            SET_N(1);
            SET_H(0);
            SET_C(0);
            registers.PC += 1;
            registers.cycles += 1;
            break;
        case 0xC0:    // RET NZ
            if (FLAG_Z == 0) {
                registers.PC = readShort(registers.SP);
                registers.SP += 2;
                registers.cycles += 5;
            } else {
                registers.PC += 1;
                registers.cycles += 2;
            }
            break;
        case 0xC1:    // POP BC
            t = readShort(registers.SP);
            SET_BC(t);
            registers.SP += 2;
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0xC2:    // JP NZ,nn
            if (FLAG_Z == 0) {
                registers.PC = readShort(registers.PC+1);
                registers.cycles += 4;
            } else {
                registers.PC += 3;
                registers.cycles += 3;
            }
            break;
        case 0xC3:    // JP nn
            registers.PC = readShort(registers.PC+1);
            registers.cycles += 4;
            break;
        case 0xC4:    // CALL NZ,nn
            if (FLAG_Z == 0) {
                registers.SP -= 2;
                writeShort(registers.SP, registers.PC+3);
                registers.PC = readShort(registers.PC+1);
                registers.cycles += 6;
            } else {
                registers.PC += 3;
                registers.cycles += 3;
            }
            break;
        case 0xC5:    // PUSH BC
            registers.SP -= 2;
            writeShort(registers.SP, GET_BC());
            registers.PC += 1;
            registers.cycles += 4;
            break;
        case 0xC6:    // ADD A,n
            i = registers.A + readByte(registers.PC + 1);
            SET_Z(!i);
            SET_N(0);
            SET_H(((i & 0xF) < (registers.A & 0xF)));
            SET_C(((i & 0xFF) < (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xC7:    // RST 00
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+1);
            registers.PC = 0x00;
            registers.cycles += 4;
            break;
        case 0xC8:    // RET Z
            if (FLAG_Z == 1) {
                registers.PC = readShort(registers.SP);
                registers.SP += 2;
                registers.cycles += 5;
            } else {
                registers.PC += 1;
                registers.cycles += 2;
            }
            break;
        case 0xC9:    // RET
            registers.PC = readShort(registers.SP);
            registers.SP += 2;
            registers.cycles += 4;
            break;
        case 0xCA:    // JP Z,nn
            if (FLAG_Z == 1) {
                registers.PC = readShort(registers.PC+1);
                registers.cycles += 4;
            } else {
                registers.PC += 3;
                registers.cycles += 3;
            }
            break;
        case 0xCB:    // Prefix
            cbPrefix(readByte(registers.PC + 1));
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xCC:    // CALL Z,nn
            if (FLAG_Z == 1) {
                registers.SP -= 2;
                writeShort(registers.SP, registers.PC+3);
                registers.PC = readShort(registers.PC+1);
                registers.cycles += 6;
            } else {
                registers.PC += 3;
                registers.cycles += 3;
            }
            break;
        case 0xCD:    // CALL n
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+3);
            registers.PC = readShort(registers.PC+1);
            registers.cycles += 6;
            break;
        case 0xCE:    // ADC A,
            s = readByte(registers.PC + 1);
            i = registers.A + s + FLAG_C >= 0x100;
            SET_N(0);
            SET_H((((registers.A + s + FLAG_C) & 0xF) < (registers.A & 0xF)));
            registers.A = registers.A + s + FLAG_C;
            SET_C(i);
            SET_Z(!registers.A);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xCF:    // RST 08
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+1);
            registers.PC = 0x08;
            registers.cycles += 4;
            break;
        case 0xD0:    // RET NC
            if (FLAG_C == 0) {
                registers.PC = readShort(registers.SP);
                registers.SP += 2;
                registers.cycles += 5;
            } else {
                registers.PC += 1;
                registers.cycles += 2;
            }
            break;
        case 0xD1:    // POP DE
            SET_DE(readShort(registers.SP));
            registers.SP += 2;
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0xD2:    // JP NC,nn
            if (FLAG_C == 0) {
                registers.PC = readShort(registers.PC+1);
                registers.cycles += 4;
            } else {
                registers.PC += 3;
                registers.cycles += 3;
            }
            break;
        case 0xD4:    // CALL NC,nn
            if (FLAG_C == 0) {
                registers.SP -= 2;
                writeShort(registers.SP, registers.PC+3);
                registers.PC = readShort(registers.PC+1);
                registers.cycles += 6;
            } else {
                registers.PC += 3;
                registers.cycles += 3;
            }
            break;
        case 0xD5:    // PUSH DE
            registers.SP -= 2;
            writeShort(registers.SP, GET_DE());
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0xD6:    // SUB A,n
            i = registers.A - readByte(registers.PC+1);
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 2;
            registers.cycles += 4;
            break;
        case 0xD7:    // RST 10
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+1);
            registers.PC = 0x10;
            registers.cycles += 4;
            break;
        case 0xD8:    // RET C
            if (FLAG_C == 1) {
                registers.PC = readShort(registers.SP);
                registers.SP += 2;
                registers.cycles += 5;
            } else {
                registers.PC += 1;
                registers.cycles += 2;
            }
            break;
        case 0xD9:    // RET NZ
            registers.PC = readShort(registers.SP);
            registers.SP += 2;
            registers.cycles += 4;
            interrupt.master = 1;
            interrupt.pending = 1;
            break;
        case 0xDA:    // JP C,nn
            if (FLAG_C == 1) {
                registers.PC = readShort(registers.PC+1);
                registers.cycles += 4;
            } else {
                registers.PC += 3;
                registers.cycles += 3;
            }
            break;
        case 0xDC:    // CALL C,nn
            if (FLAG_C == 1) {
                registers.SP -= 2;
                writeShort(registers.SP, registers.PC+3);
                registers.PC = readShort(registers.PC+1);
                registers.cycles += 6;
            } else {
                registers.PC += 3;
                registers.cycles += 3;
            }
            break;
        case 0xDE:    // SBC, nn
            i = registers.A - (readByte(registers.PC+1) + FLAG_C);
            SET_Z(!i);
            SET_N(1);
            SET_H(((i & 0xF) > (registers.A & 0xF)));
            SET_C(((i & 0xFF) > (registers.A & 0xFF)));
            registers.A = i;
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xDF:    // RST 18
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+1);
            registers.PC = 0x0018;
            registers.cycles += 4;
            break;
        case 0xE0:    // LD ($FF00+n), A
            writeByte((0xFF00 + readByte(registers.PC+1)), registers.A);
            registers.PC += 2;
            registers.cycles += 3;
            break;
        case 0xE1:    // POP HL
            SET_HL(readShort(registers.SP));
            registers.SP += 2;
            registers.PC += 1;
            registers.cycles += 3;
            break;
        case 0xE2:    // LD A,($FF00+C)
            writeByte((0xFF00 + registers.C), registers.A);
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
            registers.A &= readByte(registers.PC+1);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(1);
            SET_C(0);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xE7:    // RST 20
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+1);
            registers.PC = 0x20;
            registers.cycles += 4;
            break;
        case 0xE8:    // ADD SP,n
            t = registers.SP;
            registers.SP += (signed char)readByte(registers.PC+1);
            SET_Z(0);
            SET_N(0);
            SET_H(((registers.SP & 0xF) < (t & 0xF)));
            SET_C(((registers.SP & 0xFF) < (t & 0xFF)));
            registers.PC += 2;
            registers.cycles += 4;
            break;
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
            registers.A ^= readByte(registers.PC+1);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xEF:    // RST 28
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+1);
            registers.PC = 0x28;
            registers.cycles += 4;
            break;
        case 0xF0:    // LD A, ($FF00+n)
            s = readByte(registers.PC+1);
            registers.A = readByte(0xFF00 + s);
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
        case 0xF3:    // DI
            registers.PC += 1;
            registers.cycles += 1;
            interrupt.master = 0;
            break;
        case 0xF5:    // PUSH AF
            registers.SP -= 2;
            writeShort(registers.SP, GET_AF());
            registers.PC += 1;
            registers.cycles += 4;
            break;
        case 0xF6:    // OR A,n
            registers.A |= readByte(registers.PC+1);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xF7:    // RST 30
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+1);
            registers.PC = 0x30;
            registers.cycles += 4;
            break;
        case 0xF8:    // LD HL, SP + n
            s = readByte(registers.PC+1);
            SET_HL(registers.SP + (signed char)s);
            SET_N(0);
            SET_Z(0);
            SET_C((((registers.SP+s)&0xFF) < (registers.SP&0xFF))); // a carry will cause a wrap around = making new value smaller
            SET_H((((registers.SP+s)&0x0F) < (registers.SP&0x0F))); // add the two, see if it becomes larger
            registers.PC += 2;
            registers.cycles += 3;
            break;
        case 0xF9:    // LD SP,HL
            registers.SP = GET_HL();
            registers.PC += 1;
            registers.cycles += 2;
            break;
        case 0xFA:    // LD A,(nn)
            t = readShort(registers.PC+1);
            registers.A = readByte(t);
            registers.PC += 3;
            registers.cycles += 4;
            break;
        case 0xFB:    // DI
            registers.PC += 1;
            registers.cycles += 1;
            interrupt.master = 1;
            interrupt.pending = 1;
            break;
        case 0xFE:    // CP n
            s = readByte(registers.PC+1);
            SET_Z((registers.A == s));
            SET_N(1);
            SET_H((((registers.A-s) & 0xF) > (registers.A & 0xF)));
            SET_C((registers.A < s));
            registers.PC += 2;
            registers.cycles += 2;
            break;
        case 0xFF:    // RST 38
            registers.SP -= 2;
            writeShort(registers.SP, registers.PC+1);
            registers.PC = 0x0038;
            registers.cycles += 4;
            break;
        default:
            printf("Instruction: %02X\n", (int)instruction);
            printf("Undefined instruction.\n");
            break;
    }   
}
