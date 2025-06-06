#include "bytecode_generator.h"
#include "bytecode_opcodes.h"
#include "ir_core.h" // Your IR definitions
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_BYTECODE_CAPACITY 1024
#define MAX_VARS_TEMPS_BC 512
#define MAX_LABELS_BC 256
#define MAX_STRINGS_BC 256

typedef struct {
    char* name;
    int bytecode_offset; // Corrected member name from .offset
} LabelMapEntry;

typedef struct {
    char* name;
    uint16_t slot_id;
} VarTempMapEntryBC;

typedef struct JumpPatchBC {
    char* label_name_to_resolve;
    int bytecode_offset_of_jump_operand;
    struct JumpPatchBC* next;
} JumpPatchBC;

static BytecodeChunk current_bc_chunk;
static LabelMapEntry label_map_bc[MAX_LABELS_BC];
static int label_map_count_bc = 0;
static VarTempMapEntryBC var_temp_map_bc[MAX_VARS_TEMPS_BC];
static int var_temp_map_count_bc = 0;
static JumpPatchBC* jump_patch_list_bc = NULL;

static int bc_is_string_number(const char *s) {
    if (!s || *s == '\0') return 0;
    char *p = (char*)s;
    if (*p == '-') p++;
    if (*p == '\0') return 0;
    while (*p != '\0') {
        if (!isdigit(*p)) return 0;
        p++;
    }
    return 1;
}

static uint16_t get_or_assign_slot_id_bc(const char* name) {
    if (!name) {
        fprintf(stderr, "BC Gen Error: Attempt to get slot for NULL name.\n");
        return (uint16_t)-1;
    }
    for (int i = 0; i < var_temp_map_count_bc; ++i) {
        if (var_temp_map_bc[i].name && strcmp(var_temp_map_bc[i].name, name) == 0) {
            return var_temp_map_bc[i].slot_id;
        }
    }
    if (var_temp_map_count_bc >= MAX_VARS_TEMPS_BC) {
        fprintf(stderr, "BC Gen Error: Too many variables/temporaries for slot mapping.\n");
        exit(EXIT_FAILURE);
    }
    var_temp_map_bc[var_temp_map_count_bc].name = strdup(name);
    if (!var_temp_map_bc[var_temp_map_count_bc].name) {perror("strdup var_temp_map"); exit(EXIT_FAILURE);}
    var_temp_map_bc[var_temp_map_count_bc].slot_id = (uint16_t)var_temp_map_count_bc;
    return (uint16_t)var_temp_map_count_bc++;
}

static void ensure_string_pool_capacity(BytecodeChunk *chunk) {
    if (chunk->string_pool_count >= chunk->string_pool_capacity) {
        int new_capacity = chunk->string_pool_capacity == 0 ? 16 : chunk->string_pool_capacity * 2;
        char** new_pool = (char**)realloc(chunk->string_pool, new_capacity * sizeof(char*));
        if (!new_pool) { perror("realloc string_pool"); exit(EXIT_FAILURE); }
        chunk->string_pool = new_pool;
        chunk->string_pool_capacity = new_capacity;
    }
}

static uint16_t add_string_to_pool_bc(BytecodeChunk *chunk, const char* str) {
    if (!str) return (uint16_t)-1;
    for (int i = 0; i < chunk->string_pool_count; ++i) {
        if (chunk->string_pool[i] && strcmp(chunk->string_pool[i], str) == 0) {
            return (uint16_t)i;
        }
    }
    ensure_string_pool_capacity(chunk);
    chunk->string_pool[chunk->string_pool_count] = strdup(str);
    if(!chunk->string_pool[chunk->string_pool_count]) {perror("strdup string_pool"); exit(EXIT_FAILURE);}
    return (uint16_t)chunk->string_pool_count++;
}

void init_bytecode_chunk(BytecodeChunk *chunk, int initial_capacity) {
    if (initial_capacity <= 0) initial_capacity = INITIAL_BYTECODE_CAPACITY;
    chunk->code = (uint8_t*)malloc(initial_capacity);
    if (!chunk->code) { perror("malloc bytecode code"); exit(EXIT_FAILURE); }
    chunk->capacity = initial_capacity;
    chunk->count = 0;
    chunk->string_pool = NULL;
    chunk->string_pool_capacity = 0;
    chunk->string_pool_count = 0;
    ensure_string_pool_capacity(chunk);
}

