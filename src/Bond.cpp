/**
 * Bond Modle
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Bond.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-10-01
 */


#include "Bond.h"

#include <string>


using std::string;


/* Implementation for CBond: */

/**
 * Description: Ctor.
 */
CBond::CBond()
	:
	_nBondedAtomXId(-1),
	_nBondedAtomYId(-1),
	_nBondId(-1)
{
}


/**
 * Description: Dtor.
 */
CBond::~CBond()
{
}


/* Implementation for IBond interface: */

int CBond::getBondedAtomXId() const
{
	return _nBondedAtomXId;
}


int CBond::getBondedAtomYId() const
{
	return _nBondedAtomYId;
}


int CBond::getBondId() const
{
	return _nBondId;
}


const string& CBond::getBondType() const
{
	return _sBondType;
}


void CBond::setBondedAtomXId(int nId)
{
	_nBondedAtomXId = nId;
}


void CBond::setBondedAtomYId(int nId)
{
	_nBondedAtomYId = nId;
}


void CBond::setBondId(int nId)
{
	_nBondId = nId;
}


void CBond::setBondType(const std::string& sType)
{
	_sBondType = sType;
}


/* Implementation for ICloneable<IBond> interface: */

ICloneable* CBond::clone() const
{
	CBond* pBondClone = new CBond();
	*pBondClone = *this;

	return pBondClone;
}
