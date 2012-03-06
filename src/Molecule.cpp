/**
 * Molecule Module
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file Molecule.cpp
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#include "Molecule.h"

#include "AtomIterator.h"
#include "Exception.h"
#include "InterfaceAtom.h"
#include "InterfaceBond.h"
#include "InterfaceResidue.h"

#include <sstream>
#include <stdexcept>


using std::auto_ptr;
using std::list;
using std::map;
using std::string;
using std::vector;


/* Implementation for CMolecule class: */

/* Static members: */

const int CMolecule::ErrorCodes::nNORMAL = 0;

const string CMolecule::MessageTexts::sDYNAMIC_CAST_ERROR("Dynamic cast error! ");
const string CMolecule::MessageTexts::sIVALID_PARAM("Invalid parameter! ");

/**
 * Description: Ctor.
 */
CMolecule::CMolecule()
	:
	_bDirtyCentroid(true),
	_centroid(3, 0.0)
{
}


/**
 * Description: Copy ctor.
 */
CMolecule::CMolecule(const CMolecule& mol) :
	_bDirtyCentroid(mol._bDirtyCentroid),
	_centroid(mol._centroid),
	_sMolecularName(mol._sMolecularName)
{
	// Copy atoms list.
	FOREACH(iterAtom, mol._atomsList, list<IAtom*>::const_iterator)
	{
		IAtom* pAtom = dynamic_cast<IAtom*>((*iterAtom)->clone());
		if (pAtom)
		{
			_atomsList.push_back(pAtom);
		}
		// If dynamic cast fails:
		else
		{
			// self destruct
			clear();

			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sIVALID_PARAM
				<< "Parameter (const CMolecule& mol) do not return an instance of IAtom interface. ";
			throw CInvalidArgumentException(msgStream.str());
		}
	}

	// Copy bonds list.
	FOREACH(iterBond, mol._bondsList, list<IBond*>::const_iterator)
	{
		IBond* pBond = dynamic_cast<IBond*>((*iterBond)->clone());
		if (pBond)
		{
			_bondsList.push_back(pBond);
		}
		// If dynamic cast fails:
		else
		{
			// self destruct
			clear();

			std::stringstream msgStream;
			msgStream
				<< LOCATION_STREAM_INSERTION
				<< MessageTexts::sIVALID_PARAM
				<< "Parameter (const CMolecule& mol) do not return an instance of IBond interface. ";
			throw CInvalidArgumentException(msgStream.str());
		}
	}
}


/**
 * Description: Dtor.
 */
CMolecule::~CMolecule()
{
	/* Delete instances on heap. */
	// Free each atom.
	FOREACH(iterAtom, _atomsList, list<IAtom*>::iterator)
	{
		delete *iterAtom;
	}
	// Free each bond.
	FOREACH(iterBond, _bondsList, list<IBond*>::iterator)
	{
		delete *iterBond;
	}
	// Free each residue.
	for (map<int, IResidue*>::iterator iterResidue = _residuesMap.begin(); iterResidue != _residuesMap.end(); ++ iterResidue)
	{
		delete iterResidue->second;
	}
}


/**
 * Description: Assignment operator.
 */
const CMolecule& CMolecule::operator=(const CMolecule& mol)
{
	static const string sLOCATION("CMolecule::operator=(): ");

	clear();

	// Add each atom;
	FOREACH(iterAtom, mol._atomsList, list<IAtom*>::const_iterator)
	{
		addAtom(**iterAtom);
	}

	// Add each bond.
	FOREACH(iterBond, mol._bondsList, list<IBond*>::const_iterator)
	{
		addBond(**iterBond);
	}

	/* Copy other members. */
	_bDirtyCentroid = mol._bDirtyCentroid;
	_centroid = mol._centroid;
	_sMolecularName = mol._sMolecularName;

	return *this;
}


/**
 * Description:
 */
