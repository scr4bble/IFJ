/*
 * Project name:
 * Implementace interpretu imperativního jazyka IFJ14
 *
 * Repository:
 * https://github.com/Dasio/IFJ
 *
 * Team:
 * Dávid Mikuš			(xmikus15)
 * Peter Hostačný		(xhosta03)
 * Tomáš Kello			(xkello00)
 * Adam Lučanský		(xlucan01)
 * Michaela Lukášová		(xlukas09)
 */

#include "parser.h"

TokenVector *tokenVector;
Token *token;
Context *mainContext;
Context *funcContext;
Context *activeContext;
Symbol *funcSymbol;
extern InstructionVector *tape;
extern int64_t MY_OFFSET;
extern int64_t IP;
static int64_t mainOffset;
static int64_t funcOffset;
static int64_t *activeOffset;
static Operand a,b,c;
bool genStart = false;
uint8_t inGST = 0;
int32_t argIndex;

void parse(TokenVector *tokvect)
{
	tokenVector = tokvect;

	mainContext = InitContext();
	activeContext = mainContext;
	activeOffset = &mainOffset;
	token = TokenVectorFirst(tokenVector);
	addBuiltInFunctions();
	mainOffset = 1;
	generateInstruction(HALT, &a ,&b, &c);

	program();
	if(!getError())
		generateInstruction(HALT, &a, &b, &c);
	// Cleanup

	FreeContext(mainContext);
}
void program()
{
	var_declr();
	if(getError())
		return;

	func();
	if(getError())
		return;

	compound_stmt(0);
	if(getError())
		return;

	token++;
	if(token->type != TT_dot)
	{
		setError(ERR_Syntax);
		return;
	}
	// Check if every token was parsed
	token++;
	if(token->type != TT_empty)
	{
		setError(ERR_Syntax);
		return;
	}

	checkFuncDefinitions();

}
void var_declr()
{

	// Epsilon rule
	if(token->type != TT_keyword || token->keyword_token != Key_var)
		return;
	// literal 'var' loaded
	var_def(0);
	if(getError())
		return;
}
void var_def(uint8_t next)
{
	char *name;
	SymbolType symbolType;
	token++;
	if(token->type != TT_identifier)
	{
		// Epsilon rule - VAR_DEFN - just return, no error
		if(!next)
			setError(ERR_Syntax);
		return;
	}
	name = token->str.data;
	token++;
	if(token->type != TT_colon)
	{
		setError(ERR_Syntax);
		return;
	}

	// Check if token is type
	token++;
	if(token->type != TT_keyword)
	{
		setError(ERR_Syntax);
		return;
	}
	switch(token->keyword_token)
	{
		case Key_boolean:
		case Key_integer:
		case Key_real:
		case Key_string:
			symbolType = keywordToSymbol(token->keyword_token);
			break;
		default:
			setError(ERR_Syntax);
			return;
	}

	token++;
	if(token->type != TT_semicolon)
	{
		setError(ERR_Syntax);
		return;
	}
	// Add variable to symbol table
	SymbolAdd(activeContext, symbolType, name, NULL, NULL, false);

	if(getError())
		return;
	var_def(1);
	if(getError())
		return;

}
void func()
{
	// Epsilon rule, just return, no error
	if(token->type != TT_keyword || token->keyword_token != Key_function)
		return;
	// keyword 'function' loaded
	SymbolType returnType;
	token++;
	if(token->type != TT_identifier)
	{
		setError(ERR_Syntax);
		return;
	}
	addFunc(token->str.data);
	if(getError())
		return;
	param_def_list();
	if(getError())
		return;

	token++;
	if(token->type != TT_colon)
	{
		setError(ERR_Syntax);
		return;
	}

	token++;
	//Check if token is type
	if(token->type != TT_keyword)
	{
		setError(ERR_Syntax);
		return;
	}
	switch(token->keyword_token)
	{
		case Key_boolean:
		case Key_integer:
		case Key_real:
		case Key_string:
			returnType = keywordToSymbol(token->keyword_token);
			break;
		default:
			setError(ERR_Syntax);
			return;
	}

	token++;
	if(token->type != TT_semicolon)
	{
		setError(ERR_Syntax);
		return;
	}

	forward(returnType);
	if(getError())
		return;
	if(inGST && funcContext->argCount != argIndex)
	{
		setError(ERR_BadDefArg);
		return;
	}
	func();
	if(getError())
		return;
}
void forward(SymbolType returnType)
{
	token++;
	// 1. rule = Function Forward Declaration
	if(token->type == TT_keyword && token->keyword_token == Key_forward)
	{
		token++;
		if(token->type != TT_semicolon)
		{
			setError(ERR_Syntax);
			return;
		}
		// Add declaration of function to GST
		updateFunc(returnType,FS_Declared);
		if(getError())
			return;
	}
	else // 2. rule = Function Definition
	{
		// Add definition of function to GST
		updateFunc(returnType,FS_Defined);
		if(getError())
			return;
		// Switch to function context
		activeContext = funcContext;
		activeOffset = &funcOffset;
		var_declr();
		if(getError())
			return;
		*activeOffset = activeContext->locCount + 2;
		compound_stmt(1);
		if(getError())
			return;

		generateInstruction(RET, &a, &b, &c);
		// Switch back to main context
		activeContext = mainContext;
		activeOffset = &mainOffset;
	}
	// Need load next token
	token++;
}
void param_def_list()
{
	token++;
	if(token->type != TT_leftBrace)
	{
		setError(ERR_Syntax);
		return;
	}

	params_def(0);
	if(getError())
		return;

	// Token loaded from params
	if(token->type != TT_rightBrace)
	{
		setError(ERR_Syntax);
		return;
	}
}
void params_def(uint8_t next)
{
	token++;
	// Epsilon rule
	if(!next && token->type != TT_identifier)
		return;
	if(next && token->type != TT_semicolon)
		return;
	// Get from PARAMS_DEF_N to PARAMS_DEF to simplify code
	// semicolon loaded, now we need load identifier
	if(next)
	{
		token++;
		if(token->type != TT_identifier)
		{
			setError(ERR_Syntax);
			return;
		}
	}
	char *name = token->str.data;
	SymbolType symbolType;
	token++;
	if(token->type != TT_colon)
	{
		setError(ERR_Syntax);
		return;
	}

	token++;
	if(token->type != TT_keyword)
	{
		setError(ERR_Syntax);
		return;
	}
	switch(token->keyword_token)
	{
		case Key_boolean:
		case Key_integer:
		case Key_real:
		case Key_string:
			symbolType = keywordToSymbol(token->keyword_token);
			break;
		default:
			setError(ERR_Syntax);
			return;
	}
	addArgToFunc(symbolType,name);
	if(getError())
		return;
	params_def(1);
	if(getError())
		return;

}
uint32_t term_list()
{
	VariableType scope = GLOBAL;
	Symbol *symbol = NULL;
	uint32_t count = 0;
	token++;
	if(token->type != TT_leftBrace)
	{
		setError(ERR_Syntax);
		return 0;
	}
	Token *first = token+1;
	count += terms(0);
	if(getError())
		return 0;
	Token *current = token-1;
	while(current>=first)
	{
		a.offset = (*activeOffset)++;
		if(current->type == TT_identifier)
		{
			symbol = findVar(current->str.data,&scope);
			if(getError())
				return 0;
			b.var_type = scope;
			b.offset = symbol->index;
			switch(symbol->type)
			{
				case T_String:
					b.data_type = STRING;
					break;
				case T_double:
					b.data_type = DOUBLE;
					break;
				case T_int:
					b.data_type = INT;
					break;
				case T_bool:
					b.data_type = BOOL;
					break;
				default:
					break;
			}
		}
		else
		{
			b.var_type = CONST;
			switch(current->type)
			{
				case TT_string:
					b.data_type = STRING;
					b.str = &(current->str);
					break;
				case TT_real:
					b.data_type = DOUBLE;
					b.double_ = current->r;
					break;
				case TT_integer:
					b.data_type = INT;
					b.int_ = current->n;
					break;
				case TT_bool:
					b.data_type = BOOL;
					b.bool_ = (bool)current->n;
					break;
				default:
					break;
			}
		}
		//fprintf(stderr,"PUSH a.offset = %ld b.var_type = %d b.offset= %ld b.data_type= %d\n",a.offset,b.var_type,b.offset,b.data_type);
		generateInstruction(PUSHX, &a, &b, &c);
		// Skip comma and move to next argument
		current -= 2;
	}
	// Token loaded from terms
	if(token->type != TT_rightBrace)
	{
		setError(ERR_Syntax);
		return 0;
	}
	return count;
}
uint8_t terms(uint8_t next)
{
	if(next)
	{
		token++;
		// Epsilon rule
		if(token->type != TT_comma)
			return 0;
	}
	uint32_t count = 0;
	token++;
	// If token is not term(bool,int,double,string) or id
	if(token->type < TT_identifier || token->type > TT_bool)
	{
		setError(ERR_Syntax);
		return 0;
	}
	count = terms(1);
	if(getError())
		return 0;
	return count+1;
}
void compound_stmt(uint8_t semicolon)
{
	//fprintf(stderr,"Active %ld\n",*activeOffset);
	if(token->type != TT_keyword || token->keyword_token != Key_begin)
	{
		setError(ERR_Syntax);
		return;
	}
	if(activeContext == mainContext && !genStart)
	{
		funcContext = NULL;
		genStart = true;
		IP = tape->used;
		a.offset = 0;
		b.initialized = false;
		b.var_type = CONST;
		generateInstruction(PUSH, &a, &b, &c);

		b.var_type = CONST;
		b.data_type = INT; //doesnt matter
		for(int32_t i=0;i<mainContext->locCount;i++)
		{
			a.offset = (*activeOffset)++;
			generateInstruction(PUSH, &a, &b, &c);
		}
	}
	stmt_empty();
	if(getError())
		return;

	// token loaded from stmt()

	if(token->type != TT_keyword || token->keyword_token != Key_end)
	{
		setError(ERR_Syntax);
		return;
	}

	if(semicolon)
	{
		token++;
		if(token->type != TT_semicolon)
		{
			setError(ERR_Syntax);
			return;
		}
	}
}

