/**
 * Atom Iterator Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file AtomIterator.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-12-29
 */


#ifndef ATOM_ITERATOR_INCLUDE_H
#define ATOM_ITERATOR_INCLUDE_H
//


#include "InterfaceAtomIterator.h"

#include <list>
#include <string>


class IAtom;


/**
 * Description:
 */
class CAtomIterator : public IAtomIterator
{
	/* data: */
public:
private:
	/* Message text. */
	struct MessageTexts
	{
		static const std::string sINVALID_PARAM;

	private:
		MessageTexts(){};
	};


	// corresponding STL iterator for current atom iterator
	std::list<IAtom*>::const_iterator _atomsListIterator;

	/* method: */
public:
	CAtomIterator(const std::list<IAtom*>::const_iterator& atomIterator);
	CAtomIterator(const IAtomIterator& iterator);
	~CAtomIterator();

	CAtomIterator& operator=(const CAtomIterator& iterator);
	bool operator!=(const CAtomIterator& iterator);

	/* Implementation for IAtomIterator interface: */
	virtual IAtom* operator*();
	virtual IAtomIterator& operator++();
	virtual bool operator!=(const IAtomIterator& iterator);
	virtual IAtomIterator& operator=(const IAtomIterator& iterator);
private:
};


//
#endif