const std::vector<double>& CMolecule::getCentroid() const
{
	// If need updating centroid coordinate:
	if (_bDirtyCentroid)
	{
		double dX = 0.0;
		double dY = 0.0;
		double dZ = 0.0;

		FOREACH(iterAtom, _atomsList, list<IAtom*>::const_iterator)
		{
			const IAtom& atom = **iterAtom;
			dX += atom.getPositionX();
			dY += atom.getPositionY();
			dZ += atom.getPositionZ();
		}

		const int nAtomsCount = getAtomsCount();
		if (nAtomsCount)
		{
			dX /= nAtomsCount;
			dY /= nAtomsCount;
			dZ /= nAtomsCount;
		}

		_centroid[0] = dX;
		_centroid[1] = dY;
		_centroid[2] = dZ;

		_bDirtyCentroid = false;
	}

	return _centroid;
}


/**
 * Description:
 * @param direction:
 */
void CMolecule::move(double dX, double dY, double dZ)
{
	FOREACH(iterAtom, _atomsList, list<IAtom*>::iterator)
	{
		IAtom& atom = **iterAtom;
		atom.setPositionX(atom.getPositionX() + dX);
		atom.setPositionY(atom.getPositionY() + dY);
		atom.setPositionZ(atom.getPositionZ() + dZ);
	}
}


/* Implementation for IMolecule interface: */

/**
 * Description:
 */
void CMolecule::moveToCentroid()
{
	const vector<double>& CENTROID = getCentroid();
	
	FOREACH(iterAtom, _atomsList, list<IAtom*>::iterator)
	{
		IAtom& atom = **iterAtom;
		atom.setPositionX(atom.getPositionX() - CENTROID[0]);
		atom.setPositionY(atom.getPositionY() - CENTROID[1]);
		atom.setPositionZ(atom.getPositionZ() - CENTROID[2]);
	}
}


/**
 * Description:
 * @param dRadianX:
 * @param dRadianY:
 * @param dRadianZ:
 */
void CMolecule::rotateXYZ(double dRadianX, double dRadianY, double dRadianZ)
{
	const double dSINE_X = sin(dRadianX);
	const double dCOSINE_X = cos(dRadianX);
	const double dSINE_Y = sin(dRadianY);
	const double dCOSINE_Y = cos(dRadianY);
	const double dSINE_Z = sin(dRadianZ);
	const double dCOSINE_Z = cos(dRadianZ);

	FOREACH(iterAtom, _atomsList, list<IAtom*>::iterator)
	{
		IAtom& atom = **iterAtom;
		const double dX = atom.getPositionX();
		const double dY = atom.getPositionY();
		const double dZ = atom.getPositionZ();

		const double dNewX = dX * dCOSINE_Y * dCOSINE_Z
			+ dY * (dSINE_X * dSINE_Y * dCOSINE_Z - dCOSINE_X * dSINE_Z)
			+ dZ * (dCOSINE_X * dSINE_Y * dCOSINE_Z + dSINE_X * dSINE_Z);
		const double dNewY = dX * dCOSINE_Y * dSINE_Z
			+ dY * (dSINE_X * dSINE_Y * dSINE_Z + dCOSINE_X * dCOSINE_Z)
			+ dZ * (dCOSINE_X * dSINE_Y * dSINE_Z - dSINE_X * dCOSINE_Z);
		const double dNewZ = -dX * dSINE_Y + dY * dSINE_X * dCOSINE_Y + dZ * dCOSINE_X * dCOSINE_Y;

		atom.setPositionX(dNewX);
		atom.setPositionY(dNewY);
		atom.setPositionZ(dNewZ);
	}
}


/**
 * Description:
 */
std::auto_ptr<IAtomIterator> CMolecule::beginAtomsIterator()
{
	return auto_ptr<IAtomIterator>(new CAtomIterator(_atomsList.begin()));
}


/**
 * Description:
 */
std::auto_ptr<IAtomIterator> CMolecule::endAtomsIterator()
{
	return auto_ptr<IAtomIterator>(new CAtomIterator(_atomsList.end()));
}


/**
 * Description:
 */
