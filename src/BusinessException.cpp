/**
 * Business Exception Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file BusinessException.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-03-29
 */


#include "BusinessException.h"


/* Implementation for CEmptyMoleculeException class: */

/**
 * Description: Constructor.
 */
CEmptyMoleculeException::CEmptyMoleculeException()
	: CBusinessException()
{
}


/**
 * Description: Constructor.
 */
CEmptyMoleculeException::CEmptyMoleculeException(const std::string& sErrorMessage)
	: CBusinessException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CFileNotSupportedException class: */

/**
 * Description: Ctor.
 */
CFileNotSupportedException::CFileNotSupportedException()
	: CBusinessException()
{
}


/**
 * Description: Ctor.
 */
CFileNotSupportedException::CFileNotSupportedException(const std::string& sErrorMessage)
	: CBusinessException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CFileIoException: */

/**
 * Description: Ctor.
 */
CFileIoException::CFileIoException()
	: CBusinessException()
{
}


/**
 * Description: Ctor.
 */
CFileIoException::CFileIoException(const std::string& sErrorMessage)
	: CBusinessException(sErrorMessage)
{
}


//***************************************************************************

/* Implementation for CInvalidCommandLineSwitch: */

/**
 * Description: Ctor.
 */
CInvalidCommandLineSwitchException::CInvalidCommandLineSwitchException()
	: CBusinessException()
{
}


/**
 * Description: Ctor.
 */
CInvalidCommandLineSwitchException::CInvalidCommandLineSwitchException(const std::string& sErrorMessage)
	: CBusinessException(sErrorMessage)
{
}
