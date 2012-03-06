/**
 * Atom Iterator Interface Definition
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file InterfaceAtomIterator.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-12-29
 */


#ifndef INTERFACE_ATOM_ITERATOR_INCLUDE_H
#define INTERFACE_ATOM_ITERATOR_INCLUDE_H
//


class IAtom;


/**
 * Description: Atom iterator.
 */
class IAtomIterator
{
public:
	/**
	 * Description: Get the IAtom pointer.
	 */
	virtual IAtom* operator*() = 0;

	/**
	 * Description: Move the iterator forward.
	 */
	virtual IAtomIterator& operator++() = 0;

	/**
	 * Description: Test the inequality of two iterators.
	 */
	virtual bool operator!=(const IAtomIterator& iterator) = 0;

	/**
	 * Description: Assignment from an instance of IAtomIterator.
	 */
	virtual IAtomIterator& operator=(const IAtomIterator& iterator) = 0;

	virtual ~IAtomIterator(){};
};


//
#endif
