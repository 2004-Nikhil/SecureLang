// ir_core.h
#ifndef IR_CORE_H
#define IR_CORE_H

// No need to include ast.h here if IRInstruction/IRList don't directly hold ASTNode pointers
// (which they currently don't)

typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_ASSIGN,
    OP_LABEL, OP_GOTO, OP_IFGOTO,
    OP_PARAM, OP_CALL, OP_RETURN,
    OP_READ, OP_PRINT,
    OP_EQ, OP_NEQ, OP_LT, OP_LE, OP_GT, OP_GE
} IROpcode;

typedef struct IRInstruction {
    IROpcode op;
    char *result;
    char *arg1;
    char *arg2;
    char *label;
} IRInstruction;

typedef struct IRList {
    IRInstruction *instr;
    struct IRList *next;
} IRList;

#endif // IR_CORE_H