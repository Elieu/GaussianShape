/**
 * Exception Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Exception.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-23
 */


#ifndef EXCEPTION_INCLUDE_H
#define EXCEPTION_INCLUDE_H
//


#include <string>


/**
 * Exception relationship map:
 *
 * CException
 *	- CIoException
 *		-- CBadFormatException
 *		-- CFileOpenException
 *		-- CIoErrorException
 *	- CLogicException
 *	- CRuntimeException
 *		-- CBadCastException
 *		-- CBufferOverflowException
 *		-- CElementNotFoundException
 *		-- CInvalidArgumentException
 *	- CBusinessException
 */


// get direct string representation (without macro expansion)
#define STR(x) #x

// string conversion with macro expansion
#define TO_STR(x) STR(x)

// get string representation of current line number
#define __LINE_STR__ TO_STR(__LINE__)

// string of current location in source code
#define __MY_LOCATION__ __FILE__ ": " __LINE_STR__

/* __MY_FUNCTION__ */
// for GCC
#ifdef __GNUC__
	// get current function name with full signature
	#define __MY_FUNCTION__ __PRETTY_FUNCTION__
#endif
// for MS VC
#ifdef _MSC_VER
	// get current function name with full signature
	#define __MY_FUNCTION__ __FUNCSIG__
#endif
// for other compilers
#ifndef __MY_FUNCTION__
	// get current function name only
	#define __MY_FUNCTION__ __FUNCTION__
#endif

// insert detailed location information into a stream
#define LOCATION_STREAM_INSERTION __MY_LOCATION__ << ": " << __MY_FUNCTION__ << ": "


/**
 * Description: Base class for all exceptions.
 */
class CException
{
	/* data: */
public:
private:
	// error message
	std::string _sErrorMessage;

	/* method: */
public:
	CException();
	CException(const std::string& sErrorMessage);
	virtual ~CException();

	const std::string& getErrorMessage() const;

private:
};


/**
 * Description: Logic error.
 */
class CLogicException : public CException
{
	/* data: */
public:
private:

	/* method: */
public:
	CLogicException();
	CLogicException(const std::string& sErrorMessage);
private:
};


/**
 * Description: Runtime exception.
 */
class CRuntimeException : public CException
{
	/* data: */
public:
private:

	/* method: */
public:
	CRuntimeException();
	CRuntimeException(const std::string& sErrorMessage);
private:
};


/**
 * Description: Bad cast exception.
 */
class CBadCastException : public CRuntimeException
{
	/* data: */
public:
private:

	/* method: */
public:
	CBadCastException();
	CBadCastException(const std::string& sErrorMessage);
private:
};


/**
 * Description: Invalid argument exception.
 */
class CInvalidArgumentException : public CRuntimeException
{
	/* data: */
public:
private:

	/* method: */
public:
	CInvalidArgumentException();
	CInvalidArgumentException(const std::string& sErrorMessage);
private:
};


/**
 * Description: Buffer overflow exception.
 */
class CBufferOverflowException : public CRuntimeException
{
	/* data: */
public:
private:

	/* method: */
public:
	CBufferOverflowException();
	CBufferOverflowException(const std::string& sErrorMessage);
private:
};


class CElementNotFoundException : public CRuntimeException
{
	/* data: */
public:
private:

	/* method: */
public:
	CElementNotFoundException();
	CElementNotFoundException(const std::string& sErrorMessage);
private:
};


/**
 * Description: IO exception.
 */
class CIoException : public CException
{
	/* data: */
public:
private:

	/* method: */
public:
	CIoException();
	CIoException(const std::string& sErrorMessage);
private:
};


/**
 * Description: File open exception.
 */
class CFileOpenException : public CIoException
{
	/* data: */
public:
private:

	/* method: */
public:
	CFileOpenException();
	CFileOpenException(const std::string& sErrorMessage);
private:
};


/**
 * Description: Bad format exception.
 */
class CBadFormatException : public CIoException
{
	/* data: */
public:
private:

	/* method: */
public:
	CBadFormatException();
	CBadFormatException(const std::string& sErrorMessage);
private:
};


/**
 * Description: IO error exception.
 */
class CIoErrorException : public CIoException
{
	/* data: */
public:
private:

	/* method: */
public:
	CIoErrorException();
	CIoErrorException(const std::string& sErrorMessage);
private:
};


/**
 * Description: Base class for all specific transaction related exceptions.
 */
class CBusinessException : public CException
{
	/* data: */
public:
private:

	/* method: */
public:
	CBusinessException();
	CBusinessException(const std::string& sErrorMessage);
};


//
#endif