void stmt_list()
{
	// token loaded from stmt()
	// epsilon rule
	if(token->type != TT_semicolon)
		return;

	stmt(0);
	if(getError())
		return;

	stmt_list();
	if(getError())
		return;
}
void stmt_empty()
{
	token++;
	// Check epsilon rule
	uint8_t epsilon = stmt(1);
	if(getError())
		return;
	// Epsilon rule was used
	if(epsilon)
		return;

	stmt_list();
	if(getError())
		return;

}
uint8_t stmt(uint8_t empty)
{
	uint8_t epsilon = 0;
	VariableType scope = GLOBAL;
	Symbol *id = NULL;
	DataType exprType;
	Instruction *instruction;
	int64_t repeat1;
	// IF wasnt called from stmt_empty(), need to load next token
	if(!empty) token++;
	switch(token->type)
	{
		// 1. rule = Assignemnt
		case TT_identifier:
			// Check if was declared
			id = findVar(token->str.data,&scope);
			if(getError() != ERR_None && getError() != ERR_FuncAsVar)
				return 0;
			token++;
			if(token->type != TT_assignment)
			{
				setError(ERR_Syntax);
				return 0;
			}
			// Check for ERR_FuncAsVar
			if(getError())
				return 0;
			exprType = expr();
			if(getError())
				return 0;
			if((DataType)id->type != exprType)
			{
				setError(ERR_TypeCompatibility);
				return 0;
			}
			// INST global=mov, local=prepisovat predchadzajucu instrukciou
			if(scope == GLOBAL)
			{
				a.offset = id->index;
				b.data_type = exprType;
				c.offset = MY_OFFSET - 1;
				generateInstruction(MOV, &a, &b, &c);
			}
			// Local
			else
			{
				instruction = InstructionVectorLast(tape);
				instruction->dst.offset = id->index;
			}

			token--;
			break;
		case TT_keyword:
			switch(token->keyword_token)
			{
				// 2. rule = IF Statement
				case Key_if:
					exprType = expr();
					if(getError())
						return 0;
					if(exprType != BOOL)
					{
						setError(ERR_TypeCompatibility);
						return 0;
					}
					if(token->type != TT_keyword || token->keyword_token != Key_then)
					{
						setError(ERR_Syntax);
						return 0;
					}
					// gen instruction if1 JMP_FALSE without adress
					// compound_stmt expect already loaded token
					b.offset = MY_OFFSET - 1;
					generateInstruction(JMP_F, &a, &b, &c);
					Instruction *if1 = InstructionVectorLast(tape);
					token++;
					compound_stmt(0);
					//gen empty instruction if2 JMP without adress
					b.offset = MY_OFFSET - 1;
					generateInstruction(JMP, &a, &b, &c);
					Instruction *if2 = InstructionVectorLast(tape);
					if(getError())
						return 0;

					epsilon = if_n(if1,if2);
					if(getError())
						return 0;
					break;
				// 3. rule = While cycle
				case Key_while:
					; // hackfix ... can't declare variable after case - wtf ? :D
					int64_t while1_index = tape->used - 1;
					exprType = expr();
					if(getError())
						return 0;
					if(exprType != BOOL)
					{
						setError(ERR_TypeCompatibility);
						return 0;
					}
					// gen instruction JMP_FALSE while1
					b.offset = MY_OFFSET - 1;
					generateInstruction(JMP_F, &a, &b, &c);
					Instruction *while1 = InstructionVectorLast(tape);

					if(token->type != TT_keyword || token->keyword_token != Key_do)
					{
						setError(ERR_Syntax);
						return 0;
					}
					// compound_stmt expect already loaded token
					token++;
					compound_stmt(0);
					// gen instruction JMP to while1
					// update inst while1 with vectorsize+1
					a.offset = while1_index;
					b.offset = MY_OFFSET - 1;
					generateInstruction(JMP, &a, &b, &c);
					while1->dst.offset = tape->used - 1;
					//fprintf(stderr,"w1=%ld w2=%ld\n",while1_index,while1->dst.offset);
					if(getError())
						return 0;
					break;
				case Key_repeat:
					// save vectorsize = repeat1
					repeat1 = tape->used - 1;
					stmt(0);
					if(getError())
						return 0;
					stmt_list();
					if(getError())
						return 0;
					if(token->type != TT_keyword || token->keyword_token != Key_until)
					{
						setError(ERR_Syntax);
						return 0;
					}
					exprType = expr();
					if(getError())
						return 0;
					if(exprType != BOOL)
					{
						setError(ERR_TypeCompatibility);
						return 0;
					}
					// gen instruction JMP_FALSE to repeat1
					a.offset = repeat1;
					b.offset = MY_OFFSET - 1;
					generateInstruction(JMP_F, &a, &b, &c);
					token--;
					break;
				case Key_begin:
					compound_stmt(0);
					if (getError())
						return 0;
					break;
				case Key_readln:
					readln();
					if(getError())
						return 0;
					break;
				case Key_write:
					write();
					if(getError())
						return 0;
					break;
				default:
					if(empty) return 1;
					else
					{
						setError(ERR_Syntax);
						break;
					}
			}
			break;
		default:
			// Epsilon rule used
			if(empty) return 1;
			else
			{
				setError(ERR_Syntax);
				break;
			}
	}
	// Need to load next token for stmt_list() if wasnt loaded before by epsilon rule
	if(!epsilon) token++;
	return 0;
}
uint8_t if_n(Instruction *if1, Instruction *if2)
{
	token++;
	// Epsilon rule
	// update instruction if1 with vectorsize+1
	// change if2 to NOP

	if(token->type != TT_keyword || token->keyword_token != Key_else)
	{
		InstructionVectorPop(tape);
		if1->dst.offset = tape->used - 1;
		return 1;
	}
	// keyword 'else' loaded
	token++;
	// update instruction if1 with vectorsize+1

	if1->dst.offset = tape->used - 1;
	// Compound_stmt expect loaded next token
	compound_stmt(0);
	// update instruction if2 with vectorsize+1
	if(getError())
		return 0;
	if2->dst.offset = tape->used - 1;
	return 0;
}

