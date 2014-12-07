#include "system.h"
#include "instruction.h"
#include "stack.h"
#include "memory_mgmt.h"

#ifndef _INSTRUCTIONS_GENERATED_H
#define _INSTRUCTIONS_GENERATED_H
void Instr_NEG_LxIx(Instruction *i);
void Instr_NEG_LxDx(Instruction *i);
void Instr_NEG_GxIx(Instruction *i);
void Instr_NEG_GxDx(Instruction *i);
void Instr_NOT_LxBx(Instruction *i);
void Instr_NOT_GxBx(Instruction *i);
void Instr_MUL_CLII(Instruction *i);
void Instr_MUL_CLID(Instruction *i);
void Instr_MUL_CLDI(Instruction *i);
void Instr_MUL_CLDD(Instruction *i);
void Instr_MUL_CGII(Instruction *i);
void Instr_MUL_CGID(Instruction *i);
void Instr_MUL_CGDI(Instruction *i);
void Instr_MUL_CGDD(Instruction *i);
void Instr_DIV_CLII(Instruction *i);
void Instr_DIV_CLID(Instruction *i);
void Instr_DIV_CLDI(Instruction *i);
void Instr_DIV_CLDD(Instruction *i);
void Instr_DIV_CGII(Instruction *i);
void Instr_DIV_CGID(Instruction *i);
void Instr_DIV_CGDI(Instruction *i);
void Instr_DIV_CGDD(Instruction *i);
void Instr_AND_CLBB(Instruction *i);
void Instr_AND_CGBB(Instruction *i);
void Instr_ADD_CLII(Instruction *i);
void Instr_ADD_CLID(Instruction *i);
void Instr_ADD_CLDI(Instruction *i);
void Instr_ADD_CLDD(Instruction *i);
void Instr_ADD_CLSS(Instruction *i);
void Instr_ADD_CGII(Instruction *i);
void Instr_ADD_CGID(Instruction *i);
void Instr_ADD_CGDI(Instruction *i);
void Instr_ADD_CGDD(Instruction *i);
void Instr_ADD_CGSS(Instruction *i);
void Instr_ADD_LCSS(Instruction *i);
void Instr_ADD_LLSS(Instruction *i);
void Instr_ADD_LGSS(Instruction *i);
void Instr_ADD_GCSS(Instruction *i);
void Instr_ADD_GLSS(Instruction *i);
void Instr_ADD_GGSS(Instruction *i);
void Instr_SUB_CLII(Instruction *i);
void Instr_SUB_CLID(Instruction *i);
void Instr_SUB_CLDI(Instruction *i);
void Instr_SUB_CLDD(Instruction *i);
void Instr_SUB_CGII(Instruction *i);
void Instr_SUB_CGID(Instruction *i);
void Instr_SUB_CGDI(Instruction *i);
void Instr_SUB_CGDD(Instruction *i);
void Instr_OR_CLBB(Instruction *i);
void Instr_OR_CGBB(Instruction *i);
void Instr_XOR_CLBB(Instruction *i);
void Instr_XOR_CGBB(Instruction *i);
void Instr_L_CLII(Instruction *i);
void Instr_L_CLDD(Instruction *i);
void Instr_L_CLBB(Instruction *i);
void Instr_L_CLSS(Instruction *i);
void Instr_L_CGII(Instruction *i);
void Instr_L_CGDD(Instruction *i);
void Instr_L_CGBB(Instruction *i);
void Instr_L_CGSS(Instruction *i);
void Instr_L_LCSS(Instruction *i);
void Instr_L_LLSS(Instruction *i);
void Instr_L_LGSS(Instruction *i);
void Instr_L_GCSS(Instruction *i);
void Instr_L_GLSS(Instruction *i);
void Instr_L_GGSS(Instruction *i);
void Instr_G_CLII(Instruction *i);
void Instr_G_CLDD(Instruction *i);
void Instr_G_CLBB(Instruction *i);
void Instr_G_CLSS(Instruction *i);
void Instr_G_CGII(Instruction *i);
void Instr_G_CGDD(Instruction *i);
void Instr_G_CGBB(Instruction *i);
void Instr_G_CGSS(Instruction *i);
void Instr_G_LCSS(Instruction *i);
void Instr_G_LLSS(Instruction *i);
void Instr_G_LGSS(Instruction *i);
void Instr_G_GCSS(Instruction *i);
void Instr_G_GLSS(Instruction *i);
void Instr_G_GGSS(Instruction *i);
void Instr_LE_CLII(Instruction *i);
void Instr_LE_CLDD(Instruction *i);
void Instr_LE_CLBB(Instruction *i);
void Instr_LE_CLSS(Instruction *i);
void Instr_LE_CGII(Instruction *i);
void Instr_LE_CGDD(Instruction *i);
void Instr_LE_CGBB(Instruction *i);
void Instr_LE_CGSS(Instruction *i);
void Instr_LE_LCSS(Instruction *i);
void Instr_LE_LLSS(Instruction *i);
void Instr_LE_LGSS(Instruction *i);
void Instr_LE_GCSS(Instruction *i);
void Instr_LE_GLSS(Instruction *i);
void Instr_LE_GGSS(Instruction *i);
void Instr_GE_CLII(Instruction *i);
void Instr_GE_CLDD(Instruction *i);
void Instr_GE_CLBB(Instruction *i);
void Instr_GE_CLSS(Instruction *i);
void Instr_GE_CGII(Instruction *i);
void Instr_GE_CGDD(Instruction *i);
void Instr_GE_CGBB(Instruction *i);
void Instr_GE_CGSS(Instruction *i);
void Instr_GE_LCSS(Instruction *i);
void Instr_GE_LLSS(Instruction *i);
void Instr_GE_LGSS(Instruction *i);
void Instr_GE_GCSS(Instruction *i);
void Instr_GE_GLSS(Instruction *i);
void Instr_GE_GGSS(Instruction *i);
void Instr_EQ_CLII(Instruction *i);
void Instr_EQ_CLDD(Instruction *i);
void Instr_EQ_CLBB(Instruction *i);
void Instr_EQ_CLSS(Instruction *i);
void Instr_EQ_CGII(Instruction *i);
void Instr_EQ_CGDD(Instruction *i);
void Instr_EQ_CGBB(Instruction *i);
void Instr_EQ_CGSS(Instruction *i);
void Instr_EQ_LCSS(Instruction *i);
void Instr_EQ_LLSS(Instruction *i);
void Instr_EQ_LGSS(Instruction *i);
void Instr_EQ_GCSS(Instruction *i);
void Instr_EQ_GLSS(Instruction *i);
void Instr_EQ_GGSS(Instruction *i);
void Instr_NE_CLII(Instruction *i);
void Instr_NE_CLDD(Instruction *i);
void Instr_NE_CLBB(Instruction *i);
void Instr_NE_CLSS(Instruction *i);
void Instr_NE_CGII(Instruction *i);
void Instr_NE_CGDD(Instruction *i);
void Instr_NE_CGBB(Instruction *i);
void Instr_NE_CGSS(Instruction *i);
void Instr_NE_LCSS(Instruction *i);
void Instr_NE_LLSS(Instruction *i);
void Instr_NE_LGSS(Instruction *i);
void Instr_NE_GCSS(Instruction *i);
void Instr_NE_GLSS(Instruction *i);
void Instr_NE_GGSS(Instruction *i);

const InstrFuncPtr instruction_table[3744];

#endif