void write_byte_to_chunk(BytecodeChunk *chunk, uint8_t byte) {
    if (chunk->count >= chunk->capacity) {
        chunk->capacity = (chunk->capacity == 0) ? INITIAL_BYTECODE_CAPACITY : chunk->capacity * 2;
        uint8_t* new_code = (uint8_t*)realloc(chunk->code, chunk->capacity);
        if (!new_code) { perror("realloc bytecode code"); exit(EXIT_FAILURE); }
        chunk->code = new_code;
    }
    chunk->code[chunk->count++] = byte;
}
void write_short_to_chunk(BytecodeChunk *chunk, uint16_t value) {
    write_byte_to_chunk(chunk, (value >> 8) & 0xFF);
    write_byte_to_chunk(chunk, value & 0xFF);
}
void write_int_to_chunk(BytecodeChunk *chunk, int32_t value) {
    write_byte_to_chunk(chunk, (value >> 24) & 0xFF);
    write_byte_to_chunk(chunk, (value >> 16) & 0xFF);
    write_byte_to_chunk(chunk, (value >> 8) & 0xFF);
    write_byte_to_chunk(chunk, value & 0xFF);
}
void free_bytecode_chunk(BytecodeChunk *chunk) {
    if (chunk) {
        if (chunk->code) free(chunk->code);
        for (int i = 0; i < chunk->string_pool_count; ++i) {
            if (chunk->string_pool[i]) free(chunk->string_pool[i]);
        }
        if (chunk->string_pool) free(chunk->string_pool);
        memset(chunk, 0, sizeof(BytecodeChunk));
    }
}
void patch_short_in_chunk(BytecodeChunk *chunk, int offset, uint16_t value) {
    if (offset >= 0 && offset + 1 < chunk->count) { // Corrected: offset + 1 < count
        chunk->code[offset]     = (value >> 8) & 0xFF;
        chunk->code[offset + 1] = value & 0xFF;
    } else {
        fprintf(stderr, "BC Gen Error: Invalid offset %d for patching short (chunk count: %d)\n", offset, chunk->count);
    }
}

static void bc_generator_reset_state() {
    for(int i=0; i<label_map_count_bc; ++i) if(label_map_bc[i].name) {free(label_map_bc[i].name); label_map_bc[i].name=NULL;}
    label_map_count_bc = 0;
    for(int i=0; i<var_temp_map_count_bc; ++i) if(var_temp_map_bc[i].name) {free(var_temp_map_bc[i].name); var_temp_map_bc[i].name=NULL;}
    var_temp_map_count_bc = 0;
    JumpPatchBC* current_patch = jump_patch_list_bc;
    while(current_patch) {
        JumpPatchBC* next_patch = current_patch->next;
        if(current_patch->label_name_to_resolve) free(current_patch->label_name_to_resolve);
        free(current_patch);
        current_patch = next_patch;
    }
    jump_patch_list_bc = NULL;
}

static void add_jump_to_patch_bc(const char* label_name, int jump_operand_offset) {
    if (!label_name) { fprintf(stderr, "BC Gen Error: NULL label name for jump patch.\n"); return; }
    JumpPatchBC* patch = (JumpPatchBC*)malloc(sizeof(JumpPatchBC));
    if(!patch) { perror("malloc JumpPatchBC"); exit(EXIT_FAILURE); }
    patch->label_name_to_resolve = strdup(label_name);
    if(!patch->label_name_to_resolve) { perror("strdup label_name_to_resolve"); free(patch); exit(EXIT_FAILURE); }
    patch->bytecode_offset_of_jump_operand = jump_operand_offset;
    patch->next = jump_patch_list_bc;
    jump_patch_list_bc = patch;
}

