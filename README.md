# b-vm 8-bit vm

This is a custom-built 8-bit Virtual Machine designed with a 
Reduced Instruction Set (RISC) philosophy. It features a dedicated 
ABCD register file and a strictly defined memory map.

The project mimics the behavior of early 80s microprocessors like 
the Intel 8080 or Z80, providing a controlled environment for bytecode 
execution, stack management, and register-to-register arithmetic.

Architecture

The VM operates on a Von Neumann model with a 256-byte unified memory space.
Registers

    A (Accumulator): Primary register for arithmetic and logic results.

    B (Base): Secondary operand storage.

    C (Counter): General-purpose, intended for loop iterations.

    D (Data): General-purpose scratchpad.

    IP (Instruction Pointer): Holds the address of the next instruction to be executed.

    SP (Stack Pointer): Tracks the top of the stack (grows downwards from 0xFF).
