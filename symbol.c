/**
 * created 2.10.2014
 * symbol.c IFJ
 */


#include "symbol.h"

Context *InitContext()
{
	// malloc Context
	Context *FunCont = malloc(sizeof(Context));
	if(FunCont == NULL)
	{
		setError(ERR_Allocation)
		return NULL;
	}

	// malloc arg array
	FunCont->arg = malloc(sizeof(struct SymbolList*)*DEFAULT_ARG_NUM);
	if(FunCont->arg == NULL)
	{
		setError(ERR_Allocation);
		free(FunCont);
		return NULL;
	}

	// malloc HASH array
	FunCont->LocTable = malloc(sizeof(struct SymbolList*)*DEFAULT_HASH_SIZE);
	if (FunCont->LocTable == NULL)
	{
		setError(ERR_Allocation);
		free(FunCont->arg);
		free(FunCont);
		return NULL;
	}

	// erase HASH array
	for(unsigned i=0;i<DEFAULT_HASH_SIZE;i++)
		FunCont->LocTable[i]=NULL;

	// erase arg array
	for(unsigned i=0;i<DEFAULT_ARG_NUM;i++)
		FunCont->arg[i]=NULL;

    // set other values of Context
	FunCont->ArgCount = 0;
	FunCont->LocCount = 0;
	FunCont->ArgMax = DEFAULT_ARG_NUM;
	FunCont->LocSize = DEFAULT_HASH_SIZE;
	FunCont->InstrucIndex = 0;
	FunCont->ReturnType = T_Undefined;
	return FunCont;
}

// to add Symbol without argument call htab_addSymbol in ial.c file
// SymbolContext is NULL if SymbolType is variable
Symbol *AddArgToContext(Context *FunCont, SymbolType type, char *name, Context *SymbolContext)
{
	if(FunCont->ArgCount >= FunCont->ArgMax)
	{
		FunCont->ArgMax *= 2;
		FunCont->arg = realloc(FunCont->arg, FunCont->ArgMax);
	}

	Symbol *symbol = SymbolAdd(FunCont, type, name, SymbolContext, NULL);
	FunCont->arg[FunCont->ArgCount++] = symbol;
	FunCont->LocCount--; // Arguments are not included in LocCount number
	return symbol;
}

void FreeContext(Context *FunCont)
{
	if (FunCont==NULL)
		return;

	ContextLocTableFree(FunCont);

	if(FunCont->LocTable != NULL)
		free(FunCont->LocTable);
	FunCont->LocTable = NULL;

	if(FunCont->arg != NULL)
		free(FunCont->arg);
	FunCont->arg = NULL;

	free(FunCont);
	FunCont = NULL;
}
