/**
 * Utility Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Utility.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#ifndef UTILITY_INCLUDE_H
#define UTILITY_INCLUDE_H
//


#include <sstream>
#include <string>
#include <time.h>


/* Public macro: */

/**
 * Description: This macro traverse every element in a standard STL container, using specified iterator.
 * @param varIterator:
 * @param containerInstance:
 * @param IteratorType:
 */
/* primary version, general for all cases: */
#define FOREACH(varIterator, containerInstance, IteratorType) for (IteratorType (varIterator) = (containerInstance).begin(); (varIterator) != (containerInstance).end(); ++(varIterator))
/* efficiency version, but problematic in rare cases: */
//#define FOREACH(varIterator, containerInstance, IteratorType) for (IteratorType (varIterator) = (containerInstance).begin(), (__ ## varIterator ## _end) = (containerInstance).end(); (varIterator) != (__ ## varIterator ## _end); ++(varIterator))


/**
 * Description: This macro determine if a specified element exist in a standard STL container.
 * @param element:
 * @param containerInstance:
 * @return:
 */
#define EXIST(element, containerInstance) ((containerInstance).find(element) != (containerInstance).end() ? true : false)


/**
 * Description: This macro determine if a specified element exist in a standard STL container.
 * @param element:
 * @param containerInstance:
 * @return:
 */
#define NOT_EXIST(element, containerInstance) ((containerInstance).find(element) == (containerInstance).end() ? true : false)


/**
 * Description: Define the start point for timing.
 */
#define TIME_START() clock_t __startClock = clock()


/**
 * Description: Timing in seconds.
 */
#define TIME_SECONDS(seconds) double seconds = static_cast<double>(clock() - __startClock) / CLOCKS_PER_SEC


/**
 * Description: Timing in ticks.
 */
#define TIME_TICKS(ticks) double ticks = static_cast<double>(clock() - __startClock)


/* Private macro: */

// the set of characters to be trimmed by the "trimString" series functions 
#define trimmedCharacters " \t\r\n"


/**
 * Description:
 */
class CUtility
{
	/* data: */
public:
	/* Error codes. */
	struct ErrorCodes
	{
		static const int nCONVERSION_FAILURE;
		static const int nNORMAL;

	private:
		ErrorCodes() {};
	};

private:

	/* method: */
public:
	CUtility();
	~CUtility();

	static char *lTrimString(char* szString, const char* szCharacters = trimmedCharacters);
	static std::string& lTrimString(std::string& s, const char* szCharacters = trimmedCharacters);
	static char *rTrimString(char* szString, const char* szCharacters = trimmedCharacters);
	static std::string& rTrimString(std::string& s, const char* szCharacters = trimmedCharacters);
	static std::string& stringToUpper(std::string& s);
	static std::string& stringToLower(std::string& s);
	static char *trimString(char* szString, const char* szCharacters = trimmedCharacters);
	static std::string& trimString(std::string& s, const char* szCharacters = trimmedCharacters);

	template<typename T>
	static int parseString(const std::string& sSource, T& targetValue);
	template<typename T>
	static std::string toString(const T& source);
private:
};

/* Template implementation for CUtility class: */

/**
 * Description:
 * @param sSource: (IN)
 * @param targetValue: (OUT)
 * @return:
 */
template<typename T>
int CUtility::parseString(const std::string& sSource, T& targetValue)
{
	std::stringstream sourceStream(sSource);
	sourceStream >> targetValue;

	// If conversion failure:
	if (sourceStream.fail() || sourceStream.bad())
	{
		return ErrorCodes::nCONVERSION_FAILURE;
	}
	// If conversion success:
	else
	{
		return ErrorCodes::nNORMAL;
	}
}

/**
 * Description: Convert a given value to string.
 * @param source: (IN) Input value.
 */
template<typename T>
std::string CUtility::toString(const T& source)
{
	std::stringstream sourceStream;
	sourceStream << source;
	return sourceStream.str();
}


//********************************************************************************************

/* Template implementations: */

/**
 * Description: An analog to auto_ptr class in STL, but suitable for (and only for) array.
 */
template <typename TType>
class auto_array
{
	/* data: */
public:
private:
	// target pointer
	TType* _pType;

	/* method: */
public:
	auto_array(TType* pType);
	~auto_array();

	TType* get() const;
	TType* release();

	/* operators: */
	TType* operator*() const;
	TType* operator->() const;
	auto_array<TType>& operator=(auto_array<TType>& right);
private:
};


/* Template implementation for auto_array class: */

template <typename TType>
auto_array<TType>::auto_array(TType* pType)
{
	_pType = pType;
}


template <typename TType>
auto_array<TType>::~auto_array()
{
	delete []_pType;

}


template <typename TType>
TType* auto_array<TType>::get() const
{
	return _pType;
}


template <typename TType>
TType* auto_array<TType>::release()
{
	TType* pTmp = _pType;
	_pType = NULL;

	return pTmp;
}


template <typename TType>
TType* auto_array<TType>::operator*() const
{
	return _pType;
}


template <typename TType>
TType* auto_array<TType>::operator->() const
{
	return _pType;
}


template <typename TType>
auto_array<TType>& auto_array<TType>::operator=(auto_array<TType>& right)
{
	// If not self assignment:
	if (this != &right)
	{
		delete []_pType;
		_pType = right._pType;
		right._pType = NULL;
	}

	return *this;
}


//
#endif
