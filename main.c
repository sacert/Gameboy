#include "rom.h"
#include "cpu.h"

int main(int argc, char* argv[]) {

    romInit(argv[1]); // loads rom
    cpuInit(); // initialize registers and memory

    while (1) {
        cpuCycle();
    }

}
