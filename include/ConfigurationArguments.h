/**
 * Configuration Argument Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file ConfigurationArguments.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-01-08
 */


#ifndef CONFIGURATION_ARGUMENTS_INCLUDE_H
#define CONFIGURATION_ARGUMENTS_INCLUDE_H
//


#include <map>
#include <sstream>
#include <string>
#include <vector>


/** 
 * Description:
 */
class CConfigurationArguments
{
	/* data: */
public:
	/* Error code. */
	struct ErrorCodes
	{
		static const int nNORMAL;
		static const int nCONVERSION_FAILURE;
		
	private:
		ErrorCodes() {};
	};


private:
	/* Message text. */
	struct MessageTexts
	{
		static const std::string sBAD_FORMAT;
		static const std::string sCAN_NOT_OPEN_FILE;
		static const std::string sCAN_NOT_READ_FILE;
		static const std::string sIO_ERROR;
		static const std::string sREADING_BUFFER_OVERFLOW;

	private:
		MessageTexts() {};
	};


	// comment indicator character
	static const char _cCOMMENT_CHARACTER;
	// assignment indicator character
	static const char _cASSIGNMENT_CHARACTER;

	// arguments map
	std::map<std::string, std::string> _argumentsMap;
	// configuration file names
	std::vector<std::string> _configurationFileNames;
	// empty string
	static const std::string _sEMPTY_STRING;

	/* method: */
public:
	CConfigurationArguments();
	CConfigurationArguments(const std::string& sConfigurationFileName);
	~CConfigurationArguments();

	int appendArguments(const std::string& sFileName);
	bool existArgument(const std::string& sArgumentName) const;
	int getArgumentsCount() const;
	std::map<std::string, std::string> getArgumentsMap() const;
	std::string getArgumentValue(const std::string& sArgumentName) const;
	int getConfigurationFilesCount() const;
	bool isEmpty() const;
	int readArguments(const std::string& sFileName);

	template <typename T>
	int getArgumentValue(const std::string& sArgumentName, T& argumentValue) const;

private:
};


/* Template implementation for CConfigurationArguments class: */

/**
 * Description:
 * @param sArgumentName: (IN)
 * @param argumentValue: (OUT)
 * @return:
 *	ErrorCodes::nNORMAL:
 *	ErrorCodes::nCONVERSION_FAILURE:
 */
template <typename T>
int CConfigurationArguments::getArgumentValue(const std::string& sArgumentName, T& argumentValue) const
{
	std::stringstream valueStream(getArgumentValue(sArgumentName));
	valueStream >> argumentValue;

	// If conversion succeeded:
	if (!valueStream.fail() && !valueStream.bad())
	{
		return ErrorCodes::nNORMAL;
	}
	// If conversion failed:
	else
	{
		return ErrorCodes::nCONVERSION_FAILURE;
	}
}


/**
 * Description: Specialized template.
 */
template <>
int CConfigurationArguments::getArgumentValue<std::string>(const std::string& sArgumentName, std::string& argumentValue) const;


//
#endif
