#include "Handle/handle_input.h"
#include "Worker/worker.h"


int32 main(const int32 argc, const char** argv)
{
	Eset::Worker worker(2);

	Eset::HandleInput handleInput(argc, argv);

	const auto Result = handleInput.HandleInputFiles( );
	if (RInvalidPathName == Result)
		Log("InvalidPathName");
	else if (RInvalidNumberOfArguments == Result)
		Log("InvalidNumberOfArguments");

	return 0;
}