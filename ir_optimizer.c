// ir_optimizer.c
#include "ir_optimizer.h" // This should include ir_core.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isdigit, etc.

// --- Helper functions for optimization ---

static int is_number(const char *s) {
    if (!s || *s == '\0' || (*s == '-' && *(s + 1) == '\0')) return 0;
    char *end;
    strtol(s, &end, 10);
    return (*end == '\0');
}

static int is_temporary_var(const char* s) {
    if (!s || s[0] != 't') return 0;
    if (s[1] == '\0') return 0; // Must have digits after 't'
    for (int i = 1; s[i] != '\0'; ++i) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

// Helper to free fields of a single IR instruction (used by DCE in optimizer)
static void free_single_ir_instruction_fields_opt(IRInstruction *instr) {
    if (!instr) return;
    free(instr->result); instr->result = NULL;
    free(instr->arg1);   instr->arg1 = NULL;
    free(instr->arg2);   instr->arg2 = NULL;
    free(instr->label);  instr->label = NULL;
}


// --- Constant Folding Pass ---
static IRList* constant_folding_pass(IRList *head, int *changes_made_flag) {
    IRList *curr = head;
    while (curr) {
        IRInstruction *instr = curr->instr;
        if ((instr->op >= OP_ADD && instr->op <= OP_MOD) || (instr->op >= OP_EQ && instr->op <= OP_GE)) {
            if (instr->arg1 && instr->arg2 && is_number(instr->arg1) && is_number(instr->arg2)) {
                long val1 = atol(instr->arg1);
                long val2 = atol(instr->arg2);
                long result_val = 0;
                int computed_successfully = 1;

                switch (instr->op) {
                    case OP_ADD: result_val = val1 + val2; break;
                    case OP_SUB: result_val = val1 - val2; break;
                    case OP_MUL: result_val = val1 * val2; break;
                    case OP_DIV: if (val2 != 0) result_val = val1 / val2; else computed_successfully = 0; break;
                    case OP_MOD: if (val2 != 0) result_val = val1 % val2; else computed_successfully = 0; break;
                    case OP_EQ:  result_val = (val1 == val2); break;
                    case OP_NEQ: result_val = (val1 != val2); break;
                    case OP_LT:  result_val = (val1 < val2); break;
                    case OP_LE:  result_val = (val1 <= val2); break;
                    case OP_GT:  result_val = (val1 > val2); break;
                    case OP_GE:  result_val = (val1 >= val2); break;
                    default: computed_successfully = 0; break;
                }

                if (computed_successfully) {
                    char res_str[32];
                    sprintf(res_str, "%ld", result_val);
                    instr->op = OP_ASSIGN;
                    free(instr->arg1);
                    instr->arg1 = strdup(res_str);
                    free(instr->arg2);
                    instr->arg2 = NULL;
                    *changes_made_flag = 1;
                }
            }
        } else if (instr->op == OP_SUB && instr->arg1 && instr->arg2 &&
                   strcmp(instr->arg1, "0") == 0 && is_number(instr->arg2)) { // Unary minus: 0 - const
            long val2 = atol(instr->arg2);
            long result_val = -val2;
            char res_str[32];
            sprintf(res_str, "%ld", result_val);
            instr->op = OP_ASSIGN;
            free(instr->arg1);
            instr->arg1 = strdup(res_str);
            free(instr->arg2);
            instr->arg2 = NULL;
            *changes_made_flag = 1;
        }
        curr = curr->next;
    }
    return head;
}

// --- Constant Propagation Pass ---
typedef struct ConstMapEntry {
    char *var_name;
    char *const_value;
    struct ConstMapEntry *next;
} ConstMapEntry;

static void add_or_update_const_map(ConstMapEntry **map_head, const char *var_name, const char *const_val_str) {
    ConstMapEntry *curr = *map_head;
    while (curr) {
        if (strcmp(curr->var_name, var_name) == 0) {
            free(curr->const_value);
            curr->const_value = strdup(const_val_str);
            return;
        }
        curr = curr->next;
    }
    ConstMapEntry *new_entry = (ConstMapEntry*)malloc(sizeof(ConstMapEntry));
    new_entry->var_name = strdup(var_name);
    new_entry->const_value = strdup(const_val_str);
    new_entry->next = *map_head;
    *map_head = new_entry;
}

static char* get_from_const_map(ConstMapEntry *map_head, const char *var_name) {
    ConstMapEntry *curr = map_head;
    while (curr) {
        if (strcmp(curr->var_name, var_name) == 0) return curr->const_value;
        curr = curr->next;
    }
    return NULL;
}

static void invalidate_in_const_map(ConstMapEntry **map_head, const char *var_name) {
    ConstMapEntry *curr = *map_head, *prev = NULL;
    while (curr) {
        if (strcmp(curr->var_name, var_name) == 0) {
            if (prev) prev->next = curr->next;
            else *map_head = curr->next;
            free(curr->var_name);
            free(curr->const_value);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

static void free_const_map(ConstMapEntry *map_head) {
    while (map_head) {
        ConstMapEntry *temp = map_head;
        map_head = map_head->next;
        free(temp->var_name);
        free(temp->const_value);
        free(temp);
    }
}

static IRList* constant_propagation_pass(IRList *head, int *changes_made_flag) {
    ConstMapEntry *known_constants = NULL;
    IRList *curr = head;
    char *const_val_str;

    while (curr) {
        IRInstruction *instr = curr->instr;

        if (instr->arg1 && is_temporary_var(instr->arg1) && (const_val_str = get_from_const_map(known_constants, instr->arg1))) {
            free(instr->arg1);
            instr->arg1 = strdup(const_val_str);
            *changes_made_flag = 1;
        }
        if (instr->arg2 && is_temporary_var(instr->arg2) && (const_val_str = get_from_const_map(known_constants, instr->arg2))) {
            free(instr->arg2);
            instr->arg2 = strdup(const_val_str);
            *changes_made_flag = 1;
        }
        if (instr->op == OP_IFGOTO && instr->result && is_temporary_var(instr->result) && (const_val_str = get_from_const_map(known_constants, instr->result))) {
            free(instr->result);
            instr->result = strdup(const_val_str);
            *changes_made_flag = 1;
        }
        if ((instr->op == OP_PRINT || instr->op == OP_PARAM || instr->op == OP_RETURN) &&
            instr->result && is_temporary_var(instr->result) && (const_val_str = get_from_const_map(known_constants, instr->result))) {
            free(instr->result);
            instr->result = strdup(const_val_str);
            *changes_made_flag = 1;
        }

        if (instr->result && is_temporary_var(instr->result)) {
            if (instr->op == OP_ASSIGN && instr->arg1 && is_number(instr->arg1)) {
                add_or_update_const_map(&known_constants, instr->result, instr->arg1);
            } else {
                invalidate_in_const_map(&known_constants, instr->result);
            }
        }
        
        if (instr->op == OP_LABEL || instr->op == OP_GOTO || instr->op == OP_CALL || instr->op == OP_IFGOTO) { // Reset at control flow changes or calls
            free_const_map(known_constants);
            known_constants = NULL;
        }
        curr = curr->next;
    }
    free_const_map(known_constants);
    return head;
}

// --- Dead Code Elimination Pass ---
typedef struct UsageSetEntry {
    char *name;
    struct UsageSetEntry *next;
} UsageSetEntry;

static void add_to_usage_set(UsageSetEntry **set_head, const char* name_str) {
    if (!name_str || !is_temporary_var(name_str)) return;
    UsageSetEntry *curr = *set_head;
    while (curr) {
        if (strcmp(curr->name, name_str) == 0) return;
        curr = curr->next;
    }
    UsageSetEntry *new_entry = (UsageSetEntry*)malloc(sizeof(UsageSetEntry));
    new_entry->name = strdup(name_str);
    new_entry->next = *set_head;
    *set_head = new_entry;
}

static void scan_and_add_temps_in_string_to_usage(UsageSetEntry **set, const char* str) {
    if (!str) return;
    const char *p = str;
    while ((p = strchr(p, 't')) != NULL) {
        const char *start_of_temp = p;
        p++; 
        if (*p == '\0' || !isdigit(*p)) {
            if (*p == '\0') break;
            continue;
        }
        while (isdigit(*p)) p++;
        char temp_name_buf[16];
        int len = p - start_of_temp;
        if (len > 0 && len < sizeof(temp_name_buf)) { // Ensure len > 0
            strncpy(temp_name_buf, start_of_temp, len);
            temp_name_buf[len] = '\0';
            if (is_temporary_var(temp_name_buf)) { // Double check it's a valid temp form
                 add_to_usage_set(set, temp_name_buf);
            }
        }
         if (*p == '\0') break; // Important to break if end of string reached after processing a temp
    }
}

static int is_in_usage_set(UsageSetEntry *set_head, const char* name_str) {
    if (!name_str) return 0;
    UsageSetEntry *curr = set_head;
    while (curr) {
        if (strcmp(curr->name, name_str) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

static void free_usage_set(UsageSetEntry *set_head) {
    while (set_head) {
        UsageSetEntry *temp = set_head;
        set_head = set_head->next;
        free(temp->name);
        free(temp);
    }
}

static IRList* dead_code_elimination_pass(IRList *list_head_ptr, int *changes_made_flag) {
    UsageSetEntry *used_temporaries = NULL;
    IRList *current_node;

    current_node = list_head_ptr;
    while (current_node) {
        IRInstruction *instr = current_node->instr;
        if (instr->arg1) add_to_usage_set(&used_temporaries, instr->arg1);
        if (instr->arg2) add_to_usage_set(&used_temporaries, instr->arg2);
        if (instr->op == OP_IFGOTO && instr->result) add_to_usage_set(&used_temporaries, instr->result);
        if ((instr->op == OP_PARAM || instr->op == OP_RETURN || instr->op == OP_PRINT) && instr->result)
            add_to_usage_set(&used_temporaries, instr->result);
        
        if (instr->op == OP_ASSIGN) {
            if (instr->arg1 && strchr(instr->arg1, '[')) { // e.g. t0 = arr[t1]
                scan_and_add_temps_in_string_to_usage(&used_temporaries, instr->arg1);
            }
            if (instr->result && strchr(instr->result, '[')) { // e.g. arr[t0] = t1
                scan_and_add_temps_in_string_to_usage(&used_temporaries, instr->result);
                // Also add arg1 if it's a temp: arr[t0] = t1 -> t1 is used
                if (instr->arg1) add_to_usage_set(&used_temporaries, instr->arg1);
            }
        }
        current_node = current_node->next;
    }

    IRList *new_head = list_head_ptr;
    IRList *prev_node = NULL;
    current_node = list_head_ptr;
    while (current_node) {
        IRInstruction *instr = current_node->instr;
        int is_instruction_dead = 0;

        if (instr->result && is_temporary_var(instr->result) && !is_in_usage_set(used_temporaries, instr->result)) {
            if (strchr(instr->result, '[') == NULL) { // Not an assignment to an array element
                switch(instr->op) {
                    case OP_ASSIGN:
                    case OP_ADD: case OP_SUB: case OP_MUL: case OP_DIV: case OP_MOD:
                    case OP_EQ: case OP_NEQ: case OP_LT: case OP_LE: case OP_GT: case OP_GE:
                        is_instruction_dead = 1;
                        break;
                    // Do not remove CALL, READ, PRINT, GOTO, LABEL, PARAM, RETURN, IFGOTO here even if result is unused
                    // as they have side effects or control flow implications.
                    default: break;
                }
            }
        }

        if (is_instruction_dead) {
            *changes_made_flag = 1;
            IRList *node_to_delete = current_node;
            if (prev_node) prev_node->next = current_node->next;
            else new_head = current_node->next;
            current_node = current_node->next;

            free_single_ir_instruction_fields_opt(node_to_delete->instr);
            free(node_to_delete->instr);
            free(node_to_delete);
        } else {
            prev_node = current_node;
            current_node = current_node->next;
        }
    }
    free_usage_set(used_temporaries);
    return new_head;
}

// Main optimization function
IRList* optimize_ir(IRList *list) {
    if (!list) return NULL;

    int changes_were_made_in_iteration;
    int iteration_num = 0;
    const int MAX_ITERATIONS = 10; // Safety break

    // printf("--- Starting IR Optimization ---\n"); // Debug print
    // printf("Initial IR for Optimization:\n"); print_ir(list); // Debug print from ir_generator

    do {
        changes_were_made_in_iteration = 0;
        iteration_num++;
        // printf("\n--- Optimization Iteration %d ---\n", iteration_num); // Debug

        list = constant_folding_pass(list, &changes_were_made_in_iteration);
        // if (changes_were_made_in_iteration) { printf("After CF:\n"); print_ir(list); } // Debug

        int cp_changes = 0;
        list = constant_propagation_pass(list, &cp_changes);
        if (cp_changes) changes_were_made_in_iteration = 1;
        // if (cp_changes) { printf("After CP:\n"); print_ir(list); } // Debug

        int dce_changes = 0;
        list = dead_code_elimination_pass(list, &dce_changes);
        if (dce_changes) changes_were_made_in_iteration = 1;
        // if (dce_changes) { printf("After DCE:\n"); print_ir(list); } // Debug

    } while (changes_were_made_in_iteration && iteration_num < MAX_ITERATIONS);

    // printf("--- IR Optimization Finished after %d iterations ---\n", iteration_num); // Debug
    return list;
}