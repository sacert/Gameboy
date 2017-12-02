#define SET_AF(x) do {registers.A = ((x & 0xFF00) >> 8); registers.F = (x&0x00FF);} while(0) // multi-line macro
#define SET_BC(x) do {registers.B = ((x & 0xFF00) >> 8); registers.C = (x&0x00FF);} while(0)
#define SET_DE(x) do {registers.D = ((x & 0xFF00) >> 8); registers.E = (x&0x00FF);} while(0)
#define SET_HL(x) do {registers.H = ((x & 0xFF00) >> 8); registers.L = (x&0x00FF);} while(0)

#define GET_AF() ((registers.A << 8) | registers.F)
#define GET_BC() ((registers.B << 8) | registers.C)
#define GET_DE() ((registers.D << 8) | registers.E)
#define GET_HL() ((registers.H << 8) | registers.L)

#define SET_Z(x) registers.F = ((registers.F & 0x7F) | (x << 7))
#define SET_N(x) registers.F = ((registers.F & 0xBF) | (x << 6))
#define SET_H(x) registers.F = ((registers.F & 0xDF) | (x << 5))
#define SET_C(x) registers.F = ((registers.F & 0xEF) | (x << 4))

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


extern struct registers registers;

void cpuInterrupt(unsigned short address);
void cpuInit(void);
unsigned int getCycles(void);
void cpuCycle(void);
void cbPrefix(unsigned char inst);