BytecodeChunk generate_bytecode(IRList *ir_list_head) {
    init_bytecode_chunk(&current_bc_chunk, INITIAL_BYTECODE_CAPACITY);
    bc_generator_reset_state();

    IRList *current_ir_node = ir_list_head;
    int estimated_bc_offset = 0;

    printf("BC Gen: Pass 1 - Calculating label offsets...\n");
    while (current_ir_node) {
        IRInstruction *instr = current_ir_node->instr;
        if (!instr) { current_ir_node = current_ir_node->next; continue; }

        if (instr->op == OP_LABEL && instr->label) {
            if (label_map_count_bc < MAX_LABELS_BC) {
                label_map_bc[label_map_count_bc].name = strdup(instr->label);
                if(!label_map_bc[label_map_count_bc].name){perror("strdup label_map_bc.name"); exit(EXIT_FAILURE);}
                label_map_bc[label_map_count_bc].bytecode_offset = estimated_bc_offset; // Corrected
                label_map_count_bc++;
            } else { fprintf(stderr, "BC Gen Error: Too many labels.\n"); exit(EXIT_FAILURE); }
        } else {
            // Update estimated_bc_offset based on the instruction type
            // This needs to accurately reflect what Pass 2 will emit.
            switch(instr->op) {
                case OP_ASSIGN:
                    if (instr->arg1 && bc_is_string_number(instr->arg1)) estimated_bc_offset += 1 + 2 + 4; // LOAD_CONST_NUM_TO_SLOT
                    else if (instr->arg1 && instr->arg1[0] == '"') estimated_bc_offset += 1 + 2 + 2; // LOAD_STRING_LABEL_TO_SLOT
                    else estimated_bc_offset += 1 + 2 + 2; // MOV_SLOT_SLOT
                    break;
                case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV: case OP_MOD:
                case OP_EQ: case OP_NEQ: case OP_LT: case OP_LE: case OP_GT: case OP_GE:
                    estimated_bc_offset += 1 + 2 + 2 + 2; // opcode, dest, src1, src2 slots
                    break;
                case OP_GOTO:   estimated_bc_offset += 1 + 2; break; // opcode, offset
                case OP_IFGOTO: estimated_bc_offset += 1 + 2 + 2; break; // opcode, cond_slot, offset
                case OP_PARAM:  estimated_bc_offset += 1 + 2; // BC_OP_PARAM_SLOT slot
                                // If constant param, it might become LOAD_CONST + PARAM_SLOT
                                if (instr->result && bc_is_string_number(instr->result)) estimated_bc_offset += 1 + 2 + 4; // for the LOAD_CONST
                                break;
                case OP_CALL:
                    estimated_bc_offset += 1 + 2 + 1; // opcode, func_id, num_args
                    if(instr->result) estimated_bc_offset += 2; // for result slot
                    break;
                case OP_RETURN:
                    estimated_bc_offset += 1;
                    if (instr->result) estimated_bc_offset += 2; // value_slot
                    break;
                case OP_READ:   estimated_bc_offset += 1 + 2; break; // opcode, dest_slot
                case OP_PRINT:  estimated_bc_offset += 1 + 2; // opcode, slot_or_id
                                // If const number, may involve a temp load first
                                if (instr->result && bc_is_string_number(instr->result)) estimated_bc_offset += 1 + 2 + 4;
                                break;
                default: estimated_bc_offset += 1; // default for others like NOP
            }
        }
        current_ir_node = current_ir_node->next;
    }

    printf("BC Gen: Pass 2 - Emitting bytecode...\n");
    // var_temp_map_count_bc = 0; // Reset for fresh slot assignment (if slots are function-local, this is more complex)
                               // For now, global slot pool for simplicity

    current_ir_node = ir_list_head;
    while (current_ir_node) {
        IRInstruction *instr = current_ir_node->instr;
        if (!instr) { current_ir_node = current_ir_node->next; continue; }

        uint16_t res_slot, arg1_slot, arg2_slot;

        // Switch on IROpcode from ir_core.h
        switch (instr->op) {
            case OP_ASSIGN:
                if (!instr->result || !instr->arg1) {fprintf(stderr, "BC Gen Error: OP_ASSIGN missing operands.\n"); break;}
                res_slot = get_or_assign_slot_id_bc(instr->result);
                if (bc_is_string_number(instr->arg1)) {
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_LOAD_CONST_NUM_TO_SLOT);
                    write_short_to_chunk(&current_bc_chunk, res_slot);
                    write_int_to_chunk(&current_bc_chunk, atoi(instr->arg1));
                } else if (instr->arg1[0] == '"' || (instr->arg1[0] == '.' && instr->arg1[1] == 'S')) {
                    uint16_t string_id = add_string_to_pool_bc(&current_bc_chunk, instr->arg1);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_LOAD_STRING_LABEL_TO_SLOT);
                    write_short_to_chunk(&current_bc_chunk, res_slot);
                    write_short_to_chunk(&current_bc_chunk, string_id);
                } else {
                    arg1_slot = get_or_assign_slot_id_bc(instr->arg1);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_MOV_SLOT_SLOT);
                    write_short_to_chunk(&current_bc_chunk, res_slot);
                    write_short_to_chunk(&current_bc_chunk, arg1_slot);
                }
                break;

            case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV: case OP_MOD:
            case OP_EQ:  case OP_NEQ: case OP_LT:  case OP_LE:  case OP_GT:  case OP_GE:
                if (!instr->result || !instr->arg1 || !instr->arg2) { /* error */ break; }
                res_slot  = get_or_assign_slot_id_bc(instr->result);
                arg1_slot = get_or_assign_slot_id_bc(instr->arg1);
                arg2_slot = get_or_assign_slot_id_bc(instr->arg2);
                BytecodeOpcode bc_op_to_emit = BC_OP_NOP;
                if(instr->op == OP_ADD) bc_op_to_emit = BC_OP_ADD_SLOT;
                else if(instr->op == OP_SUB) bc_op_to_emit = BC_OP_SUB_SLOT;
                // ... (map all other IROpcodes to BytecodeOpcodes) ...
                else if(instr->op == OP_GE)  bc_op_to_emit = BC_OP_CMP_GE_SLOT;

                if (bc_op_to_emit != BC_OP_NOP) {
                    write_byte_to_chunk(&current_bc_chunk, bc_op_to_emit);
                    write_short_to_chunk(&current_bc_chunk, res_slot);
                    write_short_to_chunk(&current_bc_chunk, arg1_slot);
                    write_short_to_chunk(&current_bc_chunk, arg2_slot);
                } else { /* error: unmapped IR op */ }
                break;

            case OP_LABEL: break;
            case OP_GOTO:
                if (!instr->label) { /* error */ break; }
                write_byte_to_chunk(&current_bc_chunk, BC_OP_JUMP);
                add_jump_to_patch_bc(instr->label, current_bc_chunk.count);
                write_short_to_chunk(&current_bc_chunk, 0xFFFF);
                break;
            case OP_IFGOTO: // IR: if result == arg1("0") goto label
                if (!instr->result || !instr->label || !instr->arg1 || strcmp(instr->arg1, "0") != 0) {
                    fprintf(stderr, "BC Gen Error: OP_IFGOTO format incorrect (expected cond == 0 goto LBL).\n"); break;
                }
                res_slot = get_or_assign_slot_id_bc(instr->result); // Condition slot
                write_byte_to_chunk(&current_bc_chunk, BC_OP_JUMP_IF_FALSE);
                write_short_to_chunk(&current_bc_chunk, res_slot);
                add_jump_to_patch_bc(instr->label, current_bc_chunk.count);
                write_short_to_chunk(&current_bc_chunk, 0xFFFF);
                break;

            case OP_PARAM: // IR: param result (value)
                if (!instr->result) { /* error */ break; }
                // If param is a constant, ideally load it to a temp slot first if VM can't take const for param
                if (bc_is_string_number(instr->result)) {
                    char temp_param_name[32]; sprintf(temp_param_name, ".bct_param%d", var_temp_map_count_bc);
                    uint16_t temp_slot = get_or_assign_slot_id_bc(temp_param_name);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_LOAD_CONST_NUM_TO_SLOT);
                    write_short_to_chunk(&current_bc_chunk, temp_slot);
                    write_int_to_chunk(&current_bc_chunk, atoi(instr->result));
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_PARAM_SLOT);
                    write_short_to_chunk(&current_bc_chunk, temp_slot);
                } else {
                    arg1_slot = get_or_assign_slot_id_bc(instr->result);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_PARAM_SLOT);
                    write_short_to_chunk(&current_bc_chunk, arg1_slot);
                }
                break;
            case OP_CALL: // IR: result = call arg1 (func_name)
                if (!instr->arg1) { /* error */ break; }
                uint8_t num_args_val = 0; // TODO: Determine actual number of args!
                                        // This requires scanning previous PARAMs or info from semantic analysis.
                                        // For now, placeholder.

                if (instr->result) {
                    res_slot = get_or_assign_slot_id_bc(instr->result);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_CALL_STORE_RESULT);
                    write_short_to_chunk(&current_bc_chunk, res_slot);
                } else {
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_CALL);
                }
                add_jump_to_patch_bc(instr->arg1, current_bc_chunk.count); // func_name is a label
                write_short_to_chunk(&current_bc_chunk, 0xFFFF);          // placeholder for func address
                write_byte_to_chunk(&current_bc_chunk, num_args_val);
                break;
            case OP_RETURN:
                if (instr->result) {
                    res_slot = get_or_assign_slot_id_bc(instr->result);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_RETURN_VAL);
                    write_short_to_chunk(&current_bc_chunk, res_slot);
                } else {
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_RETURN);
                }
                break;
            case OP_READ: // IR: result = read
                if (!instr->result) { /* error */ break; }
                res_slot = get_or_assign_slot_id_bc(instr->result);
                write_byte_to_chunk(&current_bc_chunk, BC_OP_READ_NUM_TO_SLOT);
                write_short_to_chunk(&current_bc_chunk, res_slot);
                break;
            case OP_PRINT: // IR: print result
                if (!instr->result) { /* error */ break; }
                // String literals from IR generator are now ".S<num>" or "\"actual string\""
                // if (instr->result is a number string) then print number
                // else if (instr->result is a string label or literal string) then print text
                // This requires type information or better IR operands.
                if (bc_is_string_number(instr->result)) {
                    char temp_print_name[32]; sprintf(temp_print_name, ".bct_p%d", var_temp_map_count_bc);
                    uint16_t temp_slot = get_or_assign_slot_id_bc(temp_print_name);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_LOAD_CONST_NUM_TO_SLOT);
                    write_short_to_chunk(&current_bc_chunk, temp_slot);
                    write_int_to_chunk(&current_bc_chunk, atoi(instr->result));
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_PRINT_NUM_SLOT);
                    write_short_to_chunk(&current_bc_chunk, temp_slot);
                } else if (instr->result[0] == '"' || (instr->result[0] == '.' && instr->result[1] == 'S')) {
                    uint16_t str_id = add_string_to_pool_bc(&current_bc_chunk, instr->result);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_PRINT_TEXT_SLOT); // Placeholder, needs ID for string pool
                    write_short_to_chunk(&current_bc_chunk, str_id);
                } else { // Assume it's a variable/temp holding a number for now
                    res_slot = get_or_assign_slot_id_bc(instr->result);
                    write_byte_to_chunk(&current_bc_chunk, BC_OP_PRINT_NUM_SLOT);
                    write_short_to_chunk(&current_bc_chunk, res_slot);
                }
                break;
            default:
                // Use instr->source_line if you added it to your IRInstruction
                fprintf(stderr, "BC Gen Warning: Unhandled IR Opcode %d in Pass 2.\n", instr->op);
                write_byte_to_chunk(&current_bc_chunk, BC_OP_NOP);
                break;
        }
        current_ir_node = current_ir_node->next;
    }
    write_byte_to_chunk(&current_bc_chunk, BC_OP_HALT);

    printf("BC Gen: Pass 3 - Backpatching jumps...\n");
    JumpPatchBC* current_p = jump_patch_list_bc;
    while(current_p) {
        int target_bc_offset = -1;
        for (int i = 0; i < label_map_count_bc; ++i) {
            if (label_map_bc[i].name && strcmp(label_map_bc[i].name, current_p->label_name_to_resolve) == 0) {
                target_bc_offset = label_map_bc[i].bytecode_offset;
                break;
            }
        }
        if (target_bc_offset != -1) {
            patch_short_in_chunk(&current_bc_chunk, current_p->bytecode_offset_of_jump_operand, (uint16_t)target_bc_offset);
        } else {
            fprintf(stderr, "BC Gen Error: Undefined label '%s' for jump patching.\n", current_p->label_name_to_resolve);
        }
        current_p = current_p->next;
    }
    // Freeing moved to bc_generator_reset_state

    printf("BC Gen: Var/Temp to Slot mapping count: %d\n", var_temp_map_count_bc);
    return current_bc_chunk;
}

void print_bytecode_raw(BytecodeChunk *chunk) {
    if (!chunk || !chunk->code) { printf("No bytecode to print.\n"); return; }
    printf("\n--- Raw Bytecode Hex Dump (%d bytes) ---\n", chunk->count);
    for (int i = 0; i < chunk->count; ++i) {
        printf("%02X ", chunk->code[i]);
        if ((i + 1) % 16 == 0 || i == chunk->count - 1) { printf("\n"); }
    }
    if (chunk->string_pool_count > 0) {
        printf("\n--- String Pool (BC Chunk) ---\n");
        for (int i = 0; i < chunk->string_pool_count; i++) {
            printf("  ID %d: %s\n", i, chunk->string_pool[i] ? chunk->string_pool[i] : "(null string)");
        }
    }
    printf("--- End of Bytecode Info ---\n");
}
