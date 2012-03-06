/**
 * Atom Iterator Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file AtomIterator.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-12-29
 */


#include "AtomIterator.h"

#include "Exception.h"
#include "InterfaceAtom.h"

#include <sstream>
#include <stdexcept>


using std::list;
using std::string;


/* Static members: */
const string CAtomIterator::MessageTexts::sINVALID_PARAM("Invalid parameter. ");


/**
 * Description: Ctor.
 */
CAtomIterator::CAtomIterator(const std::list<IAtom*>::const_iterator& atomIterator)
	:
	_atomsListIterator(atomIterator)
{
}


/**
 * Description: Copy ctor.
 */
CAtomIterator::CAtomIterator(const IAtomIterator& iterator)
{
	*this = iterator;
}


/**
 * Description: Dtor.
 */
CAtomIterator::~CAtomIterator()
{
}


/**
 * Description: Assignment operator.
 */
CAtomIterator& CAtomIterator::operator=(const CAtomIterator& iterator)
{
	_atomsListIterator = iterator._atomsListIterator;

	return *this;
}


/**
 * Description:
 */
bool CAtomIterator::operator!=(const CAtomIterator& iterator)
{
	return (_atomsListIterator != iterator._atomsListIterator);
}


/* Implementation for IAtomIterator interface: */

/**
 * Description:
 */
IAtom* CAtomIterator::operator*()
{
	return *_atomsListIterator;
}


/**
 * Description:
 */
IAtomIterator& CAtomIterator::operator++()
{
	++_atomsListIterator;

	return *this;
}


/**
 * Descritpion:
 */
bool CAtomIterator::operator!=(const IAtomIterator& iterator)
{
	const CAtomIterator* pSrcIterator = dynamic_cast<const CAtomIterator*>(&iterator);
	if (pSrcIterator)
	{
		return (_atomsListIterator != pSrcIterator->_atomsListIterator);
	}
	// If the right hand side iterator is not an instance of CAtomIterator:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_PARAM
			<< "Parameter (const IAtomIterator& iterator) is not an instance of CAtomIterator. ";
		throw CInvalidArgumentException(msgStream.str());
	}

	return (_atomsListIterator != pSrcIterator->_atomsListIterator);
}


/**
 * Description:
 */
IAtomIterator& CAtomIterator::operator=(const IAtomIterator& iterator)
{
	static const string sLOCATION("CAtomIterator::operator =(): ");

	const CAtomIterator* pSrcIterator = dynamic_cast<const CAtomIterator*>(&iterator);
	if (pSrcIterator)
	{
		_atomsListIterator = pSrcIterator->_atomsListIterator;
	}
	// If the source iterator is not an instance of CAtomIterator:
	else
	{
		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sINVALID_PARAM
			<< "Parameter (const IAtomIterator& iterator) is not an instance of CAtomIterator. ";
		throw CInvalidArgumentException(msgStream.str());
	}

	return *this;
}
