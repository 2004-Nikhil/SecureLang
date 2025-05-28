#ifndef BYTECODE_GENERATOR_H
#define BYTECODE_GENERATOR_H

#include "ir_core.h"     // For IRList, IRInstruction (YOUR IR system)
#include <stdint.h>   // For uint8_t, uint16_t, etc.

typedef struct {
    uint8_t *code;
    int capacity;
    int count;
    char** string_pool; // For storing unique strings from the program
    int string_pool_count;
    int string_pool_capacity;
} BytecodeChunk;

void init_bytecode_chunk(BytecodeChunk *chunk, int initial_capacity);
void write_byte_to_chunk(BytecodeChunk *chunk, uint8_t byte);
void write_short_to_chunk(BytecodeChunk *chunk, uint16_t value);
void write_int_to_chunk(BytecodeChunk *chunk, int32_t value);
void free_bytecode_chunk(BytecodeChunk *chunk);
void patch_short_in_chunk(BytecodeChunk *chunk, int offset, uint16_t value);

// Main generation function - takes YOUR IRList*
BytecodeChunk generate_bytecode(IRList *ir_list_head);

// Helper for debugging bytecode
void print_bytecode_raw(BytecodeChunk *chunk);

#endif // BYTECODE_GENERATOR_H
