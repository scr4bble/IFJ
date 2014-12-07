#include "interpreter.h"

/**
 * "Global" tape containing instructions
 */
InstructionVector *tape;
extern int instr_counter;

/**
 * Pointer to instruction tape at beginning
 */
uint64_t IP = 0;

Stack stack;

void initInterpret() {
	stack.vect = StackDataVectorInit(DEFAULT_STACK_SIZE);

	// TODO: Test optimal initial size
	if(!tape)
		tape = InstructionVectorInit(512);
}

/**
 * CORE
 */

void generateExprInstruction(InstructionOp op, Operand* a, Operand* b, Operand* c) {

	instr_counter++;
	int instruction_offset = op << 8 | b->var_type << 6 | c->var_type << 4 |
						b->data_type << 2 | c->data_type;

	assert(instruction_table[instruction_offset]);
	Instruction i = (Instruction) {.instr = instruction_table[instruction_offset],
										 .dst = *a, .src_1 = *b, .src_2 = *c};
	InstructionVectorAppend(tape, i);
}

void generateInstruction(InstructionOp op, Operand* a, Operand* b) {

	instr_counter++;
	InstrFuncPtr i_ptr = NULL;

	fprintf(stderr, "OP : %d\n", op);

	switch(op) {
		case PUSH:
			switch(b->var_type)
			{
				case LOCAL:
					switch(b->data_type)
					{
						case STRING:
							i_ptr = Instr_PUSH_LS;
							break;
						case DOUBLE:
							i_ptr = Instr_PUSH_LD;
							break;
						case INT:
							i_ptr = Instr_PUSH_LI;
							break;
						case BOOL:
							i_ptr = Instr_PUSH_LB;
							break;
						default:
							break;
					}
					break;
				case GLOBAL:
					switch(b->data_type)
					{
						case STRING:
							i_ptr = Instr_PUSH_GS;
							break;
						case DOUBLE:
							i_ptr = Instr_PUSH_GD;
							break;
						case INT:
							i_ptr = Instr_PUSH_GI;
							break;
						case BOOL:
							i_ptr = Instr_PUSH_GB;
							break;
						default:
							break;
					}
					break;
				case CONST:
					switch(b->data_type)
					{
						case STRING:
							i_ptr = Instr_PUSH_CS;
							break;
						case DOUBLE:
							i_ptr = Instr_PUSH_CD;
							break;
						case INT:
							i_ptr = Instr_PUSH_CI;
							break;
						case BOOL:
							i_ptr = Instr_PUSH_CB;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			break;
		case JMP_T:
			i_ptr = Instr_JMP_T;
			break;
		case JMP_F:
			i_ptr = Instr_JMP_F;
			break;
		case JMP:
			i_ptr = Instr_JMP;
			break;
		case CALL:
			i_ptr = Instr_CALL;
			break;
		case CALL_LENGTH:
			i_ptr = Instr_CALL_LENGTH;
			break;
		case CALL_COPY:
			i_ptr = Instr_CALL_COPY;
			break;
		case CALL_FIND:
			i_ptr = Instr_CALL_FIND;
			break;
		case CALL_SORT:
			i_ptr = Instr_CALL_SORT;
			break;
		case RET:
			i_ptr = Instr_RET;
			break;
		case MOV:
			switch(b->data_type)
			{
				case STRING:
					i_ptr = Instr_MOV_GS;
					break;
				case DOUBLE:
					i_ptr = Instr_MOV_GD;
					break;
				case INT:
					i_ptr = Instr_MOV_GI;
					break;
				case BOOL:
					i_ptr = Instr_MOV_GB;
					break;
				default:
					break;
			}
			break;
		case READLN:
			if (a->var_type == LOCAL) {
				switch(b->data_type)
				{
					case STRING:
						i_ptr = Instr_READLN_GS;
						break;
					case DOUBLE:
						i_ptr = Instr_READLN_GD;
						break;
					case INT:
						i_ptr = Instr_READLN_GI;
						break;
					default:
						break;
				}
			}
			if (a->var_type == GLOBAL) {
				switch(b->data_type)
				{
					case STRING:
						i_ptr = Instr_READLN_LS;
						break;
					case DOUBLE:
						i_ptr = Instr_READLN_LD;
						break;
					case INT:
						i_ptr = Instr_READLN_LI;
						break;
					default:
						break;
				}
			}
		case WRITE:
			i_ptr = Instr_WRITE;
			break;
		case HALT:
			i_ptr = Instr_HALT;
			break;
		default:
			i_ptr = Instr_CALL;
			break;
	}

	Instruction i = (Instruction) {.instr = i_ptr, .dst = *a, .src_1 = *b};
	InstructionVectorAppend(tape, i);
}

void runInterpretation() {
	assert(tape && "Call initInterpret() before running interpreter");

	Instruction *first = InstructionVectorFirst(tape);
	while(true) {
		Instruction *i = first + IP;
		(i->instr)(i);
		IP++;
	}
}
