struct timer {
    unsigned int div;   // divider
    unsigned int tima;  // timer counter
    unsigned int tma;   // timer module
    unsigned char tac;  // timer controller
    unsigned int speed;
    unsigned int started; 
    unsigned int tick;
} timer;

void setDiv(unsigned char address);
void setTima(unsigned char address);
void setTma(unsigned char address);
void setTac(unsigned char address);

unsigned int getDiv(void);
unsigned int getTima(void); 
unsigned int getTma(void);
unsigned int getTac(void);

void tick(void);
void timerCycle(void);