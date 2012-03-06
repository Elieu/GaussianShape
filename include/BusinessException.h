/**
 * Business Exception Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file BusinessException.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-03-29
 */


#include "Exception.h"

#include <string>


/**
 * Exception relationship map:
 *
 * CBusinessException
 * 	- CEmptyMoleculeException
 *	- CFileIoException
 *	- CFileNotSupportedException
 *	- CInvalidCommandLineSwitchException
 */


/**
 * Description: Empty molecule exception.
 */
class CEmptyMoleculeException : public CBusinessException
{
	/* data: */
public:
private:

	/* method: */
public:
	CEmptyMoleculeException();
	CEmptyMoleculeException(const std::string& sErrorMessage);
};


/**
 * Description: File not supported exception.
 */
class CFileNotSupportedException : public CBusinessException
{
	/* data: */
public:
private:

	/* method: */
public:
	CFileNotSupportedException();
	CFileNotSupportedException(const std::string& sErrorMessage);
private:
};


/**
 * Description: File IO exception.
 */
class CFileIoException : public CBusinessException
{
	/* data: */
public:
private:

	/* method: */
public:
	CFileIoException();
	CFileIoException(const std::string& sErrorMessage);
private:
};


/**
 * Description: Exception for invalid command line switch.
 */
class CInvalidCommandLineSwitchException : public CBusinessException
{
	/* data: */
public:
private:

	/* method: */
public:
	CInvalidCommandLineSwitchException();
	CInvalidCommandLineSwitchException(const std::string& sErrorMessage);
private:
};
