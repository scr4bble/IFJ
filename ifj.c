#include "system.h"
#include "scanner.h"
#include "parser.h"
#include "interpreter.h"
#include "memory_mgmt.h"

int main(int argc, char *argv[]) {
	// Error code initialized to success
	int ecode = 0;
	bool scanner_initialized = false;
	bool token_vector_initialized = false;

	if(argc <= 1) {
		setError(ERR_CannotOpenFile);
		goto err;
	}

	// Scanner initialization
	Scanner scanner = initScanner();
	scanner_initialized = true;

	assignFile(&scanner.input, argv[1]);

	if(getError())
		goto err;

	// Scanner
	TokenVector *tokenVector = getTokenVector(&scanner);
	token_vector_initialized = true;

	if(getError())
		goto err;

	initInterpret();

	// Recursive descent
	parse(tokenVector);

	if(getError())
		goto err;

	// Interpretation
	runInterpretation();

	// Error handling
err:
	if(getError()) {
		printError();
		ecode = getReturnError();
	}

	// Cleanup
	if(token_vector_initialized)
		destroyTokenVector(tokenVector);
	if(scanner_initialized)
		destroyScanner(&scanner);

	implodeMemory();

	return ecode;
}
