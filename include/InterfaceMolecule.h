/**
 * Molecule Interface Definition
 *
 * Copyright(c) 2010 ECUST.
 * All Rights Reserved.
 *
 * @file InterfaceMolecule.h
 * @author Chaoqian Cai
 * @version v1.0
 * @date 2010-09-26
 */


#ifndef INTERFACE_MOLECULE_INCLUDE_H
#define INTERFACE_MOLECULE_INCLUDE_H
//


#include "InterfaceCloneable.h"

#include <list>
#include <memory>
#include <string>
#include <vector>


class IAtom;
class IAtomIterator;
class IBond;
class IResidue;


/**
 * Description: Abstract molecule interface.
 */
class IMolecule : public ICloneable
{
public:
	virtual int addAtom(const IAtom& atom) = 0;
	virtual int addBond(const IBond& bond) = 0;
	virtual void move(double dX, double dY, double dZ) = 0;
	virtual void moveToCentroid() = 0;
	virtual void clear() = 0;
	virtual void rotateXYZ(double dRadianX, double dRadianY, double dRadianZ) = 0;

	virtual std::auto_ptr<IAtomIterator> beginAtomsIterator() = 0;
	virtual std::auto_ptr<IAtomIterator> endAtomsIterator() = 0;

	virtual IResidue* findResidue(int iId) const = 0;
	virtual int getAtomsCount() const = 0;
	virtual std::list<IAtom*> getAtomsList() const = 0;
	virtual int getBondsCount() const = 0;
	virtual std::list<IBond*> getBondsList() const = 0;
	virtual const std::vector<double>& getCentroid() const = 0;
	virtual const std::string& getMolecularName() const = 0;

	virtual void setMolecularName(const std::string& sName) = 0;

	virtual ~IMolecule(){};
};


//
#endif
