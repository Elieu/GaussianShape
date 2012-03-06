/**
 * Exception Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Exception.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-02-23
 */


#include "Exception.h"


/* Implementation for CException class: */

/**
 * Description: Constructor.
 */
CException::CException()
{
}


/**
 * Description: Constructor.
 */
CException::CException(const std::string& sErrorMessage) :
	_sErrorMessage(sErrorMessage)
{
}


/**
 * Description: Dtor.
 */
CException::~CException()
{
}


/**
 * Description: Get related message.
 */
const std::string& CException::getErrorMessage() const
{
	return _sErrorMessage;
}


//***************************************************************************

/* Implementation for CLogicException class: */

/**
 * Description: Constructor.
 */
CLogicException::CLogicException() :
	CException()
{
}


/**
 * Description: Constructor.
 */
CLogicException::CLogicException(const std::string& sErrorMessage) :
	CException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CBufferOverflowException class: */

/**
 * Description: Constructor.
 */
CBufferOverflowException::CBufferOverflowException() :
	CRuntimeException()
{
}


/**
 * Description: Constructor.
 */
CBufferOverflowException::CBufferOverflowException(const std::string& sErrorMessage) :
	CRuntimeException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CElementNotFoundException class: */

/**
 * Description: Constructor.
 */
CElementNotFoundException::CElementNotFoundException() :
	CRuntimeException()
{
}


/**
 * Description: Constructor.
 */
CElementNotFoundException::CElementNotFoundException(const std::string& sErrorMessage) :
	CRuntimeException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CRuntimeException class: */

/**
 * Description: Constructor.
 */
CRuntimeException::CRuntimeException() :
	CException()
{
}


/**
 * Description: Constructor.
 */
CRuntimeException::CRuntimeException(const std::string& sErrorMessage) :
	CException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CBadCastException class: */

/**
 * Description: Constructor.
 */
CBadCastException::CBadCastException() :
	CRuntimeException()
{
}


/**
 * Description: Constructor.
 */
CBadCastException::CBadCastException(const std::string& sErrorMessage) :
	CRuntimeException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CInvalidArgumentException class: */

/**
 * Description: Constructor.
 */
CInvalidArgumentException::CInvalidArgumentException() :
	CRuntimeException()
{
}


/**
 * Description: Constructor.
 */
CInvalidArgumentException::CInvalidArgumentException(const std::string& sErrorMessage) :
	CRuntimeException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CIoException class: */

/**
 * Description: Constructor.
 */
CIoException::CIoException() :
	CException()
{
}


/**
 * Description: Constructor.
 */
CIoException::CIoException(const std::string& sErrorMessage)
	: CException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CFileOpenException: */

/**
 * Description: Constructor.
 */
CFileOpenException::CFileOpenException() :
	CIoException()
{
}


/**
 * Description: Constructor.
 */
CFileOpenException::CFileOpenException(const std::string& sErrorMessage) :
	CIoException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CBadFormateException: */

/**
 * Description: Constructor.
 */
CBadFormatException::CBadFormatException() :
	CIoException()
{
}


/**
 * Description: Constructor.
 */
CBadFormatException::CBadFormatException(const std::string& sErrorMessage) :
	CIoException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CIoErrorException: */

/**
 * Description: Constructor.
 */
CIoErrorException::CIoErrorException()
	: CIoException()
{
}


/**
 * Description: Constructor.
 */
CIoErrorException::CIoErrorException(const std::string& sErrorMessage) :
	CIoException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CBusinessException: */

/**
 * Description: Constructor.
 */
CBusinessException::CBusinessException() :
	CException()
{
}


/**
 * Description: Constructor.
 */
CBusinessException::CBusinessException(const std::string& sErrorMessage) :
	CException(sErrorMessage)
{
}
