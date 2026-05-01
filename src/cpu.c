#include "cpu.h"
#include "ISA.h"
#include <stdint.h>
#include <stdlib.h>

void init_cpu(struct cpu_t *cpu)
{
  // zero registers
  cpu->a = 								0x00;
  cpu->b = 								0x00;
  cpu->c = 								0x00;
  cpu->d = 								0x00;

  // zero pointers
  cpu->ip = 							0x00;
  cpu->dp = 							0x00;
  
  // zero flags
  cpu->flags = 						0x00;
  cpu->exceptions = 				0x00;
}

int mvprintw(int y, int x, const char* fmt, ...);
extern int refresh_cycle; 
void dump_vm(struct cpu_t *cpu)
{
  mvprintw(
        0,0,
          "+-----------------+\n"
          "|RA: %2X , RB: %2X  |\n"
          "|RC: %2X , RD: %2X  |\n"
          "|-----------------|\n"
          "|IP: %2X , DP: %2X  |\n"
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



// execute a opcode that requires NO args
//
// e.g. HLT 
// but the switch won't cover opcodes that need two or more args 
void execute_opcode_imm_na(struct cpu_t *cpu, uint8_t opcode)
{
  switch (opcode)
  {
    case OP_SYSTEM_NO_OPERATION: 
    {
      cpu->ip++;
      break;
    } 

    case OP_SYSTEM_HALT_EXECUTION:
    {
      cpu->halted = 1;
      break;
    }
  }
}

// catch a tripped bit
//
// right now it's just halting
void query_expt(struct cpu_t *cpu, uint8_t exeption_bitfield)
{
  // continue as usual
  if (exeption_bitfield & B_EXPT_NONE)
  {
    // do nothing
  }

  // div by zero
  if (exeption_bitfield & B_EXPT_DIV_BY_ZERO)
    execute_opcode_imm_na(cpu, OP_SYSTEM_HALT_EXECUTION);


  // invalid opcode in general
  if (exeption_bitfield & B_EXPT_INVALID_OPCODE)
    execute_opcode_imm_na(cpu, OP_SYSTEM_HALT_EXECUTION);


  // invalid register opcode provided as arg
  if (exeption_bitfield & B_EXPT_INVALID_REGISTER_ARG)
    execute_opcode_imm_na(cpu, OP_SYSTEM_HALT_EXECUTION);


  // TODO: make it so we don't go back to ip/dp 0 if we overflow
  // ip/dp reached limit
  if (exeption_bitfield & B_EXPT_ENOMEM)
    execute_opcode_imm_na(cpu, OP_SYSTEM_HALT_EXECUTION);

  
  // halt... duh
  if (exeption_bitfield & B_EXPT_HLT)
    execute_opcode_imm_na(cpu, OP_SYSTEM_NO_OPERATION);
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
      //
      // update: changed variable names to be less ambiguous
    case OP_MOVE_REGISTER_TO_REGISTER:
    {
      // skip the opcode, get the regiter opcode, incrememnt then use again
      uint8_t reg_dest_opcode = instruction_rom[++cpu->ip];
      uint8_t reg_src_opcode = instruction_rom[++cpu->ip];

      // get the ptrs so we can deref later
      uint8_t *reg_dest_ptr = get_reg_ptr(cpu, reg_dest_opcode);
      uint8_t *reg_src_ptr  = get_reg_ptr(cpu, reg_src_opcode);

      // if it's not NULL then assign
      if (reg_dest_ptr && reg_src_ptr) {
         *reg_dest_ptr = *reg_src_ptr; 
      }
      else {
        cpu->exceptions |= B_EXPT_INVALID_REGISTER_ARG;
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
      uint8_t reg_first_opcode = instruction_rom[++cpu->ip];
      uint8_t reg_second_opcode = instruction_rom[++cpu->ip];

      uint8_t *reg_a = get_reg_ptr(cpu, reg_first_opcode);
      uint8_t *reg_b = get_reg_ptr(cpu, reg_second_opcode);
      
      // ensure both are REGISTERS
      if (reg_a && reg_b) {
        uint8_t aux = 0;
        aux = *reg_a;
        *reg_a = *reg_b;
        *reg_b = aux;
      }
      else {
        cpu->exceptions |= B_EXPT_INVALID_REGISTER_ARG;
      }
      cpu->ip++;
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
      else {
        cpu->exceptions |= B_EXPT_INVALID_REGISTER_ARG;
      }
      cpu->ip++;
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
      else {
        cpu->exceptions |= B_EXPT_INVALID_REGISTER_ARG;
      }
      cpu->ip++;
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
      else {
        cpu->exceptions |= B_EXPT_INVALID_REGISTER_ARG;
      }
      cpu->ip++;
      break;
    }



    /* ARITHMETIC */
    case OP_ARITHMETIC_ADD_REGISTERS:
    {
      uint8_t reg_dest_opcode = instruction_rom[++cpu->ip];
      uint8_t reg_second_opcode = instruction_rom[++cpu->ip];

      uint8_t* dest_reg_ptr = get_reg_ptr(cpu, reg_dest_opcode);
      uint8_t* reg_second_ptr = get_reg_ptr(cpu, reg_second_opcode);
      if (dest_reg_ptr && reg_second_ptr)
      {
        uint16_t result = (uint16_t)(*dest_reg_ptr + *reg_second_ptr);
        
        // set the carry flag if it exceeds 0xFF (8 bit unsigned interger limit) 
        // set the zero flag if it the result is 0
        cpu->flags = (result > 0xff) ? (cpu->flags | B_CARRY) : (cpu->flags & ~B_CARRY);
        cpu->flags = ((uint8_t)result == 0) ? (cpu->flags | B_ZERO) : (cpu->flags & ~B_ZERO);

        // cast the 16 bit result
        *dest_reg_ptr = (uint8_t)result;
        // go to next instruction
      }

      // set the bit for a invalid register argument
      else {
        cpu->exceptions |= B_EXPT_INVALID_REGISTER_ARG;
      }
      cpu->ip++;
      break;
    }

    default:
    {
      cpu->exceptions |= B_EXPT_INVALID_OPCODE;
      cpu->ip++;
      break;
    }
  }
}
