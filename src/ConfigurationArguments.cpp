/**
 * Configuration Argument Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file ConfigurationArguments.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-01-08
 */


#include "ConfigurationArguments.h"

#include "Exception.h"
#include "Utility.h"

#include <fstream>
#include <sstream>


using std::fstream;
using std::map;
using std::string;
using std::stringstream;


/* Static members: */

const char CConfigurationArguments::_cCOMMENT_CHARACTER = '#';
const char CConfigurationArguments::_cASSIGNMENT_CHARACTER = '=';
const string CConfigurationArguments::_sEMPTY_STRING;

/* Error codes: */
const int CConfigurationArguments::ErrorCodes::nNORMAL = 0;
const int CConfigurationArguments::ErrorCodes::nCONVERSION_FAILURE = -1;

/* Message texts: */
const string CConfigurationArguments::MessageTexts::sBAD_FORMAT("Bad format! ");
const string CConfigurationArguments::MessageTexts::sCAN_NOT_OPEN_FILE("Can not open file! ");
const string CConfigurationArguments::MessageTexts::sCAN_NOT_READ_FILE("Can not read file! ");
const string CConfigurationArguments::MessageTexts::sIO_ERROR("IO error! ");
const string CConfigurationArguments::MessageTexts::sREADING_BUFFER_OVERFLOW("Reading buffer overflow! ");


/**
 * Description: Ctor.
 */
CConfigurationArguments::CConfigurationArguments()
{
}


/**
 * Description: Ctor.
 */
CConfigurationArguments::CConfigurationArguments(const std::string& sConfigurationFileName)
{
	appendArguments(sConfigurationFileName);
}


/**
 * Description: Dtor.
 */
CConfigurationArguments::~CConfigurationArguments()
{
}


/**
 * Description:
 * @param sFileName: (IN)
 * @return:
 * @exception:
 *	CFileOpenException:
 *	CBadFormatException
 *	CBufferOverflowException:
 *	CIoErrorException:
 */