int CMolecule::addAtom(const IAtom& atom)
{
	// Get a shallow copy.
	IAtom* pAtom = dynamic_cast<IAtom*>(atom.clone());
	// If dynamic cast OK:
	if (pAtom)
	{
		/* Deal with related residue. */
		IResidue* pResidue = pAtom->getResidue();
		// If this atom has a related residue:
		if (pResidue)
		{
			IResidue* pExistedResidue = findResidue(pResidue->getId());
			// If this residue exists in current molecule:
			if (pExistedResidue)
			{
				pResidue = pExistedResidue;
				pResidue->addRelatedAtom(*pAtom);
			}
			// If this residue does not exist in current molecule:
			else
			{
				/* Persist this residue. */
				// Get a shallow copy.
				pResidue = dynamic_cast<IResidue*>(pResidue->clone());
				// If dynamic cast OK:
				if (pResidue)
				{
					pResidue->clearRelatedAtoms();
					pResidue->addRelatedAtom(*pAtom);

					_residuesMap[pResidue->getId()] = pResidue;
				}
				// If dynamic cast fails:
				else
				{
					std::stringstream msgStream;
					msgStream
						<< LOCATION_STREAM_INSERTION
						<< "Invalid argument! "
						<< "Parameter (atom.getResidue()) is not an instance of IResidue interface. ";
					throw CInvalidArgumentException(msgStream.str());
				}
			}
		}

		/* Persist this atom. */
		pAtom->setResidue(pResidue);
		_atomsList.push_back(pAtom);
	}
	// If dynamic cast fails:
	else
	{
		// self destruct
		clear();

		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sIVALID_PARAM
			<< "Parameter (atom) is not an instance of IAtom interface. ";
		throw CInvalidArgumentException(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
int CMolecule::addBond(const IBond& bond)
{
	IBond* pBond = dynamic_cast<IBond*>(bond.clone());
	if (pBond)
	{
		_bondsList.push_back(pBond);
	}
	// If dynamic cast fails:
	else
	{
		// self destruct
		clear();

		std::stringstream msgStream;
		msgStream
			<< LOCATION_STREAM_INSERTION
			<< MessageTexts::sIVALID_PARAM
			<< "Parameter (const IBond& bond) is not an instance of IBond interface. ";
		throw std::invalid_argument(msgStream.str());
	}

	return ErrorCodes::nNORMAL;
}


/**
 * Description:
 */
IResidue* CMolecule::findResidue(int iId) const
{
	const map<int, IResidue*>::const_iterator iterResidue = _residuesMap.find(iId);
	// If found:
	if (iterResidue != _residuesMap.end())
	{
		return iterResidue->second;
	}
	// If not found:
	else
	{
		return NULL;
	}
}


int CMolecule::getAtomsCount() const
{
	return _atomsList.size();
}


int CMolecule::getBondsCount() const
{
	return _bondsList.size();
}


std::list<IAtom*> CMolecule::getAtomsList() const
{
	return _atomsList;
}


std::list<IBond*> CMolecule::getBondsList() const
{
	return _bondsList;
}


const std::string& CMolecule::getMolecularName() const
{
	return _sMolecularName;
}


void CMolecule::setMolecularName(const std::string& sName)
{
	_sMolecularName = sName;
}


void CMolecule::clear()
{
	// Free each atom.
	FOREACH(iterAtom, _atomsList, list<IAtom*>::iterator)
	{
		delete *iterAtom;
	}
	_atomsList.clear();

	// Free each bond.
	FOREACH(iterBond, _bondsList, list<IBond*>::iterator)
	{
		delete *iterBond;
	}
	_bondsList.clear();

	// Free each residue.
	for (map<int, IResidue*>::iterator iterResidue = _residuesMap.begin(); iterResidue != _residuesMap.end(); ++ iterResidue)
	{
		delete iterResidue->second;
	}
	_residuesMap.clear();

	_bDirtyCentroid = true;
	_centroid.assign(3, 0.0);
	_sMolecularName.clear();
}


/* Implementation for ICloneable interface: */

ICloneable* CMolecule::clone() const
{
	CMolecule* pCloneMolecule = new CMolecule;
	*pCloneMolecule = *this;

	return pCloneMolecule;
}
