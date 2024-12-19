#include <iostream>

#include "FBXConverter.h"

#include "System/Assert.h"

using namespace fm;
using namespace std;

const int SUCCESS = 0;
const int FAIL = 1;		// TODO make better error code for each failing reason

int main(int argc, const char* const args[])
{
	const int ARGUMENT_LENGTH = 3;

	if (argc < ARGUMENT_LENGTH)
	{
		std::cerr << "Less the value of the argument" << std::endl;
		Assert(false, "Less the value of the argument");
		return FAIL;
	}
	else if (argc > ARGUMENT_LENGTH)
	{
		std::cerr << "There are many argument" << std::endl;
		Assert(false, "There are many argument");
		return FAIL;
	}

	FBXConverter converter;
	const char* const fbxFilePath = args[1];
	const char* const outputPath = args[2];

	bool isLoadSuccess = converter.LoadModelFile(fbxFilePath);
	Assert(isLoadSuccess, "Fbx file loading fail");

	if (isLoadSuccess) 
	{
		converter.CreateCompressModelFile(outputPath);
	}
	else
	{
		std::cerr << "FBX file load fail, check your argument"<<endl
			<<"(arg1: target fbx path, arg2: compressed model file path)" << endl;
		return FAIL;
	}
	
	return SUCCESS;
}

