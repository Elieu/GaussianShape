/**
 * Command Line Argument Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file CommandLineArguments.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-01-08
 */


#include "CommandLineArguments.h"
#include "Utility.h"


using std::map;
using std::string;
using std::vector;


/* Static members: */

const char CCommandLineArguments::_cSWITCH_CHARACTER = '-';
const vector<string> CCommandLineArguments::_EMPTY_ARGUMENTS;
const string CCommandLineArguments::_sUNNAMED_SWITCH("UNNAMED");

const int CCommandLineArguments::ErrorCodes::nNORMAL = 0;


/**
 * Description: Ctor.
 */
CCommandLineArguments::CCommandLineArguments(int nArgumentsNumber, const char* aszArguments[])
	:
	_sExecutableName(aszArguments[0])
{
	// current switch string
	string sCurrentSwitch(_sUNNAMED_SWITCH);
	for (int iArgument = 1; iArgument < nArgumentsNumber; ++ iArgument)
	{
		// If a switch:
		if (aszArguments[iArgument][0] == _cSWITCH_CHARACTER)
		{
			// switch string in upper case
			sCurrentSwitch = string(aszArguments[iArgument]);
			CUtility::stringToUpper(sCurrentSwitch);

			if NOT_EXIST(sCurrentSwitch, _argumentsMap)
			{
				_argumentsMap[sCurrentSwitch] = _EMPTY_ARGUMENTS;
			}
		}
		// If not a switch:
		else
		{
			_argumentsMap[sCurrentSwitch].push_back(string(aszArguments[iArgument]));
		}
	}
}


/**
 * Description: Dtor.
 */
CCommandLineArguments::~CCommandLineArguments()
{
}


/**
 * Description:
 * @param sSwitch:
 * @return:
 */
bool CCommandLineArguments::existSwitch(const std::string& sSwitch) const
{
	// upper case of switch string
	string sSwitchUpper = sSwitch;
	CUtility::stringToUpper(sSwitchUpper);
	return EXIST(sSwitchUpper, _argumentsMap);
}


/**
 * Description:
 * @param sSwitch:
 * @return:
 */
std::vector<std::string> CCommandLineArguments::getArguments(const std::string& sSwitch) const
{
	// upper case of switch string
	string sSwitchUpper = sSwitch;
	CUtility::stringToUpper(sSwitchUpper);

	/* Find argument. */
	map<string, vector<string> >::const_iterator iterFind = _argumentsMap.find(sSwitchUpper);
	// If argument found:
	if (iterFind != _argumentsMap.end())
	{
		return iterFind->second;
	}
	// If argument not found:
	else
	{
		return _EMPTY_ARGUMENTS;
	}

}


/**
 * Description:
 * @param sSwitch: (IN)
 * @return:
 */
int CCommandLineArguments::getArgumentsCount(const std::string& sSwitch) const
{
	return getArguments(sSwitch).size();
}


/**
 * Description: Get all arguments.
 * @return: All arguments map, switch name as key, switch values vector as value.
 */
std::map<std::string, std::vector<std::string> > CCommandLineArguments::getArgumentsMap() const
{
	return _argumentsMap;
}


/**
 * Description:
 * @return:
 */
std::vector<std::string> CCommandLineArguments::getArgumentsOfUnnamedSwitch() const
{
	return getArguments(_sUNNAMED_SWITCH);
}


/**
 * Description:
 * @return
 */
std::string CCommandLineArguments::getExecutableName() const
{
	return _sExecutableName;
}


/**
 * Description:
 * @param sSwitch: (IN)
 * @return:
 */
bool CCommandLineArguments::isEmptySwitch(const std::string& sSwitch) const
{
	return (getArguments(sSwitch).size() == 0);
}
