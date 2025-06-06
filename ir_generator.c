// ir_generator.c
#include "ir_generator.h" // This should include ast.h and ir_core.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// No ctype.h needed here if helper functions like is_number moved to optimizer

// Static variables for IR generation state
static IRList *ir_head_gen = NULL;
static IRList *ir_tail_gen = NULL;
static int temp_count_gen = 0;
static int label_count_gen = 0;

// Helper function to generate a new temporary variable name (specific to generator)
static char* new_temp_gen() {
    char *buf = (char*)malloc(16);
    if (!buf) {
        perror("Failed to allocate new_temp_gen buffer");
        exit(EXIT_FAILURE);
    }
    sprintf(buf, "t%d", temp_count_gen++);
    return buf;
}

// Helper function to generate a new label name (specific to generator)
static char* new_label_gen() {
    char *buf = (char*)malloc(16);
    if (!buf) {
        perror("Failed to allocate new_label_gen buffer");
        exit(EXIT_FAILURE);
    }
    sprintf(buf, "L%d", label_count_gen++);
    return buf;
}

// Helper function to emit a single IR instruction (specific to generator)
static void emit_gen(IROpcode op, const char *res, const char *a1, const char *a2, const char *lbl) {
    IRInstruction *instr = (IRInstruction*)malloc(sizeof(IRInstruction));
    if (!instr) {
        perror("Failed to allocate IRInstruction in emit_gen");
        exit(EXIT_FAILURE);
    }
    instr->op = op;
    instr->result = res ? strdup(res) : NULL;
    instr->arg1 = a1 ? strdup(a1) : NULL;
    instr->arg2 = a2 ? strdup(a2) : NULL;
    instr->label = lbl ? strdup(lbl) : NULL;

    IRList *node = (IRList*)malloc(sizeof(IRList));
    if (!node) {
        perror("Failed to allocate IRList node in emit_gen");
        exit(EXIT_FAILURE);
    }
    node->instr = instr;
    node->next = NULL;

    if (!ir_head_gen) ir_head_gen = ir_tail_gen = node;
    else {
        ir_tail_gen->next = node;
        ir_tail_gen = node;
    }
}

// Forward declarations for recursive IR generation
static char* generate_expr_recursive(ASTNode *node);
static void generate_stmt_recursive(ASTNode *node);

