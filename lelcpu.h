#ifndef _LELCPU_H_
#define _LELCPU_H_

#include <stdint.h>

#define LELCPU8_MEM_RANGE (0xFF)

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
000  ADD       // Z = aa + bb
001  SUB       // Z = aa - bb
010  MOV       // aa = bb
011	 LI        // aa = iii
100  JMP       // PC += iiiii
101  AND       // Z = aa & bb
110  OR        // Z = aa | bb
111  SYSCALL   // syscall(iiiii)
*/

/* SYSCALLS
32 | 11111 | Halt
*/

enum lelcpu8_instr_t {
	LELCPU8_INSTR_ADD     = 0b000,
	LELCPU8_INSTR_SUB     = 0b001,
	LELCPU8_INSTR_MOV     = 0b010,
	LELCPU8_INSTR_LI      = 0b011,
	LELCPU8_INSTR_JMP     = 0b100,
	LELCPU8_INSTR_AND     = 0b101,
	LELCPU8_INSTR_OR      = 0b110,
	LELCPU8_INSTR_SYSCALL = 0b111,
};

enum lelcpu8_status_t {
	LELCPU8_STATUS_HALTED,
	LELCPU8_STATUS_ENABLED,
};

struct regfile_t {
	union {
		struct {
			uint8_t  X;
			uint8_t  Y;
			uint8_t  Z;
			uint8_t PC;
		};
		uint8_t reg[4];
	};
};


struct lelcpu8_cpu_t {
	struct regfile_t regfile;
	uint32_t ticks;
	uint32_t frequency;
	enum lelcpu8_status_t status;
	uint8_t memory[LELCPU8_MEM_RANGE];
};


void lelcpu8_init(struct lelcpu8_cpu_t *lelcpu);
void lelcpu8_reset(struct lelcpu8_cpu_t *lelcpu);
void lelcpu8_loadbin(struct lelcpu8_cpu_t *lelcpu, void *binary, int size);
void lelcpu8_step(struct lelcpu8_cpu_t *lelcpu);
void lelcpu8_run(struct lelcpu8_cpu_t *lelcpu);

// Debug functions
void lelcpu8_print_regfile(struct lelcpu8_cpu_t *lelcpu);


#endif
