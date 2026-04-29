#include <stdlib.h>

#include "cpu.h"

int main(int argc, char* argv[])
{
  struct cpu_t mcpu_obj = {
    // registers A-D
    0x00, 0x00, 0x00, 0x00,

    // IP and SP
    0x00, 0x00,

    // flags and halted
    0x00, 0x00,
  };
  
  struct cpu_t *mcpu = &mcpu_obj;

  init_cpu(mcpu);
  dump_vm(mcpu);
  return EXIT_SUCCESS;
}