// Function to generate IR for statements (recursive helper)
static void generate_stmt_recursive(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM: {
            ASTNodeList *item = node->data.statement_list;
            while (item) {
                generate_stmt_recursive(item->node);
                item = item->next;
            }
            break;
        }
        case NODE_VAR_DECL:
            if (node->data.var_decl.initializer) {
                char *rhs_val_str = generate_expr_recursive(node->data.var_decl.initializer);
                if (rhs_val_str) {
                    if (node->data.var_decl.var_name) {
                        emit_gen(OP_ASSIGN, node->data.var_decl.var_name, rhs_val_str, NULL, NULL);
                    } else {
                        fprintf(stderr, "IR Gen Error (VarDecl): Variable name is NULL.\n");
                    }
                    free(rhs_val_str);
                } else {
                     fprintf(stderr, "IR Gen Error (VarDecl): Failed to generate RHS for %s.\n", node->data.var_decl.var_name ? node->data.var_decl.var_name : "UNK_VAR");
                }
            }
            break;
        case NODE_ASSIGN_STMT: {
            char *rhs_val_str = generate_expr_recursive(node->data.assignment.rvalue);
            ASTNode* lhs_node = node->data.assignment.lvalue;

            if (!rhs_val_str) {
                fprintf(stderr, "IR Gen Error (Assignment Stmt): Failed to generate RHS expression.\n");
                break;
            }
            if (lhs_node == NULL) {
                 fprintf(stderr, "IR Gen Error (Assignment Stmt): LHS node is NULL.\n");
            } else {
                if (lhs_node->type == NODE_IDENTIFIER) {
                    if (lhs_node->data.identifier_name == NULL) {
                        fprintf(stderr, "IR Gen Error (Assignment Stmt): LHS identifier name is NULL.\n");
                    } else {
                        emit_gen(OP_ASSIGN, lhs_node->data.identifier_name, rhs_val_str, NULL, NULL);
                    }
                } else if (lhs_node->type == NODE_ARRAY_ACCESS) {
                    ASTNode* array_base_node = lhs_node->data.array_access.array_name_or_expr;
                    ASTNode* array_index_node = lhs_node->data.array_access.index_expr;

                    if (!array_base_node || array_base_node->type != NODE_IDENTIFIER || !array_base_node->data.identifier_name || !array_index_node) {
                        fprintf(stderr, "IR Gen Error (Assignment Stmt): Invalid LHS array access structure.\n");
                    } else {
                        char *index_val_str = generate_expr_recursive(array_index_node);
                        if (index_val_str == NULL) {
                            fprintf(stderr, "IR Gen Error (Assignment Stmt): Failed to generate IR for LHS array index for base %s.\n", array_base_node->data.identifier_name);
                        } else {
                            char target_str[128];
                            snprintf(target_str, sizeof(target_str), "%s[%s]", array_base_node->data.identifier_name, index_val_str);
                            emit_gen(OP_ASSIGN, target_str, rhs_val_str, NULL, NULL);
                            free(index_val_str);
                        }
                    }
                } else {
                    fprintf(stderr, "IR Gen Warning (Assignment Stmt): LHS of assignment has unhandled type: %d.\n", lhs_node->type);
                }
            }
            free(rhs_val_str);
            break;
        }
        case NODE_EXPRESSION_STMT: {
            char *expr_res = generate_expr_recursive(node->data.expression);
            if (expr_res) free(expr_res);
            break;
        }
        case NODE_RETURN_STMT:
            if (node->data.return_stmt.return_value) {
                char *ret_val = generate_expr_recursive(node->data.return_stmt.return_value);
                if (ret_val) {
                    emit_gen(OP_RETURN, ret_val, NULL, NULL, NULL);
                    free(ret_val);
                } else {
                     fprintf(stderr, "IR Gen Error (Return): Failed to generate expression for return value.\n");
                     emit_gen(OP_RETURN, NULL, NULL, NULL, NULL);
                }
            } else {
                emit_gen(OP_RETURN, NULL, NULL, NULL, NULL);
            }
            break;
        case NODE_BLOCK_STMT: {
            ASTNodeList *curr = node->data.statement_list;
            while (curr) {
                generate_stmt_recursive(curr->node);
                curr = curr->next;
            }
            break;
        }
        case NODE_IF_STMT: {
            char *cond_res = generate_expr_recursive(node->data.if_stmt.condition);
            if (!cond_res) {
                 fprintf(stderr, "IR Gen Error (IfStmt): Failed to generate condition expression.\n");
                 // Basic fallback, consider how to handle this better
                 char *elseLabel_err = new_label_gen(); char *endLabel_err = new_label_gen();
                 emit_gen(OP_LABEL, NULL, NULL, NULL, elseLabel_err);
                 generate_stmt_recursive(node->data.if_stmt.then_branch);
                 if(node->data.if_stmt.else_branch) generate_stmt_recursive(node->data.if_stmt.else_branch);
                 emit_gen(OP_LABEL, NULL, NULL, NULL, endLabel_err);
                 free(elseLabel_err); free(endLabel_err);
                 break;
            }
            char *elseLabel = new_label_gen();
            char *endLabel = new_label_gen();

            emit_gen(OP_IFGOTO, cond_res, "0", NULL, elseLabel);
            free(cond_res);

            generate_stmt_recursive(node->data.if_stmt.then_branch);
            emit_gen(OP_GOTO, NULL, NULL, NULL, endLabel);

            emit_gen(OP_LABEL, NULL, NULL, NULL, elseLabel);
            if (node->data.if_stmt.else_branch) {
                generate_stmt_recursive(node->data.if_stmt.else_branch);
            }
            emit_gen(OP_LABEL, NULL, NULL, NULL, endLabel);
            free(elseLabel);
            free(endLabel);
            break;
        }
        case NODE_LOOP_STMT: {
            char *startLabel = new_label_gen();
            char *endLabel = new_label_gen();

            emit_gen(OP_LABEL, NULL, NULL, NULL, startLabel);
            char *cond_res = generate_expr_recursive(node->data.loop_stmt.condition);
            if (!cond_res) {
                 fprintf(stderr, "IR Gen Error (LoopStmt): Failed to generate condition expression.\n");
                 free(startLabel); free(endLabel);
                 break;
            }
            emit_gen(OP_IFGOTO, cond_res, "0", NULL, endLabel);
            free(cond_res);

            generate_stmt_recursive(node->data.loop_stmt.body);
            emit_gen(OP_GOTO, NULL, NULL, NULL, startLabel);

            emit_gen(OP_LABEL, NULL, NULL, NULL, endLabel);
            free(startLabel);
            free(endLabel);
            break;
        }
        case NODE_FUNC_DEF:
            if (node->data.func_def.func_name) {
                emit_gen(OP_LABEL, NULL, NULL, NULL, node->data.func_def.func_name);
                generate_stmt_recursive(node->data.func_def.body);
            } else {
                fprintf(stderr, "IR Gen Error (FuncDef): Function name is NULL.\n");
            }
            break;
        // Catch-all for expression types used as statements if not wrapped by NODE_EXPRESSION_STMT
        // This might indicate an AST structure that could be improved (e.g., always wrap expressions used as statements)
        case NODE_FUNCTION_CALL:
        case NODE_ASSIGN_EXPR:
        case NODE_READ_NUMBER_CALL: {
             char *temp_res = generate_expr_recursive(node); // Expressions generate IR and return a temp
             if (temp_res) free(temp_res); // Result is discarded if used as a standalone statement
             break;
        }
        default:
            fprintf(stderr, "Warning: Unhandled statement type in IR generation: %d (line %d)\n", node->type, node->line_number);
            break;
    }
}

