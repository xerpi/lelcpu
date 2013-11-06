#include <stdio.h>
#include <string.h>
#include "lelcpu.h"

// TYPE I   : oooaa0bb
// TYPE II  : oooaaiii
// TYPE III : oooiiiii

#define GET_OPCODE(x)  ((x)>>5)
#define GET_A(x)       (((x)>>3) & 0b11)
#define GET_B(x)       ((x) & 0b11)
#define GET_IMM_II(x)  ((x) & 0b111)
#define GET_IMM_III(x) ((x) & 0b11111)

void lelcpu8_init(struct lelcpu8_cpu_t *lelcpu)
{
	memset(&lelcpu->regfile, 0, sizeof(struct regfile_t));
	memset(lelcpu->memory, 0, LELCPU8_MEM_RANGE);
	lelcpu->ticks = 0;
	lelcpu->frequency = 0;
	lelcpu->status = LELCPU8_STATUS_ENABLED;
}

void lelcpu8_reset(struct lelcpu8_cpu_t *lelcpu)
{
	lelcpu8_init(lelcpu);
}

void lelcpu8_loadbin(struct lelcpu8_cpu_t *lelcpu, void *binary, int size)
{
	lelcpu8_reset(lelcpu);
	memcpy(lelcpu->memory, binary, size);
}

void lelcpu8_step(struct lelcpu8_cpu_t *lelcpu)
{
	uint8_t instr = lelcpu->memory[lelcpu->regfile.PC];
	enum lelcpu8_instr_t opcode = GET_OPCODE(instr);
	uint8_t *aa_ptr, *bb_ptr;
	
	#define INC_PC() ++lelcpu->regfile.PC
	#define GET_A_PTR() aa_ptr = &lelcpu->regfile.reg[GET_A(instr)]
	#define GET_B_PTR() bb_ptr = &lelcpu->regfile.reg[GET_B(instr)]
	#define GET_PTRS() \
			GET_A_PTR(); \
			GET_B_PTR()
	
	switch (opcode) {
	case LELCPU8_INSTR_ADD:
		GET_PTRS();
		lelcpu->regfile.Z = *aa_ptr + *bb_ptr;
		INC_PC();
		break;
	case LELCPU8_INSTR_SUB:
		GET_PTRS();
		lelcpu->regfile.Z = *aa_ptr - *bb_ptr;
		INC_PC();
		break;
	case LELCPU8_INSTR_MOV:
		GET_PTRS();
		*aa_ptr = *bb_ptr;
		INC_PC();
		break;
	case LELCPU8_INSTR_LI:
		GET_A_PTR();
		*aa_ptr = GET_IMM_II(instr);
		INC_PC();
		break;
	case LELCPU8_INSTR_JMP:
		GET_PTRS();
		lelcpu->regfile.Z += GET_IMM_III(instr);
		INC_PC();
		break;
	case LELCPU8_INSTR_AND:
		GET_PTRS();
		lelcpu->regfile.Z = *aa_ptr & *bb_ptr;
		INC_PC();
		break;
	case LELCPU8_INSTR_OR:
		GET_PTRS();
		lelcpu->regfile.Z = *aa_ptr | *bb_ptr;
		INC_PC();
		break;
	case LELCPU8_INSTR_SYSCALL:
		switch (GET_IMM_III(instr))
		{
		case 31: //Halt syscall
			lelcpu->status = LELCPU8_STATUS_HALTED;
			break;
		}
		INC_PC();
		break;
	default:
		printf("Unknown opcode\n");
	}
	
	++lelcpu->ticks;
}

void lelcpu8_run(struct lelcpu8_cpu_t *lelcpu)
{
	while (lelcpu->status == LELCPU8_STATUS_ENABLED) {
		lelcpu8_step(lelcpu);
		printf("Step %i:\n", lelcpu->ticks);
		lelcpu8_print_regfile(lelcpu);
	}
}


void lelcpu8_print_regfile(struct lelcpu8_cpu_t *lelcpu)
{
	printf("    X : 0x%04X\n", lelcpu->regfile.X);
	printf("    Y : 0x%04X\n", lelcpu->regfile.Y);
	printf("    Z : 0x%04X\n", lelcpu->regfile.Z);
	printf("    PC: 0x%04X\n", lelcpu->regfile.PC);
	if (lelcpu->status == LELCPU8_STATUS_HALTED) {
		printf("\nHALTED!!\n");
	}
}