void readln()
{
	VariableType scope = GLOBAL;
	// keyword readln already loaded from STMT
	token++;
	if(token->type != TT_leftBrace)
	{
		setError(ERR_Syntax);
		return;
	}

	token++;
	if(token->type != TT_identifier)
	{
		setError(ERR_Syntax);
		return;
	}
	Symbol *symbol = findVar(token->str.data,&scope);
	if(getError())
		return;

	token++;
	if(token->type != TT_rightBrace)
	{
		setError(ERR_Syntax);
		return;
	}

	switch(symbol->type)
	{
		case T_String:
			a.data_type = STRING;
			break;
		case T_double:
			a.data_type = DOUBLE;
			break;
		case T_int:
			a.data_type = INT;
			break;
		case T_bool:
			setError(ERR_ReadBool);
			return;
		default:
			break;
	}
	a.var_type = scope;
	a.offset = symbol->index;
	//fprintf(stderr,"READLN a.var_type = %u, a.offset = %ld\n",a.var_type,a.offset);
	generateInstruction(READLN, &a, &b, &c);
	if(activeOffset == &mainOffset)
		*activeOffset = activeContext->locCount + 1;
	else
		*activeOffset = activeContext->locCount + 2;
}

void write()
{
	// keyword write already loaded from STMT
	uint32_t count = term_list();
	a.int_ = count;
	b.offset = (*activeOffset) - 1;
	generateInstruction(WRITE, &a, &b, &c);
	if(activeOffset == &mainOffset)
		*activeOffset = activeContext->locCount + 1;
	else
		*activeOffset = activeContext->locCount + 2;

}

