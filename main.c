#include <stdio.h>
#include <string.h>
#include "lelcpu.h"

// TYPE I   : oooaa0bb
// TYPE II  : oooaaiii
// TYPE III : oooiiiii

/* REGISTERS
00  X
01  Y
10  Z
11  PC
*/

/* OPCODES
000  ADD       // Z = aa + bb     | I
001  SUB       // Z = aa - bb     | I
010  MOV       // aa = bb         | I
011	 LI        // aa = iii        | II
100  JMP       // PC += iiiii     | III
101  AND       // Z = aa & bb     | I
110  OR        // Z = aa | bb     | I
111  SYSCALL   // syscall(iiiii)  | III
*/

uint8_t bin[] = 
{
	0b01100110, //LI X, 0b110      # X = 0b110
	0b01101001, //LI Y, 0b001      # Y = 0b001
	0b11000001, //AND X, Y         # Z = X & Y
	0b11111111  //SYSCALL 31       # Halt the CPU
};

int main(int argc, char *argv[])
{
	struct lelcpu8_cpu_t lelcpu;
	
	lelcpu8_init(&lelcpu);
	lelcpu8_loadbin(&lelcpu, (void *)bin, sizeof(bin));
	
	lelcpu8_run(&lelcpu);

	return 0;
}
