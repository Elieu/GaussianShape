/**
 * Atom Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Atom.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#include "Atom.h"

#include "InterfaceMolecule.h"
#include "InterfaceResidue.h"


using std::string;
using std::vector;


/**
 * Description: Ctor.
 */
CAtom::CAtom() :
	_bHeteroAtomFlag(false),
	_dAtomRadius(0.0),
	_nAtomId(-1),
	_pMolecule(NULL),
	_pResidue(NULL),
	_position(3, 0.0)
{
}


/**
 * Description: Dtor.
 */
CAtom::~CAtom()
{
}

/* Implementation for CAtom class: */

/* Implementation for IAtom interface: */

bool CAtom::isHeavyAtom() const
{
	// If hydrogen:
	if (!_sElementName.compare("H") || !_sElementName.compare("h"))
	{
		return false;
	}
	// If not hydrogen:
	else
	{
		return true;
	}
}


bool CAtom::isHeteroAtom() const
{
	return _bHeteroAtomFlag;
}


int CAtom::getAtomId() const
{
	return _nAtomId;
}


const std::string& CAtom::getAtomType() const
{
	return _sAtomType;
}


const std::string& CAtom::getAtomName() const
{
	return _sAtomName;
}


double CAtom::getAtomRadius() const
{
	return _dAtomRadius;
}


const std::string& CAtom::getElementName() const
{
	return _sElementName;
}


IMolecule* CAtom::getMolecule() const
{
	return _pMolecule;
}


const std::vector<double>& CAtom::getPosition() const
{
	return _position;
}


double CAtom::getPositionX() const
{
	return _position[0];
}


double CAtom::getPositionY() const
{
	return _position[1];
}


double CAtom::getPositionZ() const
{
	return _position[2];
}


IResidue* CAtom::getResidue() const
{
	return _pResidue;
}


void CAtom::setAtomId(int nId)
{
	_nAtomId = nId;
}


void CAtom::setAtomType(const std::string& sAtomType)
{
	_sAtomType = sAtomType;
}


void CAtom::setAtomName(const std::string& sAtomName)
{
	_sAtomName = sAtomName;
}


void CAtom::setAtomRadius(double dRadius)
{
	_dAtomRadius = dRadius;
}


void CAtom::setElementName(const std::string& sName)
{
	_sElementName = sName;
}


void CAtom::setHeteroAtomFlag(bool bFlag)
{
	_bHeteroAtomFlag = bFlag;
}


void CAtom::setMolecule(IMolecule* const pMolecule)
{
	_pMolecule = pMolecule;
}


void CAtom::setPosition(const std::vector<double>& position)
{
	_position = position;
}


void CAtom::setPositionX(double dPosX)
{
	_position[0] = dPosX;
}


void CAtom::setPositionY(double dPosY)
{
	_position[1] = dPosY;
}


void CAtom::setPositionZ(double dPosZ)
{
	_position[2] = dPosZ;
}


void CAtom::setResidue(IResidue* pResidue)
{
	_pResidue = pResidue;
}


/* Implementation for ICloneable interface: */

ICloneable* CAtom::clone() const
{
	CAtom* pAtomClone = new CAtom();
	*pAtomClone = *this;

	return pAtomClone;
}
