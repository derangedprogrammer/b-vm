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
  cpu->dp = 0x0;
  
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
          "|IP: %2x , DP: %2x  |\n"
          "+-----------------+\n"
          "FLAGS: %x, HALTED?: %d PROGRAM CYCLE: %d\n\n",
         cpu->a, cpu->b, cpu->c, cpu->d,
         cpu->ip, cpu->dp, cpu->flags, cpu->halted, refresh_cycle);
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
    case REG_D: return &(cpu->d);
        default:    return NULL; // Handle error
    }
}

void handle_opcode(struct cpu_t *cpu, uint8_t opcode, uint8_t *instruction_rom, uint8_t *data_ram)
{
  switch (opcode)
    {

    // NOP
    // do nothing and just increment the instruction ptr
    //
    // NOP
    case OP_SYSTEM_NO_OPERATION: cpu->ip++; break;

    // HLT
    // stop execution 
    //
    // HLT
    case OP_SYSTEM_HALT_EXECUTION:
      cpu->ip++;
      cpu->halted = 1;
      break;


      // MOV
      // copy data from one register to another
      //
      // MOV REG, REG
    case OP_MOVE_REGISTER_TO_REGISTER:
    {
      // skip the opcode, get the regiter opcode, incrememnt then use again
      uint8_t reg_dest_opcode = instruction_rom[++cpu->ip];
      uint8_t reg_src_opcode = instruction_rom[++cpu->ip];

      // get the ptrs so we can deref later
      uint8_t *dest_ptr = get_reg_ptr(cpu, reg_dest_opcode);
      uint8_t *src_ptr  = get_reg_ptr(cpu, reg_src_opcode);

      // if it's not NULL then assign
      if (dest_ptr && src_ptr) {
         *dest_ptr = *src_ptr; 
      }
      cpu->ip++;
      break;
    }

    // XCHG
    // swap the contents of register a and b 
    //
    // XCHG REG, REG
    case OP_SWAP_REGISTER_VALUES:
    {
      uint8_t val_a = instruction_rom[++cpu->ip];
      uint8_t val_b = instruction_rom[++cpu->ip];

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
      // place a constant (second arg) into the first arg (register)
    case OP_LOAD_IMMEDIATE_VALUE:
    {
      // get opcode for register and immediate value
      uint8_t dest_opcode = instruction_rom[++cpu->ip];
      uint8_t immediate_value = instruction_rom[++cpu->ip];

      uint8_t *dest_ptr = get_reg_ptr(cpu, dest_opcode);
      if (dest_ptr)
      {
        *dest_ptr = immediate_value;
      }
      cpu->ip++;
      break;
    }

      // LD (load from memory)
      // take the second arg's ptr and get its value and put it in the register 
      //
      // LD REG, PTR
    case OP_LOAD_FROM_MEMORY_ADDRESS:
    {
      uint8_t dest_opcode = instruction_rom[++cpu->ip];
      uint8_t reg_data_pointed_to = instruction_rom[++cpu->ip];

      // use the ptr as an index to the memory and fetch the value from that address and load it on the register
      uint8_t *reg_recipient = get_reg_ptr(cpu, dest_opcode);
      if (reg_recipient)
      {
        *reg_recipient = data_ram[reg_data_pointed_to];
      }
      break;
    }

      // ST (store) 
      // take the second arg as a data register and the first arg as a ptr and store the data to where it's pointing
      //
      // ST PTR, REG
    case OP_STORE_TO_MEMORY_ADDRESS:
    {
      uint8_t dest_ptr = instruction_rom[++cpu->ip];
      uint8_t reg_opcode = instruction_rom[++cpu->ip];

      uint8_t *reg_ptr = get_reg_ptr(cpu, reg_opcode);
      if (reg_ptr)
      {
        data_ram[dest_ptr] = *reg_ptr;
        // cpu->dp++;     // no auto inc side effects?
      }
      break;
    }

    // LDR (load from register)
    // takes the second arg and treats it as a ptr and places the data the pointer points to and places it 
    // the register provided in arg1
    //
    // LDR REG_DATA, REG_PTR
    case OP_LOAD_FROM_MEMORY_INDIRECT:
    {
      uint8_t reg_dest_opcode = instruction_rom[++cpu->ip];
      uint8_t reg_src_opcode = instruction_rom[++cpu->ip];

      uint8_t* dest_reg_ptr = get_reg_ptr(cpu, reg_dest_opcode);
      uint8_t* src_reg_ptr = get_reg_ptr(cpu, reg_src_opcode);
      
      if (dest_reg_ptr && src_reg_ptr) 
      {
        *dest_reg_ptr = data_ram[*src_reg_ptr];
      }
      cpu->ip++;
      break;
    }
  }
}
