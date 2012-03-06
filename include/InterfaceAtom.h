/**
 * Atom Interface Definition
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file InterfaceAtom.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#ifndef INTERFACE_ATOM_INCLUDE_H
#define INTERFACE_ATOM_INCLUDE_H
//


#include "InterfaceCloneable.h"

#include <string>
#include <vector>


class IMolecule;
class IResidue;


/**
 * Description: Abstract atom interface.
 */
class IAtom : public ICloneable
{
public:
	/**
	 * Description: Whether heavy atom.
	 */
	virtual bool isHeavyAtom() const = 0;

	/**
	 * Description:
	 */
	virtual bool isHeteroAtom() const = 0;

	/**
	 * Description: Get atom ID.
	 */
	virtual int getAtomId() const = 0;

	/**
	 * Description: Get atom type.
	 */
	virtual const std::string& getAtomType() const = 0;

	/**
	 * Description: Get atom name.
	 */
	virtual const std::string& getAtomName() const = 0;

	/**
	 * Description: Get atom radius.
	 */
	virtual double getAtomRadius() const = 0;

	/**
	 * Description: Get element name.
	 */
	virtual const std::string& getElementName() const = 0;

	/**
	 * Description: Get parent molecule.
	 */
	virtual IMolecule* getMolecule() const = 0;

	/**
	 * Description: Get atom position.
	 */
	virtual const std::vector<double>& getPosition() const = 0;

	/**
	 * Description: Get X coordinate.
	 */
	virtual double getPositionX() const = 0;

	/**
	 * Description: Get Y coordinate.
	 */
	virtual double getPositionY() const = 0;

	/**
	 * Description: Get Z coordinate.
	 */
	virtual double getPositionZ() const = 0;

	/**
	 * Description: Get related residue.
	 */
	virtual IResidue* getResidue() const = 0;

	//***********************************************************

	/**
	 * Description: Set atom ID.
	 */
	virtual void setAtomId(int nId) = 0;

	/**
	 * Description: Set atom type.
	 */
	virtual void setAtomType(const std::string& sAtomType) = 0;

	/**
	 * Description: Set atom name.
	 */
	virtual void setAtomName(const std::string& sAtomName) = 0;

	/**
	 * Description: Set atom radius.
	 */
	virtual void setAtomRadius(double dRadius) = 0;

	/**
	 * Description: Set element name.
	 */
	virtual void setElementName(const std::string& sName) = 0;

	/**
	 * Description:
	 */
	virtual void setHeteroAtomFlag(bool bHeteroAtomFlag) = 0;

	/**
	 * Description: Set parent molecule.
	 */
	virtual void setMolecule(IMolecule* const pMolecule) = 0;

	/**
	 * Description: Set atom position.
	 */
	virtual void setPosition(const std::vector<double>& position) = 0;

	/**
	 * Description: Set X coordinate.
	 */
	virtual void setPositionX(double dPosX) = 0;

	/**
	 * Description: Set Y coordinate.
	 */
	virtual void setPositionY(double dPosY) = 0;

	/**
	 * Description: Set Z coordinate.
	 */
	virtual void setPositionZ(double dPosZ) = 0;

	/**
	 * Description: Set related residue.
	 */
	virtual void setResidue(IResidue* pResidue) = 0;

	virtual ~IAtom(){};
};


//
#endif
