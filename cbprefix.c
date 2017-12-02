#include "cpu.h"
#include "MMU.h"

void cbPrefix(unsigned char inst) {
    
    unsigned char s;
    unsigned char instruction = inst;

    switch (instruction) {
        case 0x00:    // RLC B
            s = (registers.B >> 7);
            registers.B = (registers.B << 1) | s;
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x01:    // RLC C
            s = (registers.C >> 7);
            registers.C = (registers.C << 1) | s;
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x02:    // RLC D
            s = (registers.D >> 7);
            registers.D = (registers.D << 1) | s;
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x03:    // RLC E
            s = (registers.E >> 7);
            registers.E = (registers.E << 1) | s;
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x04:    // RLC H
            s = (registers.H >> 7);
            registers.H = (registers.H << 1) | s;
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x05:    // RLC L
            s = (registers.L >> 7);
            registers.L = (registers.L << 1) | s;
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x06:    // RLC (HL)
            s = (readByte(GET_HL()) >> 7);
            writeByte(GET_HL(), (((readByte(GET_HL()) << 1) | s)));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.cycles += 2;
            break;
        case 0x07:    // RLC A
            s = (registers.A >> 7);
            registers.A = (registers.A << 1) | s;
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x08:    // RRC B
            s = (registers.B & 0x1);
            registers.B = (registers.B >> 1) | (s << 7);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x09:    // RRC C
            s = (registers.C & 0x1);
            registers.C = (registers.C >> 1) | (s << 7);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0A:    // RRC D
            s = (registers.D & 0x1);
            registers.D = (registers.D >> 1) | (s << 7);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0B:    // RRC E
            s = (registers.E & 0x1);
            registers.E = (registers.E >> 1) | (s << 7);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0C:    // RRC H
            s = (registers.H & 0x1);
            registers.H = (registers.H >> 1) | (s << 7);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0D:    // RRC L
            s = (registers.L & 0x1);
            registers.L = (registers.L >> 1) | (s << 7);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x0E:    // RRC (HL)
            s = (readByte(GET_HL()) & 0x1);
            writeByte(GET_HL(), ((readByte(GET_HL()) << 1) | (s)));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.cycles += 2;
            break;
        case 0x0F:    // RRC A
            s = (registers.A & 0x1);
            registers.A = (registers.A >> 1) | (s << 7);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x10:    // RL B
            s = registers.B;
            registers.B = (registers.B << 1) | FLAG_C;
            SET_C(s >> 7);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            break;
        case 0x11:    // RL C
            s = registers.C;
            registers.C = (registers.C << 1) | FLAG_C;
            SET_C(s >> 7);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            break;
        case 0x12:    // RL D
            s = registers.D;
            registers.D = (registers.D << 1) | FLAG_C;
            SET_C(s >> 7);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            break;
        case 0x13:    // RL E
            s = registers.E;
            registers.E = (registers.E << 1) | FLAG_C;
            SET_C(s >> 7);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            break;
        case 0x14:    // RL H
            s = registers.H;
            registers.H = (registers.H << 1) | FLAG_C;
            SET_C(s >> 7);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            break;
        case 0x15:    // RL L
            s = registers.L;
            registers.L = (registers.L << 1) | FLAG_C;
            SET_C(s >> 7);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            break;
        case 0x16:    // RL (HL)
            s = readByte(GET_HL()) >> 7;
            writeByte(GET_HL(), ((readByte(GET_HL()) << 1) | (FLAG_C)));
            SET_C((s));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            registers.cycles += 2;
            break;
        case 0x17:    // RL A
            s = registers.A;
            registers.A = (registers.A << 1) | FLAG_C;
            SET_C((s >> 7));
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            break;
        case 0x18:    // RR B
            s = (registers.B & 0x1);
            registers.B = (registers.B >> 1) | (FLAG_C << 7);
            SET_C(s);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            break;
        case 0x19:    // RR C
            s = (registers.C & 0x1);
            registers.C = (registers.C >> 1) | (FLAG_C << 7);
            SET_C(s);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1A:    // RR D
            s = (registers.D & 0x1);
            registers.D = (registers.D >> 1) | (FLAG_C << 7);
            SET_C(s);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1B:    // RR E
            s = (registers.E & 0x1);
            registers.E = (registers.E >> 1) | (FLAG_C << 7);
            SET_C(s);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1C:    // RR H
            s = (registers.H & 0x1);
            registers.H = (registers.H >> 1) | (FLAG_C << 7);
            SET_C(s);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1D:    // RR L
            s = (registers.L & 0x1);
            registers.L = (registers.L >> 1) | (FLAG_C << 7);
            SET_C(s);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            break;
        case 0x1E:    // RR (HL)
            s = (readByte(GET_HL()) & 0x1);
            writeByte(GET_HL(), ((readByte(GET_HL()) >> 1) | (FLAG_C << 7)));
            SET_C(s);
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            registers.cycles += 2;
            break;
        case 0x1F:    // RR A
            s = (registers.A & 0x1);
            registers.A = (registers.A >> 1) | (FLAG_C << 7);
            SET_C(s);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            break;
        case 0x20:    // SLA B
            s = (registers.B >> 7);
            registers.B = (registers.B << 1);
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x21:    // SLA C
            s = (registers.C >> 7);
            registers.C = (registers.C << 1);
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x22:    // SLA D
            s = (registers.D >> 7);
            registers.D = (registers.D << 1);
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x23:    // SLA E
            s = (registers.E >> 7);
            registers.E = (registers.E << 1);
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x24:    // SLA H
            s = (registers.H >> 7);
            registers.H = (registers.H << 1);
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x25:    // SLA L
            s = (registers.L >> 7);
            registers.L = (registers.L << 1);
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x26:    // SLA HL
            s = (readByte(GET_HL()) >> 7);
            writeByte(GET_HL(), ((readByte(GET_HL()) << 1)));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            SET_C(s);
            registers.cycles += 2;
            break;
        case 0x27:    // SLA A
            s = (registers.A >> 7);
            registers.A = (registers.A << 1);
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(s);
            break;
        case 0x28:    // SRA B
            s = registers.B;
            registers.B = (registers.B >> 1) | (registers.B & 0x80);
            SET_C((s & 1));
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            break;
        case 0x29:    // SRA C
            s = registers.C;
            registers.C = (registers.C >> 1) | (registers.C & 0x80);
            SET_C((s & 1));
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2A:    // SRA D
            s = registers.D;
            registers.D = (registers.D >> 1) | (registers.D & 0x80);
            SET_C((s & 1));
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2B:    // SRA E
            s = registers.E;
            registers.E = (registers.E >> 1) | (registers.E & 0x80);
            SET_C((s & 1));
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2C:    // SRA H
            s = registers.H;
            registers.H = (registers.H >> 1) | (registers.H & 0x80);
            SET_C((s & 1));
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2D:    // SRA L
            s = registers.L;
            registers.L = (registers.L >> 1) | (registers.L & 0x80);
            SET_C((s & 1));
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            break;
        case 0x2E:    // SRA (HL)
            s = readByte(GET_HL()) & 1;
            writeByte(GET_HL(), ((readByte(GET_HL()) >> 1) | (s)));
            SET_C((s));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            registers.cycles += 2;
            break;
        case 0x2F:    // SRA A
            s = registers.A;
            registers.A = (registers.A >> 1) | (registers.A & 0x80);
            SET_C((s & 1));
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            break;
        case 0x30:    // SWAP B
            registers.B = (((registers.B & 0x0F) << 4) | ((registers.B & 0xF0) >> 4));
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            break;
        case 0x31:    // SWAP C
            registers.C = (((registers.C & 0x0F) << 4) | ((registers.C & 0xF0) >> 4));
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            break;
        case 0x32:    // SWAP D
            registers.D = (((registers.D & 0x0F) << 4) | ((registers.D & 0xF0) >> 4));
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            break;
        case 0x33:    // SWAP E
            registers.E = (((registers.E & 0x0F) << 4) | ((registers.E & 0xF0) >> 4));
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            break;
        case 0x34:    // SWAP H
            registers.H = (((registers.H & 0x0F) << 4) | ((registers.H & 0xF0) >> 4));
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            break;
        case 0x35:    // SWAP L
            registers.L = (((registers.L & 0x0F) << 4) | ((registers.L & 0xF0) >> 4));
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            break;
        case 0x36:    // SWAP HL
            writeByte(GET_HL(), (((GET_HL() & 0x0F) << 4) | ((GET_HL() & 0xF0) >> 4)));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            SET_C(0);
            registers.cycles += 2;
            break;
        case 0x37:    // SWAP A
            registers.A = (((registers.A & 0x0F) << 4) | ((registers.A & 0xF0) >> 4));
            SET_Z(!registers.A);
            SET_N(0);
            SET_H(0);
            SET_C(0);
            break;
        case 0x38:    // SRL B
            s = registers.B & 1;
            registers.B = (registers.B >> 1);
            SET_C((s));
            SET_Z(!registers.B);
            SET_N(0);
            SET_H(0);
            break;
        case 0x39:    // SRL C
            s = registers.C & 1;
            registers.C = (registers.C >> 1);
            SET_C((s));
            SET_Z(!registers.C);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3A:    // SRL D
            s = registers.D & 1;
            registers.D = (registers.D >> 1);
            SET_C((s));
            SET_Z(!registers.D);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3B:    // SRL E
            s = registers.E & 1;
            registers.E = (registers.E >> 1);
            SET_C((s));
            SET_Z(!registers.E);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3C:    // SRL H
            s = registers.H & 1;
            registers.H = (registers.H >> 1);
            SET_C((s));
            SET_Z(!registers.H);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3D:    // SRL L
            s = registers.L & 1;
            registers.L = (registers.L >> 1);
            SET_C((s));
            SET_Z(!registers.L);
            SET_N(0);
            SET_H(0);
            break;
        case 0x3E:    // SRL (HL)
            s = readByte(GET_HL()) & 1;
            writeByte(GET_HL(), ((readByte(GET_HL()) >> 1)));
            SET_C((s));
            SET_Z(!GET_HL());
            SET_N(0);
            SET_H(0);
            registers.cycles += 2;
            break;
        case 0x3F:    // SRL A
            s = registers.A & 1;
            registers.A = (registers.A >> 1);
            SET_C((s));
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
            writeByte(GET_HL(), (readByte(GET_HL()) & 0xFE));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) & 0xFD));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) & 0xFB));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) & 0xF7));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) & 0xEF));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) & 0xDF));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) & 0xBF));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) & 0x7F));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) | 0x01));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) | 0x02));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) | 0x04));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) | 0x08));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) | 0x10));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) | 0x20));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) | 0x40));
            registers.cycles += 2;
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
            writeByte(GET_HL(), (readByte(GET_HL()) | 0x80));
            registers.cycles += 2;
            break;
        case 0xFF:    // SET A 7
            registers.A |= 0x80;
            break;
    }
}
