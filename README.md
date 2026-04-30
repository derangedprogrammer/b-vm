# b-vm 8-bit vm

This is a custom-built 8-bit Virtual Machine designed with a 
Reduced Instruction Set (RISC) philosophy. It features a dedicated 
ABCD register file and a strictly defined memory map.

The project (crappily) mimics the behavior of early 80s microprocessors like 
the Intel 8080 or Z80, providing a controlled-ish environment for bytecode 
execution, stack management, and register-to-register arithmetic.

Architecture

The VM operates on a modified Harvard Architecture model with a 256-byte 
space for instructions and another 256 for data.
