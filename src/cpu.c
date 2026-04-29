#include "cpu.h"
#include "ISA.h"
#include <stdint.h>
#include <stdio.h>

void init_cpu(struct cpu_t *cpu)
{
  // zero registers
  cpu->a = 0x0;
  cpu->b = 0x0;
  cpu->c = 0x0;
  cpu->d = 0x0;

  // zero pointers
  cpu->ip = 0x0;
  cpu->sp = 0x0;
  
  // zero flags
  cpu->flags = 0x0;
}

void dump_vm(struct cpu_t *cpu) {
  printf("\nRA: %x , RB: %x\n"
         "RC: %x , RD: %x\n\n"
         "IP: %x , SP: %x\n\n"
         "FLAGS: %x, HALTED?: %d\n\n",
         cpu->a, cpu->b, cpu->c, cpu->d,
         cpu->ip, cpu->sp, cpu->flags, cpu->halted);
}

void handle_opcode(struct cpu_t *cpu, uint8_t opcode)
{
  switch (opcode) {

    // NOP
  case OP_SYSTEM_NO_OPERATION:
    cpu->ip++;
    break;

    // HLT
  case OP_SYSTEM_HALT_EXECUTION:
    cpu->halted = 1;
    break;
  }
}
