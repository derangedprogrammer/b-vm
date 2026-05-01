#ifndef CPU_H
#define CPU_H

#include <stdint.h>

#define B_CARRY   0x01
#define B_PARITY  0x02
#define B_AC      0x04
#define B_ZERO    0x08


// exeption bits
//
#define B_EXPT_NONE                         0x00
#define B_EXPT_DIV_BY_ZERO                  0x01
#define B_EXPT_INVALID_OPCODE               0x02
#define B_EXPT_INVALID_REGISTER_ARG         0x04
#define B_EXPT_ENOMEM                       0x08
#define B_EXPT_HLT                          0x10

#define MAX_EXCEPTIONS                       6
struct cpu_t
{
    // registers
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;

    // instruction and stack ptr
    uint8_t ip;
    uint8_t dp;

    // for stuff like the carry flag
  uint8_t flags;
  uint8_t exceptions;
  uint8_t halted;
};

#define ROM_SIZE 256 
#define RAM_SIZE 256

void init_cpu(struct cpu_t *cpu);
void dump_vm(struct cpu_t *cpu);
void handle_opcode(struct cpu_t *cpu, uint8_t opcode, uint8_t *instruction_rom, uint8_t* data_ram);

#endif