// Function to generate IR for expressions (recursive helper)
static char* generate_expr_recursive(ASTNode *node) {
    if (!node) {
        fprintf(stderr, "IR Gen Error: generate_expr_recursive called with NULL node.\n");
        return NULL;
    }

    switch (node->type) {
        case NODE_NUMBER_LITERAL: {
            char *tmp = new_temp_gen();
            char val_str[32];
            sprintf(val_str, "%d", node->data.number_literal);
            emit_gen(OP_ASSIGN, tmp, val_str, NULL, NULL);
            return tmp;
        }
        case NODE_STRING_LITERAL: {
            char *tmp = new_temp_gen();
            if (node->data.string_literal) {
                emit_gen(OP_ASSIGN, tmp, node->data.string_literal, NULL, NULL);
            } else {
                fprintf(stderr, "IR Gen Warning (StringLiteral): String literal data is NULL. Assigning empty string rep.\n");
                emit_gen(OP_ASSIGN, tmp, "\"\"", NULL, NULL);
            }
            return tmp;
        }
        case NODE_IDENTIFIER:
            if (node->data.identifier_name) {
                return strdup(node->data.identifier_name); // Value of identifier is itself (to be resolved later)
            } else {
                fprintf(stderr, "IR Gen Error (Identifier Expr): Identifier name is NULL.\n");
                char* dummy_err_temp = new_temp_gen();
                emit_gen(OP_ASSIGN, dummy_err_temp, "ERROR_NULL_IDENTIFIER_EXPR", NULL, NULL);
                return dummy_err_temp;
            }
        case NODE_BINARY_OP: {
            char *lhs_val_str = generate_expr_recursive(node->data.binary_op.left);
            char *rhs_val_str = generate_expr_recursive(node->data.binary_op.right);
            if (!lhs_val_str || !rhs_val_str) {
                fprintf(stderr, "IR Gen Error (BinaryOp %s): Failed to generate LHS or RHS.\n", node->data.binary_op.op ? node->data.binary_op.op : "UNK_OP");
                if(lhs_val_str) free(lhs_val_str);
                if(rhs_val_str) free(rhs_val_str);
                char* err_temp = new_temp_gen();
                emit_gen(OP_ASSIGN, err_temp, "ERROR_BINOP_OPERAND", NULL, NULL);
                return err_temp;
            }
            char *res_temp_name = new_temp_gen();
            IROpcode op_code;
            if (!node->data.binary_op.op) { /* ... error handling ... */ free(lhs_val_str); free(rhs_val_str); free(res_temp_name); return new_temp_gen(); }

            if (strcmp(node->data.binary_op.op, "+") == 0) op_code = OP_ADD;
            else if (strcmp(node->data.binary_op.op, "-") == 0) op_code = OP_SUB;
            else if (strcmp(node->data.binary_op.op, "*") == 0) op_code = OP_MUL;
            else if (strcmp(node->data.binary_op.op, "/") == 0) op_code = OP_DIV;
            else if (strcmp(node->data.binary_op.op, "%") == 0) op_code = OP_MOD;
            else if (strcmp(node->data.binary_op.op, "==") == 0) op_code = OP_EQ;
            else if (strcmp(node->data.binary_op.op, "!=") == 0) op_code = OP_NEQ;
            else if (strcmp(node->data.binary_op.op, "<") == 0) op_code = OP_LT;
            else if (strcmp(node->data.binary_op.op, "<=") == 0) op_code = OP_LE;
            else if (strcmp(node->data.binary_op.op, ">") == 0) op_code = OP_GT;
            else if (strcmp(node->data.binary_op.op, ">=") == 0) op_code = OP_GE;
            else { /* ... error handling ... */ emit_gen(OP_ASSIGN, res_temp_name, "ERROR_UNKNOWN_BINOP", NULL, NULL); free(lhs_val_str); free(rhs_val_str); return res_temp_name; }

            emit_gen(op_code, res_temp_name, lhs_val_str, rhs_val_str, NULL);
            free(lhs_val_str); free(rhs_val_str);
            return res_temp_name;
        }
        case NODE_UNARY_OP: {
            char *operand_val_str = generate_expr_recursive(node->data.unary_op.operand);
            if(!operand_val_str){ /* ... error handling ... */ return new_temp_gen(); }
            char *res_temp_name = new_temp_gen();
            if (!node->data.unary_op.op) { /* ... error handling ... */ free(operand_val_str); free(res_temp_name); return new_temp_gen(); }

            if (strcmp(node->data.unary_op.op, "-") == 0) emit_gen(OP_SUB, res_temp_name, "0", operand_val_str, NULL);
            else if (strcmp(node->data.unary_op.op, "+") == 0) emit_gen(OP_ASSIGN, res_temp_name, operand_val_str, NULL, NULL);
            else { /* ... error handling ... */ emit_gen(OP_ASSIGN, res_temp_name, "ERROR_UNKNOWN_UNARYOP", NULL, NULL); }
            free(operand_val_str);
            return res_temp_name;
        }
        case NODE_ASSIGN_EXPR: {
            char *rhs_val_str = generate_expr_recursive(node->data.assignment.rvalue);
            ASTNode* lhs_node = node->data.assignment.lvalue;
            char *expr_result_temp = new_temp_gen();

            if (!rhs_val_str || !lhs_node) { /* ... error handling ... */ if(rhs_val_str) free(rhs_val_str); emit_gen(OP_ASSIGN, expr_result_temp, "ERROR_ASSIGNEXPR_SETUP", NULL, NULL); return expr_result_temp; }

            if (lhs_node->type == NODE_IDENTIFIER && lhs_node->data.identifier_name) {
                emit_gen(OP_ASSIGN, lhs_node->data.identifier_name, rhs_val_str, NULL, NULL);
                emit_gen(OP_ASSIGN, expr_result_temp, rhs_val_str, NULL, NULL); // Value of expr is assigned value
            } else if (lhs_node->type == NODE_ARRAY_ACCESS) {
                ASTNode* base = lhs_node->data.array_access.array_name_or_expr;
                ASTNode* index_expr_node = lhs_node->data.array_access.index_expr;
                if (base && base->type == NODE_IDENTIFIER && base->data.identifier_name && index_expr_node) {
                    char *index_val_str = generate_expr_recursive(index_expr_node);
                    if (index_val_str) {
                        char target_str[128];
                        snprintf(target_str, sizeof(target_str), "%s[%s]", base->data.identifier_name, index_val_str);
                        emit_gen(OP_ASSIGN, target_str, rhs_val_str, NULL, NULL);
                        emit_gen(OP_ASSIGN, expr_result_temp, rhs_val_str, NULL, NULL);
                        free(index_val_str);
                    } else { /* ... error handling for index ... */ emit_gen(OP_ASSIGN, expr_result_temp, "ERROR_ASSIGNEXPR_IDX", NULL, NULL); }
                } else { /* ... error handling for array base ... */ emit_gen(OP_ASSIGN, expr_result_temp, "ERROR_ASSIGNEXPR_LHS_ARR", NULL, NULL); }
            } else { /* ... error handling for LHS type ... */ emit_gen(OP_ASSIGN, expr_result_temp, "ERROR_ASSIGNEXPR_LHS_TYPE", NULL, NULL); }
            free(rhs_val_str);
            return expr_result_temp;
        }
        case NODE_FUNCTION_CALL: {
            if (node->data.function_call.function_name_or_expr &&
                node->data.function_call.function_name_or_expr->type == NODE_IDENTIFIER &&
                node->data.function_call.function_name_or_expr->data.identifier_name) {
                const char* func_name = node->data.function_call.function_name_or_expr->data.identifier_name;
                if (strcmp(func_name, "print_number") == 0 || strcmp(func_name, "print_text") == 0) {
                    ASTNodeList *args = node->data.function_call.arg_list;
                    if (args && args->node) {
                        char *arg_val_str = generate_expr_recursive(args->node);
                        if (arg_val_str) { emit_gen(OP_PRINT, arg_val_str, NULL, NULL, NULL); free(arg_val_str); }
                        else { fprintf(stderr, "IR Gen Error (PrintCall Expr): Failed to gen arg for %s.\n", func_name); }
                    } // else warning for no args
                    char *dummy_ret_val = new_temp_gen();
                    emit_gen(OP_ASSIGN, dummy_ret_val, "0", NULL, NULL);
                    return dummy_ret_val;
                }
                ASTNodeList *arg_item = node->data.function_call.arg_list;
                while (arg_item) {
                    if (arg_item->node) {
                        char *arg_val_str = generate_expr_recursive(arg_item->node);
                        if (arg_val_str) { emit_gen(OP_PARAM, arg_val_str, NULL, NULL, NULL); free(arg_val_str); }
                        else { fprintf(stderr, "IR Gen Error (FuncCall): Failed to generate argument for %s.\n", func_name); }
                    }
                    arg_item = arg_item->next;
                }
                char *res_temp_name = new_temp_gen();
                emit_gen(OP_CALL, res_temp_name, func_name, NULL, NULL);
                return res_temp_name;
            } else { /* ... error handling for func name ... */ return new_temp_gen(); }
        }
        case NODE_READ_NUMBER_CALL: {
            char *res_temp_name = new_temp_gen();
            emit_gen(OP_READ, res_temp_name, NULL, NULL, NULL);
            return res_temp_name;
        }
        case NODE_ARRAY_ACCESS: {
            ASTNode* base_node = node->data.array_access.array_name_or_expr;
            ASTNode* index_expr_node = node->data.array_access.index_expr;
            if (!base_node || base_node->type != NODE_IDENTIFIER || !base_node->data.identifier_name || !index_expr_node) {
                /* ... error handling ... */ return new_temp_gen();
            }
            char *index_val_str = generate_expr_recursive(index_expr_node);
            if (!index_val_str) { /* ... error handling ... */ return new_temp_gen(); }
            char *res_temp_name = new_temp_gen();
            char array_access_str[128];
            snprintf(array_access_str, sizeof(array_access_str), "%s[%s]", base_node->data.identifier_name, index_val_str);
            emit_gen(OP_ASSIGN, res_temp_name, array_access_str, NULL, NULL);
            free(index_val_str);
            return res_temp_name;
        }
        default:
            fprintf(stderr, "Warning: Unhandled expression type in IR generation: %d (line %d)\n", node->type, node->line_number);
            char* err_temp = new_temp_gen();
            emit_gen(OP_ASSIGN, err_temp, "ERROR_UNHANDLED_EXPR_TYPE", NULL, NULL);
            return err_temp;
    }
}

