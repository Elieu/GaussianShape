// GaussianShape.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include "CommandLineArguments.h"
#include "CommandLineService.h"
#include "ConfigurationArguments.h"
#include "Debug.h"
#include "BusinessException.h"
#include "Utility.h"

#include <iostream>
#include <string>
#include <vector>


using std::endl;
using std::string;
using std::vector;


int main(int argc, const char* argv[])
{
	/* Construct command line arguments. */
	CCommandLineArguments commandLineArguments(argc, argv);

	/* Construct configuration arguments. */
	CConfigurationArguments configurationArguments;
	// command line switch to specify parameter file names
	static const string sPARAMETER_FILE_SWITCH("-paramFile");
	if (!commandLineArguments.isEmptySwitch(sPARAMETER_FILE_SWITCH))
	{
		// Get parameter file names from command line arguments.
		vector<string> parameterFileNames = commandLineArguments.getArguments(sPARAMETER_FILE_SWITCH);
		// Reading parameters from each file.
		FOREACH(iterFileName, parameterFileNames, vector<string>::iterator)
		{
			try
			{
				configurationArguments.appendArguments(*iterFileName);
			}
			catch(CException& exception)
			{
				std::cerr
					<< exception.getErrorMessage()
					<< endl;
			}
		}
	}

	/* Entry point. */
	CCommandLineService commandLineService(configurationArguments);
	try
	{
		commandLineService.startFromCommandLine(commandLineArguments);
	}
	catch(CBusinessException& exception)
	{
		std::cerr
			<< exception.getErrorMessage()
			<< endl;
	}
	catch(CException& exception)
	{
		std::cerr
			<< exception.getErrorMessage()
			<< endl;
	}

	//alignMolecule();
	//stabilityTest();
	debug();

	//std::cout << "Press any key to exit..." << std::endl;
	//getchar();

	return 0;
}


/*
int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}
*/
