#include "system.h"
#include "error.h"
#include "string.h"

#ifndef _STACK_H
#define _STACK_H

#define NUMBER_OF_CONTEXT_INFO 2 // pointer to stact and instruction tape
#define DEFAULT_STACK_SIZE 64
#define StackInit(stack)			\
			stack.Position = 0;		\
			stack.Vec = StackDataInitVector(DEFAULT_STACK_SIZE)
#define StackFree(stack) StackDataVectorFree(stack.Vec)

	// stack structure

	typedef union
	{
		int Sint;
		double Sdouble;
		String Sstring;
	} StackData;

	typedef struct Vector StackDataVector;

#include "symbol.h"
#include "vector.h"

	typedef struct
	{
		uint32_t Position;
		StackDataVector *Vec;
	} STACK;


/**
 * prepare it for global symbols
 * @param  stack stack
 * @param  Fun   name of Global Context
 * @return       true if all OK
 */
bool LoadGlobalContext(STACK *stack, Context *Fun);

/**
 * Prepare stack for new function
 * @param  stack pointer to stack
 * @param  var   number of local arguments
 * @param  inst  current index in instruction tape
 * @return       true if all OK
 */
bool CallFunction(STACK *stack, uint32_t var, uint32_t inst);

// param: number of argument, local variables,
// pointer to index in inst tape and return StackData value
/**
 * Return from the function
 * @param  stack pointer to stack
 * @param  arg   Number of arguments in function
 * @param  var   Number of local variables in function
 * @param  inst  pointer to InstPointer to change it to new value
 * @param  data  Return data
 * @return       true if all OK
 */
bool ReturnFunction(STACK *stack, uint32_t arg, uint32_t var, uint32_t* inst, StackData data);

// StackData to write, int index - negativ represents argument
/**
 * Set value in stack (doesn't control range)
 * @param stack pointer to stack
 * @param data  new data
 * @param index index where variable is(negativ-arg, positive-locVar)
 */
void StackSetValue(STACK *stack, StackData data, int index);

/**
 * Read value from stack
 * @param  stack pointer to stack
 * @param  index index where variable is(negativ-arg, positive-locVar)
 * @return       value of variable
 */
StackData *StackReadValue(STACK *stack, int index);

#endif
