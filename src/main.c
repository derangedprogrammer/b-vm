#include <stdlib.h>

#include "cpu.h"

int main(int argc, char* argv[])
{
  struct cpu_t mcpu_obj;
  struct cpu_t *mcpu = &mcpu_obj;

  init_cpu(mcpu);
  return EXIT_SUCCESS;
}
