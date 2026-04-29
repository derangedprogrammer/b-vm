#include "cpu.h"

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
