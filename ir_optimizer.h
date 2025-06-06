// ir_optimizer.h
#ifndef IR_OPTIMIZER_H
#define IR_OPTIMIZER_H

#include "ir_core.h" // For IRList type

// Function declaration for IR optimization
IRList* optimize_ir(IRList *list);

#endif // IR_OPTIMIZER_H