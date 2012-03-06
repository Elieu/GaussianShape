/**
 * Command Line Argument Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file CommandLineArguments.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-01-08
 */


#ifndef COMMAND_LINE_ARGUMENTS_INCLUDE_H
#define COMMAND_LINE_ARGUMENTS_INCLUDE_H
//


#include <map>
#include <string>
#include <vector>


/**
 * Description:
 */
class CCommandLineArguments
{
	/* data: */
public:
	/* Error code. */
	struct ErrorCodes
	{
		static const int nNORMAL;

	private:
		ErrorCodes(){};
	};


private:
	// switch indicator character
	static const char _cSWITCH_CHARACTER;

	// command line arguments map, switch string as key and vector of corresponding argument string as element
	std::map<std::string, std::vector<std::string> > _argumentsMap;
	// empty vector of arguments
	static const std::vector<std::string> _EMPTY_ARGUMENTS;
	// switch string for the arguments which are not associated with any switch 
	static const std::string _sUNNAMED_SWITCH;
	// name of the executable (firs element of command line arguments list)
	std::string _sExecutableName;

	/* method: */
public:
	CCommandLineArguments(int nArgumentsNumber, const char* aszArguments[]);
	~CCommandLineArguments();

	bool existSwitch(const std::string& sSwitch) const;
	std::vector<std::string> getArguments(const std::string& sSwitch) const;
	int getArgumentsCount(const std::string& sSwitch) const;
	std::map<std::string, std::vector<std::string> > getArgumentsMap() const;
	std::vector<std::string> getArgumentsOfUnnamedSwitch() const;
	std::string getExecutableName() const;
	bool isEmptySwitch(const std::string& sSwitch) const;

private:
};


//
#endif
