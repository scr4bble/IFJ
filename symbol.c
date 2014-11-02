/**
 * created 2.10.2014
 * symbol.c IFJ
 */


#include "symbol.h"

Context *InitContext(uint32_t ArgMax)
{
	Context *FunCont = malloc(sizeof(Context));
	if(FunCont == NULL)
	{
		setError(ERR_Allocation)
		return NULL;
	}
	FunCont->arg = malloc(sizeof(struct SymbolTable*)*ArgMax);
	if(FunCont->arg == NULL)
	{
		setError(ERR_Allocation);
		free(FunCont);
		return NULL;
	}
	FunCont->LocTable = malloc(sizeof(struct SymbolTable*)*DEFAULT_HASH_SIZE);
	if (FunCont == NULL)
	{
		setError(ERR_Allocation);
		free(FunCont->arg);
		free(FunCont);
		return NULL;
	}
	// erase HASH array
	for(unsigned i=0;i<DEFAULT_HASH_SIZE;i++)
		FunCont->LocTable[i]=NULL;

    // set other values of Context
	FunCont->ArgCount = 0;
	FunCont->ArgMax = ArgMax;
	FunCont->LocCount = DEFAULT_HASH_SIZE;
	return FunCont;
}

// DONE
// to add Symbol without argument call htab_addSymbol in ial.c file
// SymbolContext is empty if Variable is selected
void AddArgToContext(Context *FunCont, SymbolType type, int index, char *name, Context *SymbolContext)
{
	if(FunCont->ArgCount >= FunCont->ArgMax)
	{
		setError(ERR_Unknown);
		return;
	}
	// add symbol to hash table + add his pointer to array of arguments
	FunCont->arg[FunCont->ArgCount] = SymbolAdd(FunCont, type, index, name, SymbolContext);
	FunCont->ArgCount++;
}

// done
void FreeContext(Context *FunCont)
{
	if (FunCont==NULL)
		return;
	ContextLocTableFree(FunCont);
	if(FunCont->LocTable != NULL)
		free(FunCont->LocTable);
	if(FunCont->arg != NULL)
		free(FunCont->arg);
	free(FunCont);
	FunCont = NULL;
}


//GenVectorFunctions(SymbolType);
// InitSymbolTypeVector