void addFunc(char *name)
{
	argIndex = 0;
	funcSymbol = SymbolFind(mainContext, name);
	if(funcSymbol == NULL)
	{
		funcContext = InitContext();
		if(getError())
			return;
		funcSymbol = SymbolAdd(mainContext, T_FunPointer, name, funcContext, NULL, false);
		if(getError())
			return;
		funcSymbol->stateFunc = FS_Undefined;
		SymbolAdd(funcContext, T_Undefined, name, NULL, NULL, true);
		if(getError())
			return;
		inGST=0;
	}
	// Was already in GST
	else
	{
		funcContext = funcSymbol->funCont;
		// Builtin functions
		if(funcSymbol->index < 0 || funcSymbol->stateFunc == FS_Undefined)
		{
			setError(ERR_RedefVar);
			return;
		}
		inGST=1;
	}
}
void updateFunc(SymbolType returnType,FuncState funcState)
{
	// Function was already in GST
	if(inGST)
	{
		if(funcState == FS_Defined)
		{
			// If was already defined -> error
			if (funcSymbol->stateFunc == FS_Defined)
			{
				setError(ERR_RedefFunc);
				return;
			}
			// In GST is just declaration
			else
			{
				funcSymbol->stateFunc = FS_Defined;
				if(funcContext->returnType != returnType)
				{
					setError(ERR_BadDefArg);
					return;
				}
				int64_t *address = int64_tVectorFirst(funcSymbol->adressVector);
				for (int64_t i = 0; i < funcSymbol->adressVector->used; i++)
				{
					//fprintf(stderr, "index_tape: %d\n", *address);
					Instruction *instr = InstructionVectorAt(tape, *address);
					instr->dst.offset = tape->used - 1;
					address++;
				}
				int64_tVectorFree(funcSymbol->adressVector);
			}
		}
		else
		{
			setError(ERR_DeclrFunc);
			return;
		}
	}
	else
		funcContext->locCount--;

	Symbol *symbol = SymbolFind(mainContext,funcSymbol->name);
	if(getError())
		return;

	funcContext = symbol->funCont;
	Symbol *returnSymbol = SymbolFind(funcContext,funcSymbol->name);
	if(getError())
		return;

	if(funcState == FS_Declared)
	{
		funcSymbol->adressVector = int64_tVectorInit(256);
	}
	returnSymbol->type = returnType;
	funcContext->returnType = returnType;
	funcSymbol->index = tape->used - 1;
	funcSymbol->stateFunc = funcState;
	returnSymbol->index = -funcContext->argCount - 1;
}
void addArgToFunc(SymbolType type, char *name)
{
	if(inGST)
	{
		//Check with declaration
		if(funcContext->argCount <= argIndex)
		{
			setError(ERR_BadDefArg);
			return;
		}
		Symbol *s = funcContext->arg[argIndex];
		if(s->type != type || strcmp(s->name,name) != 0)
		{
			setError(ERR_BadDefArg);
			return;
		}
	}
	else
	{
		AddArgToContext(funcContext, type, name, NULL);
		if(getError())
			return;
	}
	argIndex++;
}
void checkFuncDefinitions()
{
	SymbolList **table = mainContext->locTable;
	for(int32_t index = 0; index<mainContext->locSize; index++)
	{
		for(SymbolList *item = table[index]; item != NULL; item=item->next)
		{
			if(item->data.type == T_FunPointer && item->data.stateFunc != FS_Defined)
			{
				setError(ERR_NoDefFunc);
				return;
			}
		}
	}
}

