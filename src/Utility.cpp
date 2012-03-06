/**
 * Utility Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Utility.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#include "Utility.h"

#include <algorithm>
#include <cctype>
#include <cstring>


using std::string;


/* Static Members: */

const int CUtility::ErrorCodes::nCONVERSION_FAILURE = -1;
const int CUtility::ErrorCodes::nNORMAL = 0;


/**
 * Description: Ctor.
 */
CUtility::CUtility()
{
}


/**
 * Description: Dtor.
 */
CUtility::~CUtility()
{
}


/**
 * Description: Trim the leading character of a string.
 * @param szString: Target string.
 * @param szCharacters: Specify a set of characters to trim.
 */
char* CUtility::lTrimString(char* szString, const char* szCharacters)
{
	string sString(szString);
	lTrimString(sString, szCharacters);
	strncpy(szString, sString.c_str(), strlen(szString));

	return szString;
}


/**
 * Description: Trim the specified leading characters of a string.
 * @param s: Target string.
 * @param szCharacters: Specify a set of characters to trim.
 * @return: String been trimmed.
 */
std::string& CUtility::lTrimString(std::string& s, const char* szCharacters)
{
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(szCharacters));
	}

	return s;
}


/**
 * Description: Trim the tailing character of a string.
 * @param szString: Target string.
 * @param szCharacters: Specify a set of characters to trim.
 */
char* CUtility::rTrimString(char* szString, const char* szCharacters)
{
	string sString(szString);
	rTrimString(sString, szCharacters);
	strncpy(szString, sString.c_str(), strlen(szString));

	return szString;
}


/**
 * Description: Trim the specified tailing characters of a string.
 * @param s: Target string.
 * @param szCharacters: Specify a set of characters to trim.
 * @return: String been trimmed.
 */
std::string& CUtility::rTrimString(std::string& s, const char* szCharacters)
{
	if (!s.empty())
	{
		s.erase(s.find_last_not_of(szCharacters) + 1);
	}

	return s;
}


/**
 * Description: Make a string to upper case.
 * @param s: Target string.
 */
std::string& CUtility::stringToUpper(std::string& s)
{
	if (!s.empty())
	{
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
	}

	return s;
}


/**
 * Description: Make a string to lower case.
 * @param s: Target string.
 */
std::string& CUtility::stringToLower(std::string& s)
{
	if (!s.empty())
	{
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	}

	return s;
}


/**
 * Description: Trim the leading and tailing character of a string.
 * @param szString: Target string.
 * @param szCharacters: Specify a set of characters to trim.
 */
char* CUtility::trimString(char* szString, const char* szCharacters)
{
	string sString(szString);
	trimString(sString, szCharacters);
	strncpy(szString, sString.c_str(), strlen(szString));

	return szString;
}


/**
 * Description: Trim the specified leading and tailing characters of a string.
 * @param s: Target string.
 * @param szCharacters: Specify a set of characters to trim.
 * @return: String been trimmed.
 */
std::string& CUtility::trimString(std::string& s, const char* szCharacters)
{
	if (!s.empty())
	{
		s.erase(0, s.find_first_not_of(szCharacters));
		s.erase(s.find_last_not_of(szCharacters) + 1);
	}

	return s;
}

