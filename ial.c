
#include "ial.h"

/*
    Find substring in string
    original input->data
    looking for find->data

    REQUIRED
        both input structure have to contain LENGTH of data
    Return
      SUCCESSFUL
        index which point to input index (start from "0")
      UNSUCCESSFUL
        length of input length
 */

int FindString(String *input, String *find)
{
    int *Mask = GetFormula(find);
    uint32_t inInd=1, fiInd=1;
    while(inInd<=input->length && fiInd<=find->length)
    {
        if(input->data[inInd-1] == find->data[fiInd-1])
        {
            inInd++;
            fiInd++;
        }
        else
        {
            fiInd = Mask[fiInd-1];
            if(fiInd==0)
            {
                inInd++;
                fiInd++;
            }
        }
    }
    if(fiInd>find->length)
        return inInd-find->length-1;
    return input->length;
}



/*
    Imput is string and it make Mask for KMP algorithm
    required LENGTH in String structure
 */
int *GetFormula(String *find)
{
    int r;
    int *Mask = malloc((find->length) * sizeof(int));
    if(Mask==NULL) return 0;
    Mask[0] = 0;
    for (uint32_t i = 1; i < find->length; ++i)
    {
        r = Mask[i-1];
        while( (r>0) && (find->data[r-1] != find->data[i-1]) )
            r=Mask[r-1];
        Mask[i] = r+1;
    }
    return Mask;
}

/*
    Imput is unordered array of char ended with character == 0
    sort that array based on ASCII number.

    used Recursive Quick Sort
 */

// get length of string and call QuickSortRecursive function
void QuickSort(char arr[])
{
    QuickSortRecursive(arr, strlen(arr)-1);
}

//QuickSort which use recursive function
void QuickSortRecursive(char *arr, int n)
{
    if(n<=1)
        return;// if arr include only one number

    char *l=arr, *r=arr+n-1; //prepare index on border of group
    char p = arr[n/2]; //set pivot (center value)

    while (l <= r) { // while left and right border finished
        if (*l < p) l++; //skip left number if is smaller
        else if (*r > p) r--; //skip right number if is smaller
        else {
            if (l!=r) //if the numbers are different change their value
            {
                *l += *r;
                *r = *l - *r;
                *l -= *r;
            }
            l++; r--; //goto next numbers
        }
    }

    //recursive call for array on left and right side from the pivot
    QuickSortRecursive(arr, r-arr+1);
    QuickSortRecursive(l, arr+n-l);
}


/*
    Imput is unordered array of char ended with character == 0
    sort that array based on ASCII number.

    used nonrecursive Quick Sort
 */
void QuickSortNonRecursive(char arr[])
{
    intVector *vec = intInitVector(8);
    int r,l,i,j;
    intVectorAppend(vec, 0); //left border of sorting
    intVectorAppend(vec, strlen(arr)-1); //rights border of sorting

    while (vec->used != 0)
    {
        r = *intVectorPop(vec); //get right border
        l = *intVectorPop(vec); //get left border
        while(l<r) //while left gruping did not finished
        {
            i=l; j=r; //prepare index on border of group
            int p = (i+j)/2; //set pivot (center value)
            do
            {
                // find higher value of pivot on left side
                while ((arr[i] <= arr[p]) & (i!=p)) i++;
                //find lower value of pivot on right side
                while ((arr[j] >= arr[p]) & (j!=p)) j--;
                if (i<j) //if the values are not same replace them
                {
                    arr[i] += arr[j];
                    arr[j] = arr[i] - arr[j];
                    arr[i] -= arr[j];
                }
            }while (i>j); //while group is not finished (indexes are the same)
            intVectorAppend(vec, i+1); //save left border of right group
            intVectorAppend(vec, r);  //save right border of right group
            r=j; //go to left group and set right border
        }
    }
    intVectorFree(vec);
}


// Calculate index for HashFunction
unsigned int GetHash(const char *str, unsigned htab_size)
{
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h % htab_size;
}


SymbolTable *SymbolFind(Context *FunCont, char *name)
{
    if (FunCont==NULL || name==NULL) return NULL;
    /* Vypocitanie indexu kde je kluc ulozeny a ulozenie prveho listu*/
    SymbolTable *item;
    item = FunCont->LocTable[GetHash(name,FunCont->LocSize)];
    for(; item != NULL; item=item->next)
        if(strcmp(item->data.name, name) == 0)
            break;
    return item;
}

SymbolTable *SymbolAdd(Context *FunCont, SymbolType type, int index, char *name, Context *SymbolContext)
{
    if (SymbolFind(FunCont, name) != NULL)
    {
        // Symbol already exist
        setError(ERR_RedefVar);
        return NULL;
    }
    SymbolTable *newItem = malloc(sizeof(SymbolTable));
    if (newItem == NULL)
    {
        setError(ERR_Allocation);
        return NULL;
    }
    // fill newItem
    newItem->data.type = type;
    newItem->data.index = index;
    newItem->data.name = name;
    newItem->data.FunCont = SymbolContext;

    // reuse index variable
    index = GetHash(name, FunCont->LocSize);

    // save newItem to first position of hashTable[index]
    newItem->next = FunCont->LocTable[index];
    FunCont->LocTable[index] = newItem;
    FunCont->LocCount++;
    return newItem;
}

//DONE
void ContextLocTableFree(Context *t)
{
    if (t==NULL) return;
    if (t->LocTable == NULL) return;
    SymbolTable *item = NULL;
    SymbolTable *temp = NULL;
    for (unsigned long i = 0; i < t->LocSize; i++)
    {
        if (t->LocTable[i]==NULL)
            continue;
        for(item=t->LocTable[i];item != NULL;item=temp)
        {
            temp=item->next;
            SymbolTableFree(item);
        }
        t->LocTable[i] = NULL;
    }
    free(t->LocTable);
    t->LocTable = NULL;
}

void SymbolTableFree(SymbolTable *t)
{
    if (t==NULL) return;
    //destroyString(&t->data.name);
    free(t);
    t = NULL;
}