void addBuiltInFunctions()
{
	Symbol *symbol;
	// length(s : string) : integer
	funcContext = InitContext();
	symbol = SymbolAdd(mainContext, T_FunPointer, "length", funcContext, NULL, false);
	symbol->index = -1;
	symbol->stateFunc = FS_Defined;
	funcContext->returnType = T_int;
	SymbolAdd(funcContext, T_int, "length", NULL, NULL, true);
	AddArgToContext(funcContext, T_String, "s", NULL);
	funcContext->locCount--;

	// copy(s : string; i : integer; n : integer) : string
	funcContext = InitContext();
	symbol = SymbolAdd(mainContext, T_FunPointer, "copy", funcContext, NULL, false);
	symbol->index = -2;
	symbol->stateFunc = FS_Defined;
	funcContext->returnType = T_String;
	SymbolAdd(funcContext, T_String, "copy", NULL, NULL, true);
	AddArgToContext(funcContext, T_String, "s", NULL);
	AddArgToContext(funcContext, T_int, "i", NULL);
	AddArgToContext(funcContext, T_int, "n", NULL);
	funcContext->locCount--;

	// find(s : string; search : string) : integer
	funcContext = InitContext();
	symbol = SymbolAdd(mainContext, T_FunPointer, "find", funcContext, NULL, false);
	symbol->index = -3;
	symbol->stateFunc = FS_Defined;
	funcContext->returnType = T_int;
	SymbolAdd(funcContext, T_int, "find", NULL, NULL, true);
	AddArgToContext(funcContext, T_String, "s", NULL);
	AddArgToContext(funcContext, T_String, "search", NULL);
	funcContext->locCount--;

	// sort(s : string) : string
	funcContext = InitContext();
	symbol = SymbolAdd(mainContext, T_FunPointer, "sort", funcContext, NULL, false);
	symbol->index = -4;
	symbol->stateFunc = FS_Defined;
	funcContext->returnType = T_String;
	SymbolAdd(funcContext, T_String, "name", NULL, NULL, true);
	AddArgToContext(funcContext, T_String, "s", NULL);
	funcContext->locCount--;

	funcContext = NULL;
}
Symbol *findVar(char *name, VariableType *scope)
{
	Symbol *id = SymbolFind(activeContext,name);
	if(id == NULL)
	{
	// Search also in GST
	if(activeContext != mainContext)
		id = SymbolFind(mainContext,name);
		if(id == NULL)
		{
			setError(ERR_UndefVarOrFunction);
			return NULL;
		}
		if(id->type == T_FunPointer)
		{
			setError(ERR_FuncAsVar);
			return NULL;
		}
		*scope = GLOBAL;
		return id;
	}
	if(id->type == T_FunPointer)
	{
		setError(ERR_FuncAsVar);
		return NULL;
	}
	*scope = activeContext == mainContext ? GLOBAL : LOCAL;
	return id;
}