int CConfigurationArguments::appendArguments(const std::string& sFileName)
{
	// buffer size
	static const int nBUFFER_SIZE = 256;
	// buffer
	char szBuffer[nBUFFER_SIZE];
	// string wrapper for buffer
	string sBuffer;
	// map for storing new arguments
	map<const string, const string> newArgumentsMap;

	/* Resolve new arguments from file. */
	fstream configurationFileStream(sFileName.c_str(), std::ios::in);
	// If open success:
	if (configurationFileStream.good())
	{
		while (true)
		{
			configurationFileStream.getline(szBuffer, nBUFFER_SIZE);
			// If read success:
			if (configurationFileStream.gcount() > 0)
			{
				sBuffer.assign(szBuffer);
				CUtility::trimString(sBuffer);
				// If buffer not empty:
				if (!sBuffer.empty())
				{
					/* Erase comment text. */
					size_t nCommentPosition = sBuffer.find_first_of(_cCOMMENT_CHARACTER);
					if (nCommentPosition != string::npos)
					{
						sBuffer.erase(nCommentPosition);
						CUtility::trimString(sBuffer);
					}

					/* Resolve argument. */
					// If buffer not empty:
					if (!sBuffer.empty())
					{
						size_t nAssignmentPosition = sBuffer.find_first_of(_cASSIGNMENT_CHARACTER);
						// If assignment character found:
						if (nAssignmentPosition != string::npos)
						{
							/* Get argument name and value. */
							// argument name
							string sArgumentName = sBuffer.substr(0, nAssignmentPosition);
							CUtility::trimString(sArgumentName);
							// argument value
							string sArgumentValue = sBuffer.substr(nAssignmentPosition + 1);
							CUtility::trimString(sArgumentValue);

							/* Store argument name and value. */
							if (!sArgumentName.empty() && !sArgumentValue.empty())
							{
								newArgumentsMap.insert(std::pair<string, string>(sArgumentName, sArgumentValue));
							}
							// If argument name or value is empty:
							else
							{
								stringstream msgStream;
								msgStream
									<< LOCATION_STREAM_INSERTION
									<< MessageTexts::sBAD_FORMAT
									<< sFileName;
								throw CBadFormatException(msgStream.str());
							}
						}
						// If assignment character not found:
						else
						{
							stringstream msgStream;
							msgStream
								<< LOCATION_STREAM_INSERTION
								<< MessageTexts::sBAD_FORMAT
								<< sFileName;
							throw CBadFormatException(msgStream.str());
						}
					}
					// If buffer empty:
					else
					{
						continue;
					}
				}
				// If buffer emty:
				else
				{
					continue;
				}
			}
			// If EOF:
			else if (configurationFileStream.eof())
			{
				break;
			}
			// If overflow:
			else if (configurationFileStream.fail())
			{
				stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< MessageTexts::sREADING_BUFFER_OVERFLOW
					<< sFileName;
				throw CBufferOverflowException(msgStream.str());
			}
			// If IO error:
			else
			{
				stringstream msgStream;
				msgStream
					<< LOCATION_STREAM_INSERTION
					<< MessageTexts::sIO_ERROR
					<< sFileName;
				throw CIoErrorException(msgStream.str());
			}
		}
	}
	// If open failure:
	else
	{
		stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sCAN_NOT_OPEN_FILE
			<< sFileName;
		throw CFileOpenException(msgStream.str());
	}

	/* Append new arguments. */
	_configurationFileNames.push_back(sFileName);
	_argumentsMap.insert(newArgumentsMap.begin(), newArgumentsMap.end());

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 * @param sArgumentName:
 * @return:
 */
bool CConfigurationArguments::existArgument(const std::string& sArgumentName) const
{
	return EXIST(sArgumentName, _argumentsMap);
}


/**
 * Description: Count how many arguments have been read.
 * @return: Number of arguments.
 */
int CConfigurationArguments::getArgumentsCount() const
{
	return _argumentsMap.size();
}


/**
 * Description: Get arguments map.
 * @return: All arguments map, argument name as key, argument value as value.
 */
std::map<std::string, std::string> CConfigurationArguments::getArgumentsMap() const
{
	return _argumentsMap;
}


/** 
 * Description:
 * @param sArgumentName:
 * @return:
 */
std::string CConfigurationArguments::getArgumentValue(const std::string& sArgumentName) const
{
	/* Find argument. */
	map<string, string>::const_iterator iterFind = _argumentsMap.find(sArgumentName);
	if (iterFind != _argumentsMap.end())
	{
		return iterFind->second;
	}
	// If not found:
	else
	{
		return _sEMPTY_STRING;
	}
}


/**
 * Description: Specialized template.
 */
template <>
int CConfigurationArguments::getArgumentValue<std::string>(const std::string& sArgumentName, std::string& argumentValue) const
{
	argumentValue = getArgumentValue(sArgumentName);

	// If success:
	if (!argumentValue.empty())
	{
		return ErrorCodes::nNORMAL;
	}
	// If failure:
	else
	{
		return ErrorCodes::nCONVERSION_FAILURE;
	}
}


/**
 * Description: Count how many configuration files have been read.
 * @return: Number of configuration file.
 */
int CConfigurationArguments::getConfigurationFilesCount() const
{
	return _configurationFileNames.size();
}


/**
 * Description:
 */
bool CConfigurationArguments::isEmpty() const
{
	return _argumentsMap.empty();
}


/**
 * Description:
 * @param sFileName: (IN)
 * @exception:
 *	CFileOpenException:
 *	CBadFormatException
 *	CBufferOverflowException:
 *	CIoErrorException:
 */
int CConfigurationArguments::readArguments(const std::string& sFileName)
{
	/* Emptyi all previous information. */
	_configurationFileNames.clear();
	_argumentsMap.clear();

	/* Read new arguments. */
	appendArguments(sFileName);

	return ErrorCodes::nNORMAL;
}
