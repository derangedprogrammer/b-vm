#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "ISA.h"
#include "cpu.h"

// HEY YOU! YES YOU!
// comment this block out and handle_opcode() in the 
// looop in the if stmt in main() if you don't want a demo
//
// mem

uint8_t program_rom[ROM_SIZE] = {
  OP_LOAD_IMMEDIATE_VALUE, REG_A, 0x62,
  OP_LOAD_IMMEDIATE_VALUE, REG_B, 0x68,
  OP_LOAD_IMMEDIATE_VALUE, REG_C, 0x64,
  OP_SWAP_REGISTER_VALUES, REG_C, REG_A,
  OP_SYSTEM_HALT_EXECUTION,
};

uint8_t data_ram[256] = {0};

struct cpu_t mcpu_obj = {
  // registers A-D
  0x00, 0x00, 0x00, 0x00,

  // IP and SP
  0x00, 0x00,

  // flags and exeptions bitfields 
  0x00, 0x00,

  // halted?
  0
};

struct cpu_t *mcpu = &mcpu_obj;
int refresh_cycle = 0;

extern inst_t* dispatch_table_ptr;
int main(int argc, char* argv[])
{
  init_cpu(mcpu);
  cbreak();
  initscr();
  noecho();
  nodelay(stdscr,TRUE);

  static int sleep_delay = 0;
  refresh_cycle = 0;
  if (argc > 1)
  {
    sleep_delay = atoi(argv[1]);
  }
  else {
    sleep_delay = 1;
  }

  static int current_key = 0;
  while (TRUE)
  {
    current_key = getch();
    if (current_key == 'q')
      break;

    // comment this out if you don't want a demo
    // otherwise execute whatever was in ram until we hit a HLT instruction
    if (!mcpu->halted)
      handle_opcode(
          mcpu,                           // cpu 
          program_rom[mcpu->ip],          // current instruction the handle will handle
          dispatch_table_ptr,             // the pointer to the dispatch table address
          program_rom,                    // instructions
          data_ram                        // data
      );

    // prompt
    dump_vm(mcpu);
    printw("Press 'q' to Exit\n"
          "Delay for each cycle: %ds\n\n", sleep_delay);
    printw("(bvm) ");
    refresh_cycle++;

    refresh();
    sleep(sleep_delay);
  }
  endwin();
  return EXIT_SUCCESS;
}
