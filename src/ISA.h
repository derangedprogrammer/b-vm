#ifndef VM_CORE_ISA_H
#define VM_CORE_ISA_H

#define REG_A          0x00
#define REG_B          0x01
#define REG_C          0x02
#define REG_D          0x03
#define REG_IP         0x04
#define REG_DP         0x05

/**
 * SYSTEM & UTILITY (0x0X)
 */
#define OP_SYSTEM_NO_OPERATION             0x00
#define OP_SYSTEM_HALT_EXECUTION           0x0F

/**
 * DATA MOVEMENT: REGISTER-TO-REGISTER (0x1X)
 * Format: [OPCODE] [DEST_REG] [SRC_REG]
 */
#define OP_MOVE_REGISTER_TO_REGISTER       0x10
#define OP_SWAP_REGISTER_VALUES            0x11

/**
 * DATA MOVEMENT: IMMEDIATE & MEMORY (0x2X - 0x5X)
 * Format: [OPCODE] [REG] [VALUE/ADDR]
 */
#define OP_LOAD_IMMEDIATE_VALUE            0x20
#define OP_LOAD_FROM_MEMORY_ADDRESS        0x30
#define OP_STORE_TO_MEMORY_ADDRESS         0x40
#define OP_LOAD_FROM_MEMORY_INDIRECT       0x50 // Reg = RAM[Reg_Source]

/**
 * ARITHMETIC OPERATIONS (0x6X)
 * Format: [OPCODE] [DEST_REG] [SRC_REG]
 */
#define OP_ARITHMETIC_ADD_REGISTERS        0x60
#define OP_ARITHMETIC_SUBTRACT_REGISTERS   0x61
#define OP_ARITHMETIC_INCREMENT_REGISTER   0x62
#define OP_ARITHMETIC_DECREMENT_REGISTER   0x63

/**
 * LOGICAL OPERATIONS (0x7X)
 */
#define OP_LOGIC_AND_REGISTERS             0x70
#define OP_LOGIC_OR_REGISTERS              0x71
#define OP_LOGIC_XOR_REGISTERS             0x72
#define OP_LOGIC_NOT_REGISTER              0x73
#define OP_LOGIC_COMPARE_REGISTERS         0x74 // Updates flags only

/**
 * STACK OPERATIONS (0x8X)
 * Uses the DP register. Stack usually grows 0xFF -> 0x00.
 */
#define OP_STACK_PUSH_REGISTER             0x80
#define OP_STACK_POP_REGISTER              0x81
#define OP_STACK_PEEK_REGISTER             0x82

/**
 * CONTROL FLOW (0x9X)
 * Format: [OPCODE] [8-BIT_ADDR]
 */
#define OP_BRANCH_UNCONDITIONAL_JUMP       0x90
#define OP_BRANCH_JUMP_IF_ZERO             0x91
#define OP_BRANCH_JUMP_IF_NOT_ZERO         0x92
#define OP_BRANCH_CALL_SUBROUTINE          0x93
#define OP_BRANCH_RETURN_FROM_SUBROUTINE   0x94

/**
 * INPUT / OUTPUT (0xAX)
 */
#define OP_IO_WRITE_TO_PORT                0xA0
#define OP_IO_READ_FROM_PORT               0xA1

#endif /* VM_CORE_ISA_H */
