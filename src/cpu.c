#include "cpu.h"
#include "ISA.h"
#include <stdint.h>
#include <stdlib.h>

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

int mvprintw(int y, int x, const char* fmt, ...);
extern int refresh_cycle; 
void dump_vm(struct cpu_t *cpu)
{
  mvprintw(
        0,0,
          "+-----------------+\n"
          "|RA: %2x , RB: %2x  |\n"
          "|RC: %2x , RD: %2x  |\n"
          "|-----------------|\n"
          "|IP: %2x , SP: %2x  |\n"
          "+-----------------+\n"
          "FLAGS: %x, HALTED?: %d PROGRAM CYCLE: %d\n\n",
         cpu->a, cpu->b, cpu->c, cpu->d,
         cpu->ip, cpu->sp, cpu->flags, cpu->halted, refresh_cycle);
}

uint8_t get_opcode(struct cpu_t *cpu, uint8_t *mem)
{
  // we don't need to bounds check because a byte wraps to the min
  uint8_t byte = mem[cpu->ip];
  return byte;
}

uint8_t* get_reg_ptr(struct cpu_t *cpu, uint8_t reg_opcode) {
    switch (reg_opcode) {
    case REG_A: return &(cpu->a);
    case REG_B: return &(cpu->b);
    case REG_C: return &(cpu->c);
        default:    return NULL; // Handle error
    }
}

void handle_opcode(struct cpu_t *cpu, uint8_t opcode, uint8_t *mem)
{
  switch (opcode)
    {

    // NOP
    case OP_SYSTEM_NO_OPERATION:
      cpu->ip++;
      break;

    // HLT
    case OP_SYSTEM_HALT_EXECUTION:
      cpu->halted = 1;
      break;


      // MOV
    case OP_MOVE_REGISTER_TO_REGISTER:
    {
      cpu->ip++;
      uint8_t dest_opcode = mem[cpu->ip];
    
      cpu->ip++;
      uint8_t src_opcode = mem[cpu->ip];

      // get the ptrs so we can deref later
      uint8_t *dest_ptr = get_reg_ptr(cpu, dest_opcode);
      uint8_t *src_ptr  = get_reg_ptr(cpu, src_opcode);

      // if it's not NULL then assign
      if (dest_ptr && src_ptr) {
         *dest_ptr = *src_ptr; 
      }
      cpu->ip++;
      break;
    }

    case OP_SWAP_REGISTER_VALUES:
    {
      uint8_t val_a = mem[++cpu->ip];
      uint8_t val_b = mem[++cpu->ip];

      uint8_t *reg_a = get_reg_ptr(cpu, val_a);
      uint8_t *reg_b = get_reg_ptr(cpu,val_b);
      
      // ensure both are REGISTERS
      if (reg_a && reg_b) {
        uint8_t aux = 0;
        aux = *reg_a;
        *reg_a = *reg_b;
        *reg_b = aux;
      }
      break;
    }
      // LDI
    case OP_LOAD_IMMEDIATE_VALUE:
    {
      // get opcode for register and immediate value
      uint8_t dest_opcode = mem[++cpu->ip];
      uint8_t immediate_value = mem[++cpu->ip];

      uint8_t *dest_ptr = get_reg_ptr(cpu, dest_opcode);
      if (dest_ptr)
      {
        *dest_ptr = immediate_value;
      }
      cpu->ip++;
      break;
    }

  }
}
