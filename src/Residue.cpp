/**
 * Residue Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Residue.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2011-06-29
 */


#include "Residue.h"


/**
 * Description: Constructor.
 */
CResidue::CResidue()
{
}


/**
 * Description: Destructor.
 */
CResidue::~CResidue()
{
}


/**
 * Description:
 */
int CResidue::addRelatedAtom(const IAtom& atom)
{
	_relatedAtomsVector.push_back(&atom);

	return 0;
}


/**
 * Description:
 */
int CResidue::clearRelatedAtoms()
{
	_relatedAtomsVector.clear();

	return 0;
}


/**
 * Description:
 */
ICloneable* CResidue::clone() const
{
	CResidue* pResidue = new CResidue();
	*pResidue = *this;

	return pResidue;
}


/**
 * Description:
 */
int CResidue::getId() const
{
	return _nId;
}


/**
 * Description:
 */
const std::string& CResidue::getName() const
{
	return _sName;
}


/**
 * Description:
 */
void CResidue::setId(int nId)
{
	_nId = nId;
}


/**
 * Description:
 */
void CResidue::setName(const std::string& sName)
{
	_sName = sName;
}
