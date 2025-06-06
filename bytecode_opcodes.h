#ifndef BYTECODE_OPCODES_H
#define BYTECODE_OPCODES_H

#include <stdint.h>

typedef enum {
    // VM Control
    BC_OP_HALT = 0x00,
    BC_OP_NOP  = 0xFF,

    // Load Constants into a Slot
    BC_OP_LOAD_CONST_NUM_TO_SLOT = 0x01, // Operands: dest_slot (uint16_t), const_value (int32_t)
    BC_OP_LOAD_STRING_LABEL_TO_SLOT = 0x02, // Operands: dest_slot (uint16_t), string_label_id (uint16_t)

    // Variable/Slot Operations
    BC_OP_MOV_SLOT_SLOT = 0x03, // Operands: dest_slot (uint16_t), src_slot (uint16_t) (result = arg1)

    // Arithmetic (result_slot = slot1 op slot2)
    BC_OP_ADD_SLOT = 0x10, // Operands: dest_slot, slot1, slot2
    BC_OP_SUB_SLOT = 0x11,
    BC_OP_MUL_SLOT = 0x12,
    BC_OP_DIV_SLOT = 0x13,
    BC_OP_MOD_SLOT = 0x14,
    // BC_OP_NEG_SLOT = 0x15, // dest_slot = -src_slot (can be 0 - src_slot)

    // Comparison (result_slot = slot1 op slot2 ? 1 : 0)
    BC_OP_CMP_EQ_SLOT  = 0x20,
    BC_OP_CMP_NEQ_SLOT = 0x21,
    BC_OP_CMP_LT_SLOT  = 0x22,
    BC_OP_CMP_LE_SLOT  = 0x23,
    BC_OP_CMP_GT_SLOT  = 0x24,
    BC_OP_CMP_GE_SLOT  = 0x25,

    // Control Flow
    BC_OP_JUMP          = 0x30, // Operand: target_bytecode_offset (uint16_t, absolute or relative)
    BC_OP_JUMP_IF_FALSE = 0x31, // Operands: cond_slot (uint16_t), target_bytecode_offset (uint16_t)

    // Function Related (Simplified for now)
    BC_OP_PARAM_SLOT = 0x40, // Operand: slot_id of value to pass as parameter
    BC_OP_CALL       = 0x41, // Operands: func_label_id (uint16_t), num_args (uint8_t)
    BC_OP_CALL_STORE_RESULT = 0x42, // Operands: dest_slot (uint16_t), func_label_id, num_args
    BC_OP_RETURN     = 0x43, // No operand (void return)
    BC_OP_RETURN_VAL = 0x44, // Operand: slot_id of value to return

    // Array Operations (More complex - placeholders, assumes base address + index logic for VM)
    // BC_OP_LOAD_ARRAY_ELEMENT  = 0x50, // result_slot = array_base_slot[index_slot]
    // BC_OP_STORE_ARRAY_ELEMENT = 0x51, // array_base_slot[index_slot] = value_slot

    // I/O
    BC_OP_PRINT_NUM_SLOT  = 0x60, // Operand: slot_id of number
    BC_OP_PRINT_TEXT_SLOT = 0x61, // Operand: slot_id holding string_label_id or direct string_label_id
    BC_OP_READ_NUM_TO_SLOT= 0x62, // Operand: dest_slot

    // You will likely expand this set based on your IR and VM design.
    // For example, opcodes for different operand types (Var-Const, Const-Var).
} BytecodeOpcode;

#endif // BYTECODE_OPCODES_H