// Main function to start IR generation
IRList* generate_ir(ASTNode *root) {
    ir_head_gen = NULL;
    ir_tail_gen = NULL;
    temp_count_gen = 0;
    label_count_gen = 0;

    if (!root) {
        fprintf(stderr, "IR Gen Error: Root ASTNode is NULL for generate_ir.\n");
        return NULL;
    }
    generate_stmt_recursive(root);

    return ir_head_gen;
}

// Helper to free fields of a single IR instruction (used by free_ir)
static void free_single_ir_instruction_fields_util(IRInstruction *instr) {
    if (!instr) return;
    free(instr->result); instr->result = NULL;
    free(instr->arg1);   instr->arg1 = NULL;
    free(instr->arg2);   instr->arg2 = NULL;
    free(instr->label);  instr->label = NULL;
}

// Function to print the generated IR instructions
void print_ir(IRList *list) {
    IRInstruction *i;
    // printf("Generated Intermediate Representation (3-address code):\n"); // Title printed by caller
    if (!list) {
        printf("(No IR instructions to print)\n");
        return;
    }
    while (list) {
        i = list->instr;
        if (!i) {
            printf("Error: Found NULL instruction in IRList during print_ir.\n");
            list = list->next;
            continue;
        }
        switch (i->op) {
            case OP_ADD: printf("%s = %s + %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_SUB: printf("%s = %s - %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_MUL: printf("%s = %s * %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_DIV: printf("%s = %s / %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_MOD: printf("%s = %s %% %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_ASSIGN: printf("%s = %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1"); break;
            case OP_LABEL: printf("%s:\n", i->label ? i->label : "null_label"); break;
            case OP_GOTO: printf("goto %s\n", i->label ? i->label : "null_label"); break;
            case OP_IFGOTO: printf("if %s == %s goto %s\n", i->result ? i->result : "null_cond", i->arg1 ? i->arg1 : "0" /*expected cmp val*/, i->label ? i->label : "null_label"); break;
            case OP_PARAM: printf("param %s\n", i->result ? i->result : "null_param"); break;
            case OP_CALL: printf("%s = call %s()\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_func_name" ); break;
            case OP_RETURN: printf("return %s\n", i->result ? i->result : "" ); break;
            case OP_READ: printf("%s = read_number()\n", i->result ? i->result : "null_res"); break;
            case OP_PRINT: printf("print %s\n", i->result ? i->result : "null_print_val"); break;
            case OP_EQ: printf("%s = %s == %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_NEQ: printf("%s = %s != %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_LT: printf("%s = %s < %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_LE: printf("%s = %s <= %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_GT: printf("%s = %s > %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            case OP_GE: printf("%s = %s >= %s\n", i->result ? i->result : "null_res", i->arg1 ? i->arg1 : "null_arg1", i->arg2 ? i->arg2 : "null_arg2"); break;
            default: printf("Unknown IR Opcode in print_ir: %d\n", i->op); break;
        }
        list = list->next;
    }
}

// Function to free all allocated memory for IR instructions
void free_ir(IRList *list) {
    IRList *tmp;
    while (list) {
        tmp = list;
        IRInstruction *instr = list->instr;
        if (instr) {
            free_single_ir_instruction_fields_util(instr);
            free(instr);
        }
        list = list->next;
        free(tmp);
    }
